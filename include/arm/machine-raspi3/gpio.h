/**
 * @file include/arm/machine-raspi3/gpio.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_ARM_MACHINE_RASPI3_GPIO_H__
#define	__MCUBE_ARM_MACHINE_RASPI3_GPIO_H__


#define GPIO_FSEL1   (PERIPHERAL_BASE + 0x00200004)
#define GPIO_SET0    (PERIPHERAL_BASE + 0x0020001c)
#define GPIO_CLR0    (PERIPHERAL_BASE + 0x00200028)
#define GPIO_PUD     (PERIPHERAL_BASE + 0x00200094)
#define GPIO_PUDCLK0 (PERIPHERAL_BASE + 0x00200098)


#endif /*	__MCUBE_ARM_MACHINE_RASPI3_GPIO_H__ */

