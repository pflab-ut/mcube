/**
 * @file include/arm/raspi3/addr.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_RASPI3_ADDR_H__
#define __MCUBE_ARM_RASPI3_ADDR_H__


#define RAM_BASE                  0x00000000
#define GPU_BASE                  0x37000000
#define PERIPHERAL_BASE           0x3f000000
/* NOTE: QEMU does not support system timer */
#define SYSTEM_TIMER_BASE         0x3f003000
#define DMAC_BASE                 0x3f007000
#define INTERRUPT_CONTROLLER_BASE 0x3f00b000
#define IRQ_BASIC_BASE            0x3f00b200
#define SP804_TIMER_BASE          0x3f00b400
#define VIDEOCORE_MAILBOX_BASE    0x3f00b880
#define POWER_MANAGEMENT_BASE     0x3f100000
#define RANDOM_NUMBER_GEN_BASE    0x3f104000
#define GPIO_CONTROLLER_BASE      0x3f200000
/* UART0 (serial port, PL011) */
/* for raspi2 & 3, 0x20201000 for raspi1 */
#define UART0_BASE                0x3f201000
/* UART1 (serial port, AUX mini UART) */
#define UART1_BASE                0x3f215000
#define EMMC_BASE                 0x3f300000
#define USB_BASE                  0x3f980000
#define LOCAL_PERIPHERAL_BASE     0x40000000


#define RAM_SIZE 0x37000000
#define GPU_SIZE 0x08000000


#define STACK_ADDR 0x00800000


#endif /* __MCUBE_ARM_RASPI3_ADDR_H__ */

