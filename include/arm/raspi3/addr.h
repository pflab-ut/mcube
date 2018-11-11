/**
 * @file include/arm/raspi3/addr.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_ARM_RASPI3_ADDR_H__
#define	__MCUBE_ARM_RASPI3_ADDR_H__

#define STACK_ADDR 0x00800000

#define PERIPHERAL_BASE 0x3f000000

/* NOTE: qemu does not support system timer */
#define SYSTEM_TIMER_BASE 0x3f003000

#define IRQ_BASIC_BASE 0x3f00b200

#define SP804_TIMER_BASE 0x3f00b400

#define GENERIC_TIMER_BASE 0x40000000


#endif /* __MCUBE_ARM_RASPI3_ADDR_H__ */

