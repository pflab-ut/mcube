/**
 * @file arch/arm/raspi3/mailbox.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


/* mailbox message buffer */
volatile unsigned int  __attribute__((aligned(16))) mbox[VIDEOCORE_MAILBOX_SIZE];


/**
 * call a mailbox. Returns 0 on failure, non-zero on success
 */
static int call_mbox(unsigned char ch)
{
  unsigned int r = (((unsigned int)((unsigned long)&mbox) & ~0xf) | (ch & 0xf));
  /* wait until we can write to the mailbox */
  do {
    nop();
  } while (mmio_in32(VIDEOCORE_MAILBOX_STATUS) & VIDEOCORE_MAILBOX_FULL);
  /* write the address of our message to the mailbox with channel identifier */
  mmio_out32(VIDEOCORE_MAILBOX_WRITE, r);
  /* now wait for the response */
  while (1) {
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
  mbox[0] = VIDEOCORE_MAILBOX_SIZE;
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
  mbox[0] = VIDEOCORE_MAILBOX_SIZE;
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
    mbox[0] = VIDEOCORE_MAILBOX_SIZE;
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
  delay(150);
  mmio_out32(GPPUDCLK0, 0xffffffff);
  mmio_out32(GPPUDCLK1, 0xffffffff);
  delay(150);
  mmio_out32(GPPUDCLK0, 0);
  /* flush GPIO setup */
  mmio_out32(GPPUDCLK1, 0);

  // power off the SoC (GPU + CPU)
  r = mmio_in32(POWER_MANAGEMENT_RSTS);
  r &= ~0xfffffaaa;
  r |= 0x555;    // partition 63 used to indicate halt
  mmio_out32(POWER_MANAGEMENT_RSTS, POWER_MANAGEMENT_WDOG_MAGIC | r);
  mmio_out32(POWER_MANAGEMENT_WDOG, POWER_MANAGEMENT_WDOG_MAGIC | 10);
  mmio_out32(POWER_MANAGEMENT_RSTC, POWER_MANAGEMENT_WDOG_MAGIC | POWER_MANAGEMENT_RSTC_FULLRST);
}
