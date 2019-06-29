/**
 * @file include/aarch64/raspi3/gpio.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_RASPI3_GPIO_H__
#define __MCUBE_AARCH64_RASPI3_GPIO_H__

/**
 * @def GPFSEL0
 * @brief GPIO function select 0.
 */
#define GPFSEL0 (GPIO_CONTROLLER_BASE + 0x00)

/**
 * @def GPFSEL1
 * @brief GPIO function select 1.
 */
#define GPFSEL1 (GPIO_CONTROLLER_BASE + 0x04)

/**
 * @def GPFSEL2
 * @brief GPIO function select 2.
 */
#define GPFSEL2 (GPIO_CONTROLLER_BASE + 0x08)

/**
 * @def GPFSEL3
 * @brief GPIO function select 3.
 */
#define GPFSEL3 (GPIO_CONTROLLER_BASE + 0x0c)

/**
 * @def GPFSEL4
 * @brief GPIO function select 4.
 */
#define GPFSEL4 (GPIO_CONTROLLER_BASE + 0x10)

/**
 * @def GPFSEL5
 * @brief GPIO function select 5.
 */
#define GPFSEL5 (GPIO_CONTROLLER_BASE + 0x14)

/* GPIO_CONTROLLER_BASE + 0x00200018: reserved */

/**
 * @def GPSET0
 * @brief GPIO pin output set 0.
 */
#define GPSET0 (GPIO_CONTROLLER_BASE + 0x1c)

/**
 * @def GPSET1
 * @brief GPIO pin output set 1.
 */
#define GPSET1 (GPIO_CONTROLLER_BASE + 0x20)

/* GPIO_CONTROLLER_BASE + 0x00200024: reserved */

/**
 * @def GPCLR0
 * @brief GPIO pin output clear 0.
 */
#define GPCLR0 (GPIO_CONTROLLER_BASE + 0x28)

/**
 * @def GPCLR1
 * @brief GPIO pin output clear 1.
 */
#define GPCLR1 (GPIO_CONTROLLER_BASE + 0x2c)

/* GPIO_CONTROLLER_BASE + 0x00200030: reserved */

/**
 * @def GPLEV0
 * @brief GPIO pin level 0.
 */
#define GPLEV0 (GPIO_CONTROLLER_BASE + 0x34)

/**
 * @def GPLEV1
 * @brief GPIO pin level 1.
 */
#define GPLEV1 (GPIO_CONTROLLER_BASE + 0x38)

/* GPIO_CONTROLLER_BASE + 0x0020003c: reserved */

/**
 * @def GPEDS0
 * @brief GPIO pin event detect status 0.
 */
#define GPEDS0 (GPIO_CONTROLLER_BASE + 0x40)

/**
 * @def GPEDS1
 * @brief GPIO pin event detect status 1.
 */
#define GPEDS1 (GPIO_CONTROLLER_BASE + 0x44)

/* GPIO_CONTROLLER_BASE + 0x00200048: reserved */

/**
 * @def GPREN0
 * @brief GPIO pin rising edge detect enable 0.
 */
#define GPREN0 (GPIO_CONTROLLER_BASE + 0x4c)

/**
 * @def GPREN1
 * @brief GPIO pin rising edge detect enable 1.
 */
#define GPREN1 (GPIO_CONTROLLER_BASE + 0x50)

/* GPIO_CONTROLLER_BASE + 0x00200054: reserved */

/**
 * @def GPFEN0
 * @brief GPIO pin falling edge detect enable 0.
 */
#define GPFEN0 (GPIO_CONTROLLER_BASE + 0x58)

/**
 * @def GPFEN1
 * @brief GPIO pin falling edge detect enable 1.
 */
#define GPFEN1 (GPIO_CONTROLLER_BASE + 0x5c)

/* GPIO_CONTROLLER_BASE + 0x00200060: reserved */

/**
 * @def GPHEN0
 * @brief GPIO pin high detect enable 0.
 */
#define GPHEN0 (GPIO_CONTROLLER_BASE + 0x64)

/**
 * @def GPHEN1
 * @brief GPIO pin high detect enable 1.
 */
#define GPHEN1 (GPIO_CONTROLLER_BASE + 0x68)

/* GPIO_CONTROLLER_BASE + 0x0020006c: reserved */

/**
 * @def GPLEN0
 * @brief GPIO pin low detect enable 0.
 */
#define GPLEN0 (GPIO_CONTROLLER_BASE + 0x70)

/**
 * @def GPLEN1
 * @brief GPIO pin low detect enable 1.
 */
#define GPLEN1 (GPIO_CONTROLLER_BASE + 0x74)

/* GPIO_CONTROLLER_BASE + 0x00200078: reserved */

/**
 * @def GPAREN0
 * @brief GPIO pin async. Rising edge detect 0.
 */
#define GPAREN0 (GPIO_CONTROLLER_BASE + 0x7c)

/**
 * @def GPAREN1
 * @brief GPIO pin async. Rising edge detect 1.
 */
#define GPAREN1 (GPIO_CONTROLLER_BASE + 0x80)

/* GPIO_CONTROLLER_BASE + 0x00200084: reserved */

/**
 * @def GPAFEN0
 * @brief GPIO pin async. Falling edge detect 0.
 */
#define GPAFEN0 (GPIO_CONTROLLER_BASE + 0x88)

/**
 * @def GPAFEN1
 * @brief GPIO pin async. Falling edge detect 1.
 */
#define GPAFEN1 (GPIO_CONTROLLER_BASE + 0x8c)

/* GPIO_CONTROLLER_BASE + 0x00200090: reserved */

/**
 * @def GPPUD
 * @brief GPIO pin pull-up/down enable.
 */
#define GPPUD (GPIO_CONTROLLER_BASE + 0x94)

/**
 * @def GPPUDCLK0
 * @brief GPIO pin pull-up/down enable clock 0.
 */
#define GPPUDCLK0 (GPIO_CONTROLLER_BASE + 0x98)

/**
 * @def GPPUDCLK1
 * @brief GPIO pin pull-up/down enable clock 1.
 */
#define GPPUDCLK1 (GPIO_CONTROLLER_BASE + 0x9c)

/* GPIO_CONTROLLER_BASE + 0x002000a0: reserved */
/* GPIO_CONTROLLER_BASE + 0x002000b0: test */


/* GPIO Function Select Registers (GPFSELn), where 0 <= n <= 53 */
/* 31-30: reserved */
/**
 * @def GPIO_PINn_INPUT
 * @brief 000 = GPIO Pin n is an input.
 */
#define GPIO_PINn_INPUT 0x0

/**
 * @def GPIO_PINn_OUTPUT
 * @brief 001 = GPIO Pin n is an output.
 */
#define GPIO_PINn_OUTPUT 0x1

/**
 * @def GPIO_PINn_ALT0
 * @brief 100 = GPIO Pin n takes alternate function 0.
 */
#define GPIO_PINn_ALT0 0x4

/**
 * @def GPIO_PINn_ALT1
 * @brief 101 = GPIO Pin n takes alternate function 1.
 */
#define GPIO_PINn_ALT1 0x5

/**
 * @def GPIO_PINn_ALT2
 * @brief 110 = GPIO Pin n takes alternate function 2.
 */
#define GPIO_PINn_ALT2 0x6

/**
 * @def GPIO_PINn_ALT3
 * @brief 111 = GPIO Pin n takes alternate function 3.
 */
#define GPIO_PINn_ALT3 0x7

/**
 * @def GPIO_PINn_ALT4
 * @brief 011 = GPIO Pin n takes alternate function 4.
 */
#define GPIO_PINn_ALT4 0x3

/**
 * @def GPIO_PINn_ALT5
 * @brief 010 = GPIO Pin n takes alternate function 5.
 */
#define GPIO_PINn_ALT5 0x2

/**
 * @def GPFSEL_FSELn9_SHIFT
 * @brief 29-27: FSELn9 - function select n9 shift.
 */
#define GPFSEL_FSELn9_SHIFT 27

/**
 * @def GPFSEL_FSELn9_MASK
 * @brief 29-27: FSELn9 - function select n9 mask.
 */
#define GPFSEL_FSELn9_MASK (0x7 << GPFSEL_FSELn9_SHIFT)

/**
 * @def GPFSEL_FSELn8_SHIFT
 * @brief 26-24: FSELn8 - Function Select n8 shift.
 */
#define GPFSEL_FSELn8_SHIFT 24

/**
 * @def GPFSEL_FSELn8_MASK
 * @brief 26-24: FSELn8 - Function Select n8 mask.
 */
#define GPFSEL_FSELn8_MASK (0x7 << GPFSEL_FSELn8_SHIFT)

/**
 * @def GPFSEL_FSELn7_SHIFT
 * @brief 23-21: FSELn7 - Function Select n7 shift.
 */
#define GPFSEL_FSELn7_SHIFT 21

/**
 * @def GPFSEL_FSELn7_MASK
 * @brief 23-21: FSELn7 - Function Select n7 mask.
 */
#define GPFSEL_FSELn7_MASK (0x7 << GPFSEL_FSELn7_SHIFT)


/**
 * @def GPFSEL_FSELn6_SHIFT
 * @brief 20-18: FSELn6 - Function Select n6 shift.
 */
#define GPFSEL_FSELn6_SHIFT 18

/**
 * @def GPFSEL_FSELn6_MASK
 * @brief 20-18: FSELn6 - Function Select n6 mask.
 */
#define GPFSEL_FSELn6_MASK (0x7 << GPFSEL_FSELn6_SHIFT)

/**
 * @def GPFSEL_FSELn5_SHIFT
 * @brief 17-15: FSELn5 - Function Select n5 shift.
 */
#define GPFSEL_FSELn5_SHIFT 15

/**
 * @def GPFSEL_FSELn5_MASK
 * @brief 17-15: FSELn5 - Function Select n5 mask.
 */
#define GPFSEL_FSELn5_MASK (0x7 << GPFSEL_FSELn5_SHIFT)

/**
 * @def GPFSEL_FSELn4_SHIFT
 * @brief 14-12: FSELn4 - Function Select n4 shift.
 */
#define GPFSEL_FSELn4_SHIFT 12

/**
 * @def GPFSEL_FSELn4_MASK
 * @brief 14-12: FSELn4 - Function Select n4 mask.
 */
#define GPFSEL_FSELn4_MASK (0x7 << GPFSEL_FSELn4_SHIFT)

/**
 * @def GPFSEL_FSELn3_SHIFT
 * @brief 11-9: FSELn3 - Function Select n3 shift.
 */
#define GPFSEL_FSELn3_SHIFT 9

/**
 * @def GPFSEL_FSELn3_MASK
 * @brief 11-9: FSELn3 - Function Select n3 mask.
 */
#define GPFSEL_FSELn3_MASK (0x7 << GPFSEL_FSELn3_SHIFT)

/**
 * @def GPFSEL_FSELn2_SHIFT
 * @brief 8-6: FSELn2 - Function Select n2 shift.
 */
#define GPFSEL_FSELn2_SHIFT 6

/**
 * @def GPFSEL_FSELn2_MASK
 * @brief 8-6: FSELn2 - Function Select n2 mask.
 */
#define GPFSEL_FSELn2_MASK (0x7 << GPFSEL_FSELn2_SHIFT)

/**
 * @def GPFSEL_FSELn1_SHIFT
 * @brief 5-3: FSELn1 - Function Select n1 shift.
 */
#define GPFSEL_FSELn1_SHIFT 3

/**
 * @def GPFSEL_FSELn1_MASK
 * @brief 5-3: FSELn1 - Function Select n1 mask.
 */
#define GPFSEL_FSELn1_MASK (0x7 << GPFSEL_FSELn1_SHIFT)

/**
 * @def GPFSEL_FSELn0_SHIFT
 * @brief 2-0: FSELn0 - Function Select n0 shift.
 */
#define GPFSEL_FSELn0_SHIFT 0

/**
 * @def GPFSEL_FSELn0_MASK
 * @brief 2-0: FSELn0 - Function Select n0 mask.
 */
#define GPFSEL_FSELn0_MASK (0x7 << GPFSEL_FSELn0_SHIFT)

/* GPIO Pin Output Set Registers (GPSETn) */
/* GPIO Pin Output Set Register 0
 * 31-0: SETn (n=0...31)
 * 0 = No effect
 * 1 = Set GPIO pin n
 */
/* GPIO Pin Output Set Register 1
 * 31-22: reserved
 * 21-0: SETn (n=32...53)
 * 0 = No effect
 * 1 = Set GPIO pin n.
 */
/**
 * @def GPSETn_SET(x)
 * @brief set GPIO pin output set registers.
 */
#define GPSETn_SET(x) (0x1 << ((x) & 0x1f))

/* GPIO Pin Output Clear Registers (GPCLRn) */
/* GPIO Pin Output Clear Register 0
 * 31-0: CLRn (n=0...31)
 * 0 = No effect
 * 1 = Set GPIO pin n
 */
/* GPIO Pin Output Clear Register 1
 * 31-22: reserved
 * 21-0: CLRn (n=32...53)
 * 0 = No effect
 * 1 = Set GPIO pin n.
 */
/**
 * @def GPCLRn_CLEAR(x)
 * @brief set GPIO pin output clear register.
 */
#define GPCLRn_CLEAR(x) (0x1 << ((x) & 0x1f))

/* GPIO Pin Level Registers (GPLEVn) */
/* GPIO Pin Level Register 0
 * 31-0: LEVn (n=0...31)
 * 0 = No effect
 * 1 = Set GPIO pin n
 */
/* GPIO Pin Level Register 1
 * 31-22: reserved
 * 21-0: LEVn (n=32...53)
 * 0 = No effect
 * 1 = Set GPIO pin n.
 */
/**
 * @def GPLEVn_HIGH(x)
 * @brief set GPIO pin level registers.
 */
#define GPLEVn_HIGH(x) (0x1 << ((x) & 0x1f))

/* GPIO Event Detect Status Registers (GPEDSn) */
/* GPIO Event Detect Register 0
 * 31-0: EDSn (n=0...31)
 * 0 = Event not detected on GPIO pin n
 * 1 = Event detected on GPIO pin n
 */
/* GPIO Event Detect Register 1
 * 31-22: reserved
 * 21-0: LEVn (n=32...53)
 * 0 = Event not detected on GPIO pin n
 * 1 = Event detected on GPIO pin n
 */
/**
 * @def GPEDSn_EVENT_DETECT(x)
 * @brief set GPIO event detect registers.
 */
#define GPEDSn_EVENT_DETECT(x) (0x1 << ((x) & 0x1f))

/* GPIO Rising Edge Detect Enable Registers (GPRENn) */
/* GPIO Rising Edge Detect Enable Register 0
 * 31-0: RENn (n=0...31)
 * 0 = Rising edge detect disabled on GPIO pin n.
 * 1 = Rising edge on GPIO pin n sets corresponding bit in EDSn.
 */
/* GPIO Rising Edge Detect Enable Register 1
 * 31-22: reserved
 * 21-0: RENn (n=32...53)
 * 0 = Rising edge detect disabled on GPIO pin n.
 * 1 = Rising edge on GPIO pin n sets corresponding bit in EDSn.
 */
/**
 * @def GPRENn_SET(x)
 * @brief set GPIO rising edge detect enable registers.
 */
#define GPRENn_SET(x) (0x1 << ((x) & 0x1f))

/* GPIO Falling Edge Detect Enable Registers (GPRENn) */
/* GPIO Falling Edge Detect Enable Register 0
 * 31-0: FENn (n=0...31)
 * 0 = Falling edge detect disabled on GPIO pin n.
 * 1 = Falling edge on GPIO pin n sets corresponding bit in EDSn.
 */
/* GPIO Falling Edge Detect Enable Register 1
 * 31-22: reserved
 * 21-0: FENn (n=32...53)
 * 0 = Falling edge detect disabled on GPIO pin n.
 * 1 = Falling edge on GPIO pin n sets corresponding bit in EDSn.
 */
/**
 * @def GPFENn_SET(x)
 * @brief set GPIO falling edge detect enable registers.
 */
#define GPFENn_SET(x) (0x1 << ((x) & 0x1f))

/* GPIO High Detect Enable Registers (GPHENn) */
/* GPIO High Detect Enable Register 0
 * 31-0: HENn (n=0...31)
 * 0 = High detect disabled on GPIO pin n.
 * 1 = High on GPIO pin n sets corresponding bit in GPEDS.
 */
/* GPIO High Detect Enable Register 1
 * 31-22: reserved
 * 21-0: HENn (n=32...53)
 * 0 = High detect disabled on GPIO pin n.
 * 1 = High on GPIO pin n sets corresponding bit in GPEDS.
 */
/**
 * @def GPHENn_SET(x)
 * @brief set GPIO high detect enable registers.
 */
#define GPHENn_SET(x) (0x1 << ((x) & 0x1f))

/* GPIO Low Detect Enable Registers (GPLENn) */
/* GPIO Low Detect Enable Register 0
 * 31-0: LENn (n=0...31)
 * 0 = Low detect disabled on GPIO pin n.
 * 1 = Low on GPIO pin n sets corresponding bit in GPEDS.
 */
/* GPIO Low Detect Enable Register 1
 * 31-22: reserved
 * 21-0: LENn (n=32...53)
 * 0 = Low detect disabled on GPIO pin n.
 * 1 = Low on GPIO pin n sets corresponding bit in GPEDS.
 */
/**
 * @def GPLENn_SET(x)
 * @brief set GPIO low detect enable registers.
 */
#define GPLENn_SET(x) (0x1 << ((x) & 0x1f))

/* GPIO Asynchronous rising Edge Detect Enable Registers (GPARENn) */
/* GPIO Asynchronous rising Edge Detect Enable Register 0
 * 31-0: ARENn (n=0...31)
 * 0 = Asynchronous rising edge detect disabled on GPIO pin n.
 * 1 = Asynchronous rising edge on GPIO pin n sets corresponding bit in EDSn.
 */
/* GPIO Asynchronous rising Edge Detect Enable Register 1
 * 31-22: reserved
 * 21-0: ARENn (n=32...53)
 * 0 = Asynchronous rising edge detect disabled on GPIO pin n.
 * 1 = Asynchronous rising edge on GPIO pin n sets corresponding bit in EDSn.
 */
/**
 * @def GPARENn_SET(x)
 * @brief set GPIO asynchronous rising edge detect enable registers.
 */
#define GPARENn_SET(x) (0x1 << ((x) & 0x1f))

/* GPIO Asynchronous Falling Edge Detect Enable Registers (GPAFENn) */
/* GPIO Asynchronous Falling Edge Detect Enable Register 0
 * 31-0: AFENn (n=0...31)
 * 0 = Asynchronous falling edge detect disabled on GPIO pin n.
 * 1 = Asynchronous falling edge on GPIO pin n sets corresponding bit in EDSn.
 */
/* GPIO Asynchronous Falling Edge Detect Enable Register 1
 * 31-22: reserved
 * 21-0: AFENn (n=32...53)
 * 0 = Asynchronous falling edge detect disabled on GPIO pin n.
 * 1 = Asynchronous falling edge on GPIO pin n sets corresponding bit in EDSn.
 */
/**
 * @def GPAFENn_SET(x)
 * @brief set GPIO asynchronous falling edge detect enable registers.
 */
#define GPAFENn_SET(x) (0x1 << ((x) & 0x1f))

/* GPIO Pull-up/down Register (GPPUD)
 * 31-2: reserved
 * 1-0: PUD - GPIO Pin Pull-up/down
 * 00 = Off - disable pull-up/down
 * 01 = Enable Pull Down control
 * 10 = Enable Pull Up control
 * 11 = Reserved
 * Use in conjunction with GPPUDCLK0/1/2
 */
/**
 * @def GPPUD_DISABLE_PULL_UP_DOWN
 * @brief 00 = Off - disable pull-up/down.
 */
#define GPPUD_DISABLE_PULL_UP_DOWN 0x0

/**
 * @def GPPUD_ENABLE_PULL_DOWN_CTRL
 * @brief 01 = Enable Pull Down control.
 */
#define GPPUD_ENABLE_PULL_DOWN_CTRL 0x1

/**
 * @def GPPUD_ENABLE_PULL_UP_CTRL
 * @brief 10 = Enable Pull Up control.
 */
#define GPPUD_ENABLE_PULL_UP_CTRL 0x2

/* GPIO Pull-up/down Clock Registers (GPPUDCLKn) */
/* GPIO Pull-up/down Clock Register 0
 * 31-0: PUDCLKn (n=0...31)
 * 0 = No Effect
 * 1 = Assert Clock on line (n)
 * Use in conjunction with GPPUD
 */
/* GPIO Pull-up/down Clock Register 1
 * 31-22: reserved
 * 21-0: PUDCLKn (n=32...53)
 * 0 = No Effect
 * 1 = Assert Clock on line (n)
 * Use in conjunction with GPPUD
 */
/**
 * @def GPPUDCLKn_SET(x)
 * @brief set GPIO pull-up/down clock registers.
 */
#define GPPUDCLKn_SET(x) (0x1 << ((x) & 0x1f))



#endif /* __MCUBE_AARCH64_RASPI3_GPIO_H__ */

