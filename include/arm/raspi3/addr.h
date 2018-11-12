/**
 * @file include/arm/raspi3/addr.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_ARM_RASPI3_ADDR_H__
#define	__MCUBE_ARM_RASPI3_ADDR_H__


#define RAM_BASE 0x00000000
#define GPU_BASE 0x37000000
#define PERIPHERAL_BASE 0x3f000000
#define SYSTEM_TIMER_BASE 0x3f003000 /* NOTE: qemu does not support system timer */
#define DMAC_BASE 0x3f007000
#define IRQ_BASIC_BASE 0x3f00b200
#define SP804_TIMER_BASE 0x3f00b400
#define LOCAL_PERIPHERAL_BASE 0x40000000


#define RAM_SIZE 0x37000000
#define GPU_SIZE 0x08000000


#define STACK_ADDR 0x00800000


#endif /* __MCUBE_ARM_RASPI3_ADDR_H__ */

