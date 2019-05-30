/**
 * @file arch/aarch64/raspi3/videocore_mailbox.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

/* mailbox message buffer */
volatile unsigned int __attribute__((aligned(32))) mbox[VIDEOCORE_MAILBOX_SIZE];

/**
 * call a mailbox. Returns 0 on failure, non-zero on success
 */
static int call_mbox(unsigned char ch)
{
  unsigned int r = (((unsigned int)((unsigned long) &mbox) & ~0xf) | (ch & 0xf));

  /* wait until we can write to the mailbox */
  do {
    nop();
  } while (mmio_in32(VIDEOCORE_MAILBOX_STATUS) & VIDEOCORE_MAILBOX_FULL);

  /* write the address of our message to the mailbox with channel identifier */
  mmio_out32(VIDEOCORE_MAILBOX_WRITE, r);

  /* now wait for the response */
  while (true) {
    /* is there a response? */
    do {
      nop();
    } while (mmio_in32(VIDEOCORE_MAILBOX_STATUS) & VIDEOCORE_MAILBOX_EMPTY);

    /* is it a response to our message? */
    if (mmio_in32(VIDEOCORE_MAILBOX_READ) == r) {
      /* is it a valid successful response? */
      return VIDEOCORE_MAILBOX_RESPONSE == mbox[1];
    }
  }

  return 0;
}

/* get the board's unique serial number with a mailbox call */
unsigned long get_serial_number(void)
{
  unsigned long serial_number;

  /* buffer size in bytes (including the header values, the end tag and padding) */
  mbox[0] = 8 * sizeof(unsigned int);
  /* buffer request/response code
   * Request codes:
   ** 0x00000000: process request
   ** All other values reserved
   * Response codes:
   ** 0x80000000: request successful
   ** 0x80000001: error parsing request buffer (partial response)
   ** All other values reserved
   */
  mbox[1] = VIDEOCORE_MAILBOX_REQUEST;
  /* tag ID */
  mbox[2] = VIDEOCORE_MAILBOX_TAG_GET_BOARD_SERIAL;
  /* value buffer size in bytes */
  mbox[3] = 8;
  /* Request codes:
   ** b31 clear: request
   ** b30-b0: reserved
   * Response codes:
   ** b31 set: response
   ** b30-b0: value length in bytes
   */
  mbox[4] = 0;
  /* clear output buffer */
  mbox[5] = 0;
  mbox[6] = 0;
  /* end tag */
  mbox[7] = VIDEOCORE_MAILBOX_TAG_LAST;

  if (call_mbox(VIDEOCORE_MAILBOX_CH_PROPERTY_TAGS_ARM_TO_VC)) {
    /* set serial number */
    serial_number = mbox[6];
    serial_number = (serial_number << 32) | mbox[5];
  } else {
    print("Unable to query serial!\n");
    serial_number = ~0x0UL;
  }

  return serial_number;
}


void setup_pl011_uart(void)
{
  /* buffer size in bytes (including the header values, the end tag and padding) */
  mbox[0] = 9 * sizeof(unsigned int);
  /* buffer request/response code
   * Request codes:
   ** 0x00000000: process request
   ** All other values reserved
   * Response codes:
   ** 0x80000000: request successful
   ** 0x80000001: error parsing request buffer (partial response)
   ** All other values reserved
   */
  mbox[1] = VIDEOCORE_MAILBOX_REQUEST;
  /* tag ID */
  mbox[2] = VIDEOCORE_MAILBOX_TAG_SET_CLOCK_RATE;
  /* value buffer size in bytes */
  mbox[3] = 12;
  /* Request codes:
   ** b31 clear: request
   ** b30-b0: reserved
   * Response codes:
   ** b31 set: response
   ** b30-b0: value length in bytes
   */
  mbox[4] = 0;
  /* UART clock ID */
  mbox[5] = CLOCK_ID_UART;
  /* Rate (in Hz) */
  mbox[6] = PL011_UART_HZ;
  /* skip setting turbo */
  mbox[7] = 0;
  mbox[8] = VIDEOCORE_MAILBOX_TAG_LAST;
  call_mbox(VIDEOCORE_MAILBOX_CH_PROPERTY_TAGS_ARM_TO_VC);

  //  print("mbox[5] = %u mbox[6] = %u\n", mbox[5], mbox[6]);
}


/* shutdown */
void power_off(void)
{
  unsigned long r;

  // power off devices one by one
  for (r = 0; r < 16; r++) {
    /* buffer size in bytes (including the header values, the end tag and padding) */
    mbox[0] = 8 * sizeof(unsigned int);
    /* buffer request/response code
     * Request codes:
     ** 0x00000000: process request
     ** All other values reserved
     * Response codes:
     ** 0x80000000: request successful
     ** 0x80000001: error parsing request buffer (partial response)
     ** All other values reserved
     */
    mbox[1] = VIDEOCORE_MAILBOX_REQUEST;
    /* tag ID */
    mbox[2] = VIDEOCORE_MAILBOX_TAG_SET_POWER_STATE;
    /* value buffer size in bytes */
    mbox[3] = 8;
    /* Request codes:
    ** b31 clear: request
    ** b30-b0: reserved
    * Response codes:
    ** b31 set: response
    ** b30-b0: value length in bytes
    */
    mbox[4] = 8;
    /* device id */
    mbox[5] = (unsigned int) r;
    /* bit 0: off, bit 1: no wait */
    mbox[6] = 0;
    mbox[7] = VIDEOCORE_MAILBOX_TAG_LAST;
    call_mbox(VIDEOCORE_MAILBOX_CH_PROPERTY_TAGS_ARM_TO_VC);
  }

  // power off gpio pins (but not VCC pins)
  mmio_out32(GPFSEL0, 0);
  mmio_out32(GPFSEL1, 0);
  mmio_out32(GPFSEL2, 0);
  mmio_out32(GPFSEL3, 0);
  mmio_out32(GPFSEL4, 0);
  mmio_out32(GPFSEL5, 0);
  mmio_out32(GPPUD, 0);
  delay_cycles(150);
  mmio_out32(GPPUDCLK0, 0xffffffff);
  mmio_out32(GPPUDCLK1, 0xffffffff);
  delay_cycles(150);
  mmio_out32(GPPUDCLK0, 0);
  /* flush GPIO setup */
  mmio_out32(GPPUDCLK1, 0);

  // power off the SoC (GPU + CPU)
  r = mmio_in32(POWER_MANAGEMENT_RSTS);
  r &= ~0xfffffaaa;
  r |= 0x555;    // partition 63 used to indicate halt
  mmio_out32(POWER_MANAGEMENT_RSTS, POWER_MANAGEMENT_WDOG_MAGIC | r);
  mmio_out32(POWER_MANAGEMENT_WDOG, POWER_MANAGEMENT_WDOG_MAGIC | 10);
  mmio_out32(POWER_MANAGEMENT_RSTC,
             POWER_MANAGEMENT_WDOG_MAGIC | POWER_MANAGEMENT_RSTC_FULLRST);
}


/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

unsigned int fb_width, fb_height, fb_pitch;
unsigned char *fb_ptr;

/**
 * Set screen resolution to 1024x768
 */
void init_frame_buffer(struct frame_buffer *fb)
{
  /* buffer size in bytes (including the header values, the end tag and padding) */
  mbox[0] = 35 * sizeof(unsigned int);
  /* buffer request/response code
   * Request codes:
   ** 0x00000000: process request
   ** All other values reserved
   * Response codes:
   ** 0x80000000: request successful
   ** 0x80000001: error parsing request buffer (partial response)
   ** All other values reserved
   */
  mbox[1] = VIDEOCORE_MAILBOX_REQUEST;

  mbox[2] = VIDEOCORE_MAILBOX_TAG_SET_PHYSICAL_DISPLAY_WIDTH_HEIGHT;  //set phy wh
  mbox[3] = 8;
  mbox[4] = 8;
  mbox[5] = fb->width;         //FrameBufferInfo.width
  mbox[6] = fb->height;          //FrameBufferInfo.height

  mbox[7] = VIDEOCORE_MAILBOX_TAG_SET_VIRTUAL_BUFFER_WIDTH_HEIGHT;  //set virt wh
  mbox[8] = 8;
  mbox[9] = 8;
  mbox[10] = fb->virtual_width;        //FrameBufferInfo.virtual_width
  mbox[11] = fb->virtual_height;         //FrameBufferInfo.virtual_height

  mbox[12] = VIDEOCORE_MAILBOX_TAG_SET_VIRTUAL_OFFSET; //set virt offset
  mbox[13] = 8;
  mbox[14] = 8;
  mbox[15] = fb->x_offset;           //FrameBufferInfo.x_offset
  mbox[16] = fb->y_offset;           //FrameBufferInfo.y_offset

  mbox[17] = VIDEOCORE_MAILBOX_TAG_SET_DEPTH; //set depth
  mbox[18] = 4;
  mbox[19] = 4;
  mbox[20] = fb->depth;          //FrameBufferInfo.depth

  mbox[21] = VIDEOCORE_MAILBOX_TAG_SET_PIXEL_ORDER; //set pixel order
  mbox[22] = 4;
  mbox[23] = 4;
  mbox[24] = fb->state;           //RGB, not BGR preferably

  mbox[25] =
    VIDEOCORE_MAILBOX_TAG_ALLOCATE_BUFFER; //get framebuffer, gets alignment on request
  mbox[26] = 8;
  mbox[27] = 8;
  mbox[28] = fb->pointer;        //FrameBufferInfo.pointer
  mbox[29] = fb->size;           //FrameBufferInfo.size

  mbox[30] = VIDEOCORE_MAILBOX_TAG_GET_PITCH; //get pitch
  mbox[31] = 4;
  mbox[32] = 4;
  mbox[33] = fb->pitch;           //FrameBufferInfo.pitch

  mbox[34] = VIDEOCORE_MAILBOX_TAG_LAST;

  if (call_mbox(VIDEOCORE_MAILBOX_CH_PROPERTY_TAGS_ARM_TO_VC)
      && mbox[20] == fb->depth && mbox[28] != 0) {
    mbox[28] &= 0x3fffffff;
    fb_width = mbox[5];
    fb_height = mbox[6];
    fb_pitch = mbox[33];
    fb_ptr = (void *)((unsigned long) mbox[28]);
  } else {
    printk("Unable to set screen resolution to 1024x768x32\n");
  }
}

/* Call this macro repeatedly.  After each use, the pixel data can be extracted  */

#define HEADER_PIXEL(data, pixel) do {                                  \
    pixel[0] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4));         \
    pixel[1] = ((((data[1] - 33) & 0xf) << 4) | ((data[2] - 33) >> 2)); \
    pixel[2] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33)));      \
    data += 4;                                                          \
  } while (0)

/**
 * Show a picture
 */
void fb_show_picture(char *data, int width, int height)
{
  int x, y;
  char pixel[4];

  fb_ptr += ((fb_height - height) / 2) * fb_pitch + (fb_width - width) * 2;

  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      HEADER_PIXEL(data, pixel);
      *((unsigned int *) fb_ptr) = *((unsigned int *) &pixel);
      fb_ptr += 4;
    }

    fb_ptr += fb_pitch - width * 4;
  }
}

extern volatile unsigned char _binary___lib_font_psf_start;


/**
 * Display a string
 */
void fb_print(int x, int y, const char *s)
{
  // get our font
  psf_t *font = (psf_t *) &_binary___lib_font_psf_start;

  //  psf_t *font = (psf_t *) &_binary_font_psf_start;
  // draw next character if it's not zero
  while (*s) {
    // get the offset of the glyph. Need to adjust this to support unicode table
    unsigned char *glyph = (unsigned char *) &_binary___lib_font_psf_start
                           + font->headersize + (*((unsigned char *) s) < font->numglyph ? *s : 0)
                           * font->bytesperglyph;
    // calculate the offset on screen
    int offs = (y * font->height * fb_pitch) + (x * (font->width + 1) * 4);
    // variables
    unsigned int i, j;
    int line, mask, bytesperline = (font->width + 7) / 8;

    // handle carrige return
    if (*s == '\r') {
      x = 0;
    } else

      // new line
      if (*s == '\n') {
        x = 0;
        y++;
      } else {
        // display a character
        for (j = 0; j < font->height; j++) {
          // display one row
          line = offs;
          mask = 1 << (font->width - 1);

          for (i = 0; i < font->width; i++) {
            // if bit set, we use white color, otherwise black
            *((unsigned int *)(fb_ptr + line)) = ((int) * glyph) & mask ? 0xffffff : 0;
            mask >>= 1;
            line += 4;
          }

          // adjust to next line
          glyph += bytesperline;
          offs += fb_pitch;
        }

        x++;
      }

    // next character
    s++;
  }
}
