/**
 * @file include/arm/raspi3/videocore_mailbox.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_RASPI3_VIDEOCORE_MAILBOX_H__
#define __MCUBE_ARM_RASPI3_VIDEOCORE_MAILBOX_H__

#define VIDEOCORE_MAILBOX_READ   (VIDEOCORE_MAILBOX_BASE + 0x00)
#define VIDEOCORE_MAILBOX_POLL   (VIDEOCORE_MAILBOX_BASE + 0x10)
#define VIDEOCORE_MAILBOX_SENDER (VIDEOCORE_MAILBOX_BASE + 0x14)
#define VIDEOCORE_MAILBOX_STATUS (VIDEOCORE_MAILBOX_BASE + 0x18)
#define VIDEOCORE_MAILBOX_CONFIG (VIDEOCORE_MAILBOX_BASE + 0x1c)
#define VIDEOCORE_MAILBOX_WRITE  (VIDEOCORE_MAILBOX_BASE + 0x20)


#define VIDEOCORE_MAILBOX_SIZE       140
#define VIDEOCORE_MAILBOX_REQUEST    0

/* channels */
#define VIDEOCORE_MAILBOX_CH_POWER                   0
#define VIDEOCORE_MAILBOX_CH_FRAME_BUFFER            1
#define VIDEOCORE_MAILBOX_CH_VIRTUAL_UART            2
#define VIDEOCORE_MAILBOX_CH_VCHIQ                   3
#define VIDEOCORE_MAILBOX_CH_LEDS                    4
#define VIDEOCORE_MAILBOX_CH_BUTTONS                 5
#define VIDEOCORE_MAILBOX_CH_TOUCH_SCREEN            6
#define VIDEOCORE_MAILBOX_CH_COUNT                   7
#define VIDEOCORE_MAILBOX_CH_PROPERTY_TAGS_ARM_TO_VC 8
#define VIDEOCORE_MAILBOX_CH_PROPERTY_TAGS_VC_TO_ARM 9


/* tags */
#define VIDEOCORE_MAILBOX_TAG_LAST                               0x00000000
#define VIDEOCORE_MAILBOX_TAG_GET_FIRMWARE_REVISION              0x00000001
#define VIDEOCORE_MAILBOX_TAG_SET_CURSOR_INFO                    0x00008010
#define VIDEOCORE_MAILBOX_TAG_SET_CURSOR_STATE                   0x00008011
#define VIDEOCORE_MAILBOX_TAG_GET_BOARD_MODEL                    0x00010001
#define VIDEOCORE_MAILBOX_TAG_GET_BOARD_REVISION                 0x00010002
#define VIDEOCORE_MAILBOX_TAG_GET_BOARD_MAC_ADDRESS              0x00010003
#define VIDEOCORE_MAILBOX_TAG_GET_BOARD_SERIAL                   0x00010004
#define VIDEOCORE_MAILBOX_TAG_GET_ARM_MEMORY                     0x00010005
#define VIDEOCORE_MAILBOX_TAG_GET_VC_MEMORY                      0x00010006
#define VIDEOCORE_MAILBOX_TAG_GET_CLOCKS                         0x00010007
#define VIDEOCORE_MAILBOX_TAG_GET_POWER_STATE                    0x00020001
#define VIDEOCORE_MAILBOX_TAG_GET_TIMING                         0x00020002
#define VIDEOCORE_MAILBOX_TAG_SET_POWER_STATE                    0x00028001
#define VIDEOCORE_MAILBOX_TAG_GET_CLOCK_STATE                    0x00030001
#define VIDEOCORE_MAILBOX_TAG_GET_CLOCK_RATE                     0x00030002
#define VIDEOCORE_MAILBOX_TAG_GET_VOLTAGE                        0x00030003
#define VIDEOCORE_MAILBOX_TAG_GET_MAX_CLOCK_RATE                 0x00030004
#define VIDEOCORE_MAILBOX_TAG_GET_MAX_VOLTAGE                    0x00030005
#define VIDEOCORE_MAILBOX_TAG_GET_TEMPERATURE                    0x00030006
#define VIDEOCORE_MAILBOX_TAG_GET_MIN_CLOCK_RATE                 0x00030007
#define VIDEOCORE_MAILBOX_TAG_GET_MIN_VOLTAGE                    0x00030008
#define VIDEOCORE_MAILBOX_TAG_GET_TURBO                          0x00030009
#define VIDEOCORE_MAILBOX_TAG_GET_MAX_TEMPERATURE                0x0003000a
#define VIDEOCORE_MAILBOX_TAG_ALLOCATE_MEMORY                    0x0003000c
#define VIDEOCORE_MAILBOX_TAG_LOCK_MEMORY                        0x0003000d
#define VIDEOCORE_MAILBOX_TAG_UNLOCK_MEMORY                      0x0003000e
#define VIDEOCORE_MAILBOX_TAG_RELEASE_MEMORY                     0x0003000f
#define VIDEOCORE_MAILBOX_TAG_EXECUTE_CODE                       0x00030010
#define VIDEOCORE_MAILBOX_TAG_GET_DISPMANX_RESOURCE_MEM_HANDLE   0x00030014
#define VIDEOCORE_MAILBOX_TAG_GET_EDID_BLOCK                     0x00030020
#define VIDEOCORE_MAILBOX_TAG_SET_CLOCK_STATE                    0x00038001
#define VIDEOCORE_MAILBOX_TAG_SET_CLOCK_RATE                     0x00038002
#define VIDEOCORE_MAILBOX_TAG_SET_VOLTAGE                        0x00038003
#define VIDEOCORE_MAILBOX_TAG_SET_TURBO                          0x00038009

#define VIDEOCORE_MAILBOX_TAG_ALLOCATE_BUFFER                    0x00040001
#define VIDEOCORE_MAILBOX_TAG_BLANK_SCREEN                       0x00040002
#define VIDEOCORE_MAILBOX_TAG_GET_PHYSICAL_DISPLAY_WIDTH_HEIGHT  0x00040003
#define VIDEOCORE_MAILBOX_TAG_GET_VIRTUAL_BUFFER_WIDTH_HEIGHT    0x00040004
#define VIDEOCORE_MAILBOX_TAG_GET_DEPTH                          0x00040005
#define VIDEOCORE_MAILBOX_TAG_GET_ALPHA_MODE                     0x00040007
#define VIDEOCORE_MAILBOX_TAG_GET_PITCH                          0x00040008
#define VIDEOCORE_MAILBOX_TAG_GET_VIRTUAL_OFFSET                 0x00040009
#define VIDEOCORE_MAILBOX_TAG_GET_OVERSCAN                       0x0004000a
#define VIDEOCORE_MAILBOX_TAG_GET_PALETTE                        0x0004000b
#define VIDEOCORE_MAILBOX_TAG_TEST_PHYSICAL_DISPLAY_WIDTH_HEIGHT 0x00044003
#define VIDEOCORE_MAILBOX_TAG_TEST_VIRTUAL_BUFFER_WIDTH_HEIGHT   0x00044004
#define VIDEOCORE_MAILBOX_TAG_TEST_DEPTH                         0x00044005
#define VIDEOCORE_MAILBOX_TAG_TEST_PIXEL_ORDER                   0x00044006
#define VIDEOCORE_MAILBOX_TAG_TEST_ALPHA_MODE                    0x00044007
#define VIDEOCORE_MAILBOX_TAG_TEST_VIRTUAL_OFFSET                0x00044009
#define VIDEOCORE_MAILBOX_TAG_TEST_OVERSCAN                      0x0004400a
#define VIDEOCORE_MAILBOX_TAG_TEST_PALETTE                       0x0004400b
#define VIDEOCORE_MAILBOX_TAG_RELEASE_BUFFER                     0x00048001
#define VIDEOCORE_MAILBOX_TAG_SET_PHYSICAL_DISPLAY_WIDTH_HEIGHT  0x00048003
#define VIDEOCORE_MAILBOX_TAG_SET_VIRTUAL_BUFFER_WIDTH_HEIGHT    0x00048004
#define VIDEOCORE_MAILBOX_TAG_SET_DEPTH                          0x00048005
#define VIDEOCORE_MAILBOX_TAG_SET_PIXEL_ORDER                    0x00048006
#define VIDEOCORE_MAILBOX_TAG_SET_ALPHA_MODE                     0x00048007
#define VIDEOCORE_MAILBOX_TAG_SET_VIRTUAL_OFFSET                 0x00048009
#define VIDEOCORE_MAILBOX_TAG_SET_OVERSCAN                       0x0004800a
#define VIDEOCORE_MAILBOX_TAG_SET_PALETTE                        0x0004800b
#define VIDEOCORE_MAILBOX_TAG_GET_COMMAND_LINE                   0x00050001
#define VIDEOCORE_MAILBOX_TAG_GET_DMA_CHANNELS                   0x00060001



#define VIDEOCORE_MAILBOX_RESPONSE 0x80000000
#define VIDEOCORE_MAILBOX_ERROR    0x80000001

#define VIDEOCORE_MAILBOX_FULL     0x80000000
#define VIDEOCORE_MAILBOX_EMPTY    0x40000000


/* unique power device IDs */
#define POWER_DEVICE_ID_SD_CARD 0x0
#define POWER_DEVICE_ID_UART0   0x1
#define POWER_DEVICE_ID_UART1   0x2
#define POWER_DEVICE_ID_USB_HCD 0x3
#define POWER_DEVICE_ID_I2C0    0x4
#define POWER_DEVICE_ID_I2C1    0x5
#define POWER_DEVICE_ID_I2C2    0x6
#define POWER_DEVICE_ID_SPI     0x7
#define POWER_DEVICE_ID_CCP2TX  0x8

/* unique clock IDs */
/* 0x0: reserved */
#define CLOCK_ID_EMMC  0x1
#define CLOCK_ID_UART  0x2
#define CLOCK_ID_ARM   0x3
#define CLOCK_ID_CORE  0x4
#define CLOCK_ID_V3D   0x5
#define CLOCK_ID_H264  0x6
#define CLOCK_ID_ISP   0x7
#define CLOCK_ID_SDRAM 0x8
#define CLOCK_ID_PIXEL 0x9
#define CLOCK_ID_PWM   0xa

/* unique voltage device IDs */
/* 0x0: reserved */
#define VOLTAGE_ID_CORE    0x1
#define VOLTAGE_ID_SDRAM_C 0x2
#define VOLTAGE_ID_SDRAM_P 0x3
#define VOLTAGE_ID_SDRAM_I 0x4

/* pixel order state */
#define PIXEL_ORDER_STATE_BGR 0x0
#define PIXEL_ORDER_STATE_RGB 0x1


#ifndef __ASSEMBLY__

/**
 * @struct frame_buffer
 * @brief Frame buffer information.
 */
struct frame_buffer {
  unsigned int width;
  unsigned int height;

  unsigned int virtual_width;
  unsigned int virtual_height;

  unsigned int x_offset;
  unsigned int y_offset;

  unsigned int depth;

  unsigned int state;

  unsigned int pointer;
  unsigned int size;

  unsigned int pitch;
};



/**
 * @struct pc_screen_font_info
 * @brief PC screen font information.
 */
struct pc_screen_font_info {
  unsigned int magic;
  unsigned int version;
  unsigned int headersize;
  unsigned int flags;
  unsigned int numglyph;
  unsigned int bytesperglyph;
  unsigned int height;
  unsigned int width;
  unsigned char glyphs;
} __packed;

typedef struct pc_screen_font_info psf_t;


unsigned long get_serial_number(void);
void setup_pl011_uart(void);
void fb_show_picture(char *data, int width, int height);
void fb_print(int x, int y, const char *s);

void init_frame_buffer(struct frame_buffer *fb);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_ARM_RASPI3_VIDEOCORE_MAILBOX_H__ */

