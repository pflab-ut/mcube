/**
 * @file include/arm/raspi3/gpio.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_ARM_RASPI3_GPIO_H__
#define	__MCUBE_ARM_RASPI3_GPIO_H__

#define GPFSEL0   (PERIPHERAL_BASE + 0x00200000)
#define GPFSEL1   (PERIPHERAL_BASE + 0x00200004)
#define GPFSEL2   (PERIPHERAL_BASE + 0x00200008)
#define GPFSEL3   (PERIPHERAL_BASE + 0x0020000c)
#define GPFSEL4   (PERIPHERAL_BASE + 0x00200010)
#define GPFSEL5   (PERIPHERAL_BASE + 0x00200014)
/* PERIPHERAL_BASE + 0x00200018: reserved */
#define GPSET0    (PERIPHERAL_BASE + 0x0020001c)
#define GPSET1    (PERIPHERAL_BASE + 0x00200020)
/* PERIPHERAL_BASE + 0x00200024: reserved */
#define GPCLR0    (PERIPHERAL_BASE + 0x00200028)
#define GPCLR1    (PERIPHERAL_BASE + 0x0020002c)
/* PERIPHERAL_BASE + 0x00200030: reserved */
#define GPLEV0    (PERIPHERAL_BASE + 0x00200034)
#define GPLEV1    (PERIPHERAL_BASE + 0x00200038)
/* PERIPHERAL_BASE + 0x0020003c: reserved */
#define GPEDS0    (PERIPHERAL_BASE + 0x00200040)
#define GPEDS1    (PERIPHERAL_BASE + 0x00200044)
/* PERIPHERAL_BASE + 0x00200048: reserved */
#define GPREN0    (PERIPHERAL_BASE + 0x0020004c)
#define GPREN1    (PERIPHERAL_BASE + 0x00200050)
/* PERIPHERAL_BASE + 0x00200054: reserved */
#define GPFEN0    (PERIPHERAL_BASE + 0x00200058)
#define GPFEN1    (PERIPHERAL_BASE + 0x0020005c)
/* PERIPHERAL_BASE + 0x00200060: reserved */
#define GPHEN0    (PERIPHERAL_BASE + 0x00200064)
#define GPHEN1    (PERIPHERAL_BASE + 0x00200068)
/* PERIPHERAL_BASE + 0x0020006c: reserved */
#define GPLEN0    (PERIPHERAL_BASE + 0x00200070)
#define GPLEN1    (PERIPHERAL_BASE + 0x0020007c)
/* PERIPHERAL_BASE + 0x00200078: reserved */
#define GPAREN0   (PERIPHERAL_BASE + 0x0020007c)
#define GPAREN1   (PERIPHERAL_BASE + 0x00200080)
/* PERIPHERAL_BASE + 0x00200084: reserved */
#define GPAFEN0   (PERIPHERAL_BASE + 0x00200088)
#define GPAFEN1   (PERIPHERAL_BASE + 0x0020008c)
/* PERIPHERAL_BASE + 0x00200090: reserved */
#define GPPUD     (PERIPHERAL_BASE + 0x00200094)
#define GPPUDCLK0 (PERIPHERAL_BASE + 0x00200098)
#define GPPUDCLK1 (PERIPHERAL_BASE + 0x0020009c)
/* PERIPHERAL_BASE + 0x002000a0: reserved */
/* PERIPHERAL_BASE + 0x002000b0: test */


/* GPIO Function Select Registers (GPFSELn), where 0 <= n <= 53 */
/* 31-30: reserved */
/* 000 = GPIO Pin n is an input
 * 001 = GPIO Pin n is an output
 * 100 = GPIO Pin n takes alternate function 0
 * 101 = GPIO Pin n takes alternate function 1
 * 110 = GPIO Pin n takes alternate function 2
 * 111 = GPIO Pin n takes alternate function 3
 * 011 = GPIO Pin n takes alternate function 4
 * 010 = GPIO Pin n takes alternate function 5
 */
#define GPIO_PINn_INPUT  0x0
#define GPIO_PINn_OUTPUT 0x1
#define GPIO_PINn_ALT0   0x4
#define GPIO_PINn_ALT1   0x5
#define GPIO_PINn_ALT2   0x6
#define GPIO_PINn_ALT3   0x7
#define GPIO_PINn_ALT4   0x3
#define GPIO_PINn_ALT5   0x2

/* 29-27: FSELn9 - Function Select n9 */
#define GPFSEL_FSELn9_SHIFT 27
#define GPFSEL_FSELn9_MASK (0x7 << GPFSEL_FSELn9_SHIFT)
/* 26-24: FSELn8 - Function Select n8 */
#define GPFSEL_FSELn8_SHIFT 24
#define GPFSEL_FSELn8_MASK (0x7 << GPFSEL_FSELn8_SHIFT)
/* 23-21: FSELn7 - Function Select n7 */
#define GPFSEL_FSELn7_SHIFT 21
#define GPFSEL_FSELn7_MASK (0x7 << GPFSEL_FSELn7_SHIFT)
/* 20-18: FSELn6 - Function Select n6 */
#define GPFSEL_FSELn6_SHIFT 18
#define GPFSEL_FSELn6_MASK (0x7 << GPFSEL_FSELn6_SHIFT)
/* 17-15: FSELn5 - Function Select n5 */
#define GPFSEL_FSELn5_SHIFT 15
#define GPFSEL_FSELn5_MASK (0x7 << GPFSEL_FSELn5_SHIFT)
/* 14-12: FSELn4 - Function Select n4 */
#define GPFSEL_FSELn4_SHIFT 12
#define GPFSEL_FSELn4_MASK (0x7 << GPFSEL_FSELn4_SHIFT)
/* 11-9:  FSELn3 - Function Select n3 */
#define GPFSEL_FSELn3_SHIFT 9
#define GPFSEL_FSELn3_MASK (0x7 << GPFSEL_FSELn3_SHIFT)
/* 8-6:   FSELn2 - Function Select n2 */
#define GPFSEL_FSELn2_SHIFT 6
#define GPFSEL_FSELn2_MASK (0x7 << GPFSEL_FSELn2_SHIFT)
/* 5-3:   FSELn1 - Function Select n1 */
#define GPFSEL_FSELn1_SHIFT 3
#define GPFSEL_FSELn1_MASK (0x7 << GPFSEL_FSELn1_SHIFT)
/* 2-0:   FSELn0 - Function Select n0 */
#define GPFSEL_FSELn0_SHIFT 0
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
#define GPPUD_DISABLE_PULL_UP_DOWN 0x0
#define GPPUD_ENABLE_PULL_DOWN_CTRL 0x1
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
#define GPPUDCLKn_SET(x) (0x1 << ((x) & 0x1f))



#endif /*	__MCUBE_ARM_RASPI3_GPIO_H__ */

