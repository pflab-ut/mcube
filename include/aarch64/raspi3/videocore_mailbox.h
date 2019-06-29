/**
 * @file include/aarch64/raspi3/videocore_mailbox.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_RASPI3_VIDEOCORE_MAILBOX_H__
#define __MCUBE_AARCH64_RASPI3_VIDEOCORE_MAILBOX_H__

/**
 * @def VIDEOCORE_MAILBOX_READ
 * @brief Videocore mailbox read.
 */
#define VIDEOCORE_MAILBOX_READ (VIDEOCORE_MAILBOX_BASE + 0x00)

/**
 * @def VIDEOCORE_MAILBOX_POLL
 * @brief Videocore mailbox poll.
 */
#define VIDEOCORE_MAILBOX_POLL (VIDEOCORE_MAILBOX_BASE + 0x10)

/**
 * @def VIDEOCORE_MAILBOX_SENDER
 * @brief Videocore mailbox sender.
 */
#define VIDEOCORE_MAILBOX_SENDER (VIDEOCORE_MAILBOX_BASE + 0x14)

/**
 * @def VIDEOCORE_MAILBOX_STATUS
 * @brief Videocore mailbox status.
 */
#define VIDEOCORE_MAILBOX_STATUS (VIDEOCORE_MAILBOX_BASE + 0x18)

/**
 * @def VIDEOCORE_MAILBOX_CONFIG
 * @brief Videocore mailbox configuration.
 */
#define VIDEOCORE_MAILBOX_CONFIG (VIDEOCORE_MAILBOX_BASE + 0x1c)

/**
 * @def VIDEOCORE_MAILBOX_WRITE
 * @brief Videocore mailbox write.
 */
#define VIDEOCORE_MAILBOX_WRITE (VIDEOCORE_MAILBOX_BASE + 0x20)

/**
 * @def VIDEOCORE_MAILBOX_SIZE
 * @brief Videocore mailbox size.
 */
#define VIDEOCORE_MAILBOX_SIZE 140

/**
 * @def VIDEOCORE_MAILBOX_REQUEST
 * @brief Videocore mailbox request.
 */
#define VIDEOCORE_MAILBOX_REQUEST 0

/* channels */

/**
 * @def VIDEOCORE_MAILBOX_CH_POWER
 * @brief Videocore mailbox channel power.
 */
#define VIDEOCORE_MAILBOX_CH_POWER 0

/**
 * @def VIDEOCORE_MAILBOX_CH_FRAME_BUFFER
 * @brief Videocore mailbox channel frame buffer.
 */
#define VIDEOCORE_MAILBOX_CH_FRAME_BUFFER 1

/**
 * @def VIDEOCORE_MAILBOX_CH_VIRTUAL_UART
 * @brief Videocore mailbox channel virtual UART.
 */
#define VIDEOCORE_MAILBOX_CH_VIRTUAL_UART 2

/**
 * @def VIDEOCORE_MAILBOX_CH_VCHIQ
 * @brief Videocore mailbox channel VC interface queue.
 */
#define VIDEOCORE_MAILBOX_CH_VCHIQ 3

/**
 * @def VIDEOCORE_MAILBOX_CH_LEDS
 * @brief Videocore mailbox channel LEDs.
 */
#define VIDEOCORE_MAILBOX_CH_LEDS 4

/**
 * @def VIDEOCORE_MAILBOX_CH_BUTTONS
 * @brief Videocore mailbox channel buttons.
 */
#define VIDEOCORE_MAILBOX_CH_BUTTONS 5

/**
 * @def VIDEOCORE_MAILBOX_CH_TOUCH_SCREEN
 * @brief Videocore mailbox channel touch screen.
 */
#define VIDEOCORE_MAILBOX_CH_TOUCH_SCREEN 6

/**
 * @def VIDEOCORE_MAILBOX_CH_COUNT
 * @brief Videocore mailbox channel count.
 */
#define VIDEOCORE_MAILBOX_CH_COUNT 7

/**
 * @def VIDEOCORE_MAILBOX_CH_PROPERTY_TAGS_ARM_TO_VC
 * @brief Videocore mailbox channel property tags ARM to VC.
 */
#define VIDEOCORE_MAILBOX_CH_PROPERTY_TAGS_ARM_TO_VC 8

/**
 * @def VIDEOCORE_MAILBOX_CH_PROPERTY_TAGS_VC_TO_ARM
 * @brief Videocore mailbox channel property tags VC to ARM.
 */
#define VIDEOCORE_MAILBOX_CH_PROPERTY_TAGS_VC_TO_ARM 9


/* tags */

/**
 * @def VIDEOCORE_MAILBOX_TAG_LAST
 * @brief Videocore mailbox tag list.
 */
#define VIDEOCORE_MAILBOX_TAG_LAST 0x00000000

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_FIRMWARE_REVISION
 * @brief Videocore mailbox tag get firmware revision.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_FIRMWARE_REVISION 0x00000001

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_CURSOR_INFO
 * @brief Videocore mailbox tag set cursor information.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_CURSOR_INFO 0x00008010

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_CURSOR_STATE
 * @brief Videocore mailbox tag set cursor state.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_CURSOR_STATE 0x00008011

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_BOARD_MODEL
 * @brief Videocore mailbox tag get board model.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_BOARD_MODEL 0x00010001

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_BOARD_REVISION
 * @brief Videocore mailbox tag get board revision.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_BOARD_REVISION 0x00010002

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_BOARD_MAC_ADDRESS
 * @brief Videocore mailbox tag get board MAC address.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_BOARD_MAC_ADDRESS 0x00010003

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_BOARD_SERIAL
 * @brief Videocore mailbox tag get board serial.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_BOARD_SERIAL 0x00010004

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_ARM_MEMORY
 * @brief Videocore mailbox tag get ARM memory.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_ARM_MEMORY 0x00010005

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_VC_MEMORY
 * @brief Videocore mailbox tag get VC memory.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_VC_MEMORY 0x00010006

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_CLOCKS
 * @brief Videocore mailbox tag get clocks.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_CLOCKS 0x00010007

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_POWER_STATE
 * @brief Videocore mailbox tag get power state.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_POWER_STATE 0x00020001

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_TIMING
 * @brief Videocore mailbox tag get timing.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_TIMING 0x00020002

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_POWER_STATE
 * @brief Videocore mailbox tag set power state.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_POWER_STATE 0x00028001

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_CLOCK_STATE
 * @brief Videocore mailbox tag get clock state.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_CLOCK_STATE 0x00030001

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_CLOCK_RATE
 * @brief Videocore mailbox tag get clock rate.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_CLOCK_RATE 0x00030002

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_VOLTAGE
 * @brief Videocore mailbox tag get voltage.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_VOLTAGE 0x00030003

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_MAX_CLOCK_RATE
 * @brief Videocore mailbox tag get max clock rate.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_MAX_CLOCK_RATE 0x00030004

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_MAX_VOLTAGE
 * @brief Videocore mailbox tag get maximum voltage.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_MAX_VOLTAGE 0x00030005

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_TEMPERATURE
 * @brief Videocore mailbox tag get temperature.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_TEMPERATURE 0x00030006

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_MIN_CLOCK_RATE
 * @brief Videocore mailbox tag get minimum clock rate.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_MIN_CLOCK_RATE 0x00030007

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_MIN_VOLTAGE
 * @brief Videocore mailbox tag get minimum voltage.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_MIN_VOLTAGE 0x00030008

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_TURBO
 * @brief Videocore mailbox tag get turbo.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_TURBO 0x00030009

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_MAX_TEMPERATURE
 * @brief Videocore mailbox tag get maximum temperature.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_MAX_TEMPERATURE 0x0003000a

/**
 * @def VIDEOCORE_MAILBOX_TAG_ALLOCATE_MEMORY
 * @brief Videocore mailbox tag allocate memory.
 */
#define VIDEOCORE_MAILBOX_TAG_ALLOCATE_MEMORY 0x0003000c

/**
 * @def VIDEOCORE_MAILBOX_TAG_LOCK_MEMORY
 * @brief Videocore mailbox tag lock memory.
 */
#define VIDEOCORE_MAILBOX_TAG_LOCK_MEMORY 0x0003000d

/**
 * @def VIDEOCORE_MAILBOX_TAG_UNLOCK_MEMORY
 * @brief Videocore mailbox tag unlock memory.
 */
#define VIDEOCORE_MAILBOX_TAG_UNLOCK_MEMORY 0x0003000e

/**
 * @def VIDEOCORE_MAILBOX_TAG_RELEASE_MEMORY
 * @brief Videocore mailbox tag release memory.
 */
#define VIDEOCORE_MAILBOX_TAG_RELEASE_MEMORY 0x0003000f

/**
 * @def VIDEOCORE_MAILBOX_TAG_EXECUTE_CODE
 * @brief Videocore mailbox tag execute code.
 */
#define VIDEOCORE_MAILBOX_TAG_EXECUTE_CODE 0x00030010

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_DISPMANX_RESOURCE_MEM_HANDLE
 * @brief Videocore mailbox tag get dismanx resource mem handle.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_DISPMANX_RESOURCE_MEM_HANDLE 0x00030014

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_EDID_BLOCK
 * @brief Videocore mailbox tag get EDID block.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_EDID_BLOCK 0x00030020

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_CLOCK_STATE
 * @brief Videocore mailbox tag set clock state.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_CLOCK_STATE 0x00038001

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_CLOCK_RATE
 * @brief Videocore mailbox tag set clock rate.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_CLOCK_RATE 0x00038002

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_VOLTAGE
 * @brief Videocore mailbox tag set voltage.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_VOLTAGE 0x00038003

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_TURBO
 * @brief Videocore mailbox tag set turbo.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_TURBO 0x00038009

/**
 * @def VIDEOCORE_MAILBOX_TAG_ALLOCATE_BUFFER
 * @brief Videocore mailbox tag allocate buffer.
 */
#define VIDEOCORE_MAILBOX_TAG_ALLOCATE_BUFFER 0x00040001

/**
 * @def VIDEOCORE_MAILBOX_TAG_BLANK_SCREEN
 * @brief Videocore mailbox tag blank screen.
 */
#define VIDEOCORE_MAILBOX_TAG_BLANK_SCREEN 0x00040002

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_PHYSICAL_DISPLAY_WIDTH_AND_HEIGHT
 * @brief Videocore mailbox tag get physical display width and height.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_PHYSICAL_DISPLAY_WIDTH_AND_HEIGHT 0x00040003

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_VIRTUAL_BUFFER_WIDTH_AND_HEIGHT
 * @brief Videocore mailbox tag get virtual buffer width and height.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_VIRTUAL_BUFFER_WIDTH_AND_HEIGHT 0x00040004

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_DEPTH
 * @brief Videocore mailbox tag get depth.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_DEPTH 0x00040005

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_ALPHA_MODE
 * @brief Videocore mailbox tag get alpha mode.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_ALPHA_MODE 0x00040007

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_PITCH
 * @brief Videocore mailbox tag get pitch.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_PITCH 0x00040008

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_VIRTUAL_OFFSET
 * @brief Videocore mailbox tag get virtual offset.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_VIRTUAL_OFFSET 0x00040009

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_OVERSCAN
 * @brief Videocore mailbox tag get overscan.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_OVERSCAN 0x0004000a

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_PALETTE
 * @brief Videocore mailbox tag get palette.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_PALETTE 0x0004000b

/**
 * @def VIDEOCORE_MAILBOX_TAG_TEST_PHYSICAL_DISPLAY_WIDTH_AND_HEIGHT
 * @brief Videocore mailbox tag test physical display width and height.
 */
#define VIDEOCORE_MAILBOX_TAG_TEST_PHYSICAL_DISPLAY_WIDTH_AND_HEIGHT 0x00044003

/**
 * @def VIDEOCORE_MAILBOX_TAG_TEST_VIRTUAL_BUFFER_WIDTH_AND_HEIGHT
 * @brief Videocore mailbox tag test virtual buffer width and height.
 */
#define VIDEOCORE_MAILBOX_TAG_TEST_VIRTUAL_BUFFER_WIDTH_AND_HEIGHT 0x00044004

/**
 * @def VIDEOCORE_MAILBOX_TAG_TEST_DEPTH
 * @brief Videocore mailbox tag test depth.
 */
#define VIDEOCORE_MAILBOX_TAG_TEST_DEPTH 0x00044005

/**
 * @def VIDEOCORE_MAILBOX_TAG_TEST_PIXEL_ORDER
 * @brief Videocore mailbox tag test pixel order.
 */
#define VIDEOCORE_MAILBOX_TAG_TEST_PIXEL_ORDER 0x00044006

/**
 * @def VIDEOCORE_MAILBOX_TAG_TEST_ALPHA_MODE
 * @brief Videocore mailbox tag test alpha mode.
 */
#define VIDEOCORE_MAILBOX_TAG_TEST_ALPHA_MODE 0x00044007

/**
 * @def VIDEOCORE_MAILBOX_TAG_TEST_VIRTUAL_OFFSET
 * @brief Videocore mailbox tag test virtual offset.
 */
#define VIDEOCORE_MAILBOX_TAG_TEST_VIRTUAL_OFFSET 0x00044009

/**
 * @def VIDEOCORE_MAILBOX_TAG_TEST_OVERSCAN
 * @brief Videocore mailbox tag test overscan.
 */
#define VIDEOCORE_MAILBOX_TAG_TEST_OVERSCAN 0x0004400a

/**
 * @def VIDEOCORE_MAILBOX_TAG_TEST_PALETTE
 * @brief Videocore mailbox tag test palette.
 */
#define VIDEOCORE_MAILBOX_TAG_TEST_PALETTE 0x0004400b

/**
 * @def VIDEOCORE_MAILBOX_TAG_RELEASE_BUFFER
 * @brief Videocore mailbox tag release buffer.
 */
#define VIDEOCORE_MAILBOX_TAG_RELEASE_BUFFER 0x00048001

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_PHYSICAL_DISPLAY_WIDTH_AND_HEIGHT
 * @brief Videocore mailbox tag set physical display width and height.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_PHYSICAL_DISPLAY_WIDTH_AND_HEIGHT 0x00048003

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_VIRTUAL_BUFFER_WIDTH_AND_HEIGHT
 * @brief Videocore mailbox tag set virtual buffer width and height.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_VIRTUAL_BUFFER_WIDTH_AND_HEIGHT 0x00048004

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_DEPTH
 * @brief Videocore mailbox tag set depth.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_DEPTH 0x00048005

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_PIXEL_ORDER
 * @brief Videocore mailbox tag set pixel order.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_PIXEL_ORDER 0x00048006

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_ALPHA_MODE
 * @brief Videocore mailbox tag set alpha mode.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_ALPHA_MODE 0x00048007

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_VIRTUAL_OFFSET
 * @brief Videocore mailbox tag set virtual offset.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_VIRTUAL_OFFSET 0x00048009

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_OVERSCAN
 * @brief Videocore mailbox tag set overscan.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_OVERSCAN 0x0004800a

/**
 * @def VIDEOCORE_MAILBOX_TAG_SET_PALETTE
 * @brief Videocore mailbox tag set palette.
 */
#define VIDEOCORE_MAILBOX_TAG_SET_PALETTE 0x0004800b

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_COMMAND_LINE
 * @brief Videocore mailbox tag get command line.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_COMMAND_LINE 0x00050001

/**
 * @def VIDEOCORE_MAILBOX_TAG_GET_DMA_CHANNELS
 * @brief Videocore mailbox tag get DMA channels.
 */
#define VIDEOCORE_MAILBOX_TAG_GET_DMA_CHANNELS 0x00060001

/**
 * @def VIDEOCORE_MAILBOX_RESPONSE
 * @brief Videocore mailbox response.
 */
#define VIDEOCORE_MAILBOX_RESPONSE 0x80000000

/**
 * @def VIDEOCORE_MAILBOX_ERROR
 * @brief Videocore mailbox error.
 */
#define VIDEOCORE_MAILBOX_ERROR 0x80000001

/**
 * @def VIDEOCORE_MAILBOX_FULL
 * @brief Videocore mailbox full.
 */
#define VIDEOCORE_MAILBOX_FULL 0x80000000

/**
 * @def VIDEOCORE_MAILBOX_EMPTY
 * @brief Videocore mailbox empty.
 */
#define VIDEOCORE_MAILBOX_EMPTY 0x40000000


/* unique power device IDs */

/**
 * @def POWER_DEVICE_ID_SD_CARD
 * @brief Power device ID SD card.
 */
#define POWER_DEVICE_ID_SD_CARD 0x0

/**
 * @def POWER_DEVICE_ID_UART0
 * @brief Power device ID UART 0.
 */
#define POWER_DEVICE_ID_UART0 0x1

/**
 * @def POWER_DEVICE_ID_UART1
 * @brief Power device ID UART 1.
 */
#define POWER_DEVICE_ID_UART1 0x2

/**
 * @def POWER_DEVICE_ID_USB_HCD
 * @brief Power device ID USB HCD.
 */
#define POWER_DEVICE_ID_USB_HCD 0x3

/**
 * @def POWER_DEVICE_ID_I2C0
 * @brief Power device ID I2C 0.
 */
#define POWER_DEVICE_ID_I2C0 0x4

/**
 * @def POWER_DEVICE_ID_I2C1
 * @brief Power device ID I2C 1.
 */
#define POWER_DEVICE_ID_I2C1 0x5

/**
 * @def POWER_DEVICE_ID_I2C2
 * @brief Power device ID I2C 2.
 */
#define POWER_DEVICE_ID_I2C2 0x6

/**
 * @def POWER_DEVICE_ID_SPI
 * @brief Power device ID SPI.
 */
#define POWER_DEVICE_ID_SPI 0x7

/**
 * @def POWER_DEVICE_ID_CCP2TX
 * @brief Power device ID CCP2TX.
 */
#define POWER_DEVICE_ID_CCP2TX 0x8

/* unique clock IDs */
/* 0x0: reserved */

/**
 * @def CLOCK_ID_EMMC
 * @brief Clock ID EMMC.
 */
#define CLOCK_ID_EMMC 0x1

/**
 * @def CLOCK_ID_UART
 * @brief Clock ID UART.
 */
#define CLOCK_ID_UART 0x2

/**
 * @def CLOCK_ID_ARM
 * @brief Clock ID ARM.
 */
#define CLOCK_ID_ARM 0x3

/**
 * @def CLOCK_ID_CORE
 * @brief Clock ID core.
 */
#define CLOCK_ID_CORE 0x4

/**
 * @def CLOCK_ID_V3D
 * @brief Clock ID V3D.
 */
#define CLOCK_ID_V3D 0x5

/**
 * @def CLOCK_ID_H264
 * @brief Clock ID H264.
 */
#define CLOCK_ID_H264 0x6

/**
 * @def CLOCK_ID_ISP
 * @brief Clock ID ISP.
 */
#define CLOCK_ID_ISP 0x7

/**
 * @def CLOCK_ID_SDRAM
 * @brief Clock ID SDRAM.
 */
#define CLOCK_ID_SDRAM 0x8

/**
 * @def CLOCK_ID_PIXEL
 * @brief Clock ID pixel.
 */
#define CLOCK_ID_PIXEL 0x9

/**
 * @def CLOCK_ID_PWM
 * @brief Clock ID PWM.
 */
#define CLOCK_ID_PWM 0xa

/* unique voltage device IDs */
/* 0x0: reserved */

/**
 * @def VOLTAGE_ID_CORE
 * @brief Voltage ID core.
 */
#define VOLTAGE_ID_CORE 0x1

/**
 * @def VOLTAGE_ID_SDRAM_C
 * @brief Voltage ID SDRAM C.
 */
#define VOLTAGE_ID_SDRAM_C 0x2

/**
 * @def VOLTAGE_ID_SDRAM_P
 * @brief Voltage ID SDRAM P.
 */
#define VOLTAGE_ID_SDRAM_P 0x3

/**
 * @def VOLTAGE_ID_SDRAM_I
 * @brief Voltage ID SDRAM I.
 */
#define VOLTAGE_ID_SDRAM_I 0x4

/* pixel order state */
/**
 * @def PIXEL_ORDER_STATE_BGR
 * @brief Pixel order state BGR.
 */
#define PIXEL_ORDER_STATE_BGR 0x0

/**
 * @def PIXEL_ORDER_STATE_RGB
 * @brief Pixel order state RGB.
 */
#define PIXEL_ORDER_STATE_RGB 0x1


#ifndef __ASSEMBLY__

/**
 * @struct frame_buffer
 * @brief Frame buffer information.
 */
struct frame_buffer {
  /**
   * Width.
   */
  unsigned int width;

  /**
   * Height.
   */
  unsigned int height;

  /**
   * Virtual width.
   */
  unsigned int virtual_width;

  /**
   * Virtual height.
   */
  unsigned int virtual_height;

  /**
   * X offset.
   */
  unsigned int x_offset;

  /**
   * Y offset.
   */
  unsigned int y_offset;

  /**
   * Depth.
   */
  unsigned int depth;

  /**
   * State.
   */
  unsigned int state;

  /**
   * Pointer.
   */
  unsigned int pointer;

  /**
   * Size.
   */
  unsigned int size;

  /**
   * Pitch.
   */
  unsigned int pitch;
};



/**
 * @struct pc_screen_font_info
 * @brief PC screen font information.
 */
struct pc_screen_font_info {
  /**
   * Magic.
   */
  unsigned int magic;

  /**
   * Version.
   */
  unsigned int version;

  /**
   * Header size.
   */
  unsigned int headersize;

  /**
   * Flags.
   */
  unsigned int flags;

  /**
   * Number of glyphs.
   */
  unsigned int numglyph;

  /**
   * Bytes per glyph.
   */
  unsigned int bytesperglyph;

  /**
   * Height.
   */
  unsigned int height;

  /**
   * Width.
   */
  unsigned int width;

  /**
   * Glyphs.
   */
  unsigned char glyphs;
} __packed /** packed. */;

/**
 * @typedef psf_t
 * @brief Typedef of @struct pc_screen_font_info.
 */
typedef struct pc_screen_font_info psf_t;

/**
 * @fn unsigned long get_serial_number(void)
 * @brief get serial number.
 *
 * @return Serial number.
 */
unsigned long get_serial_number(void);

/**
 * @fn void setup_pl011_uart(void)
 * @brief setup PL011 UART.
 */
void setup_pl011_uart(void);

/**
 * @fn void fb_show_picture(char *data, int width, int height)
 * @brief show picture in frame buffer.
 *
 * @param data Data.
 * @param width Width.
 * @param height Height.
 */
void fb_show_picture(char *data, int width, int height);

/**
 * @fn void fb_print(int x, int y, const char *s)
 * @brief print string in frame buffer.
 *
 * @param x X-axis.
 * @param y Y-axis.
 * @param s String.
 */
void fb_print(int x, int y, const char *s);

/**
 * @fn void init_frame_buffer(struct frame_buffer *fb)
 * @brief initialize frame buffer.
 *
 * @param fb Frame buffer.
 */
void init_frame_buffer(struct frame_buffer *fb);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_AARCH64_RASPI3_VIDEOCORE_MAILBOX_H__ */

