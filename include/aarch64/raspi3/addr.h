/**
 * @file include/aarch64/raspi3/addr.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_RASPI3_ADDR_H__
#define __MCUBE_AARCH64_RASPI3_ADDR_H__

/**
 * @def RAM_BASE
 * @brief RAM base.
 */
#define RAM_BASE 0x00000000

/**
 * @def GPU_BASE
 * @brief GPU base.
 */
#define GPU_BASE 0x37000000

/**
 * @def PERIPHERAL_BASE
 * @brief Peripheral base.
 */
#define PERIPHERAL_BASE 0x3f000000

/* NOTE: QEMU does not support system timer */

/**
 * @def SYSTEM_TIMER_BASE
 * @brief System timer base.
 */
#define SYSTEM_TIMER_BASE 0x3f003000

/**
 * @def DMAC_BASE
 * @brief DMAC base.
 */
#define DMAC_BASE 0x3f007000

/**
 * @def INTERRUPT_CONTROLLER_BASE
 * @brief Interrupt controller base.
 */
#define INTERRUPT_CONTROLLER_BASE 0x3f00b000

/**
 * @def IRQ_BASIC_BASE
 * @brief IRQ basic base.
 */
#define IRQ_BASIC_BASE 0x3f00b200

/**
 * @def SP804_TIMER_BASE
 * @brief SP804 timer base.
 */
#define SP804_TIMER_BASE 0x3f00b400

/**
 * @def VIDEOCORE_MAILBOX_BASE
 * @brief Videocore mailbox base.
 */
#define VIDEOCORE_MAILBOX_BASE 0x3f00b880

/**
 * @def POWER_MANAGEMENT_BASE
 * @brief Power management base.
 */
#define POWER_MANAGEMENT_BASE 0x3f100000

/**
 * @def RANDOM_NUMBER_GEN_BASE
 * @brife Random number generator base.
 */
#define RANDOM_NUMBER_GEN_BASE 0x3f104000

/**
 * @def GPIO_CONTROLLER_BASE
 * @brief GPIO controller base.
 */
#define GPIO_CONTROLLER_BASE 0x3f200000

/* UART0 (serial port, PL011) */
/* for raspi2 & 3, 0x20201000 for raspi1 */

/**
 * @def UART0_BASE
 * @brief UART0 base.
 */
#define UART0_BASE 0x3f201000

/* UART1 (serial port, AUX mini UART) */
/**
 * @def UART1_BASE
 * @brief UART1 base.
 */
#define UART1_BASE 0x3f215000

/**
 * @def EMMC_BASE
 * @brief EMMC base.
 */
#define EMMC_BASE 0x3f300000

/**
 * @def USB_BASE
 * @brief USB base.
 */
#define USB_BASE 0x3f980000

/**
 * @def LOCAL_PERIPHERAL_BASE
 * @brief Local peripheral base.
 */
#define LOCAL_PERIPHERAL_BASE 0x40000000

/**
 * @def RAM_SIZE
 * @brief RAM size.
 */
#define RAM_SIZE 0x37000000

/**
 * @def GPU_SIZE
 * @brief GPU size.
 */
#define GPU_SIZE 0x08000000

/**
 * @def STACK_ADDR
 * @brief Stack address.
 */
#define STACK_ADDR 0x00800000


#endif /* __MCUBE_AARCH64_RASPI3_ADDR_H__ */

