/**
 * @file include/axis/addr.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_ADDR_H__
#define __MCUBE_AXIS_ADDR_H__

/**
 * @def MEMORY_BASE
 * @brief Scratch Pad Memory (SPM) base address.
 */
#define MEMORY_BASE 0x00000000

/**
 * @def SYS_BASE
 * @brief System register base address.
 */
#define SYS_BASE 0x40000000

/**
 * @def DMAC_BASE
 * @brief Direct Memory Access Controller (DMAC) base address.
 */
#define DMAC_BASE 0x60000000

/**
 * @def NET_BASE
 * @brief Network base address.
 */
#define NET_BASE 0x80000000

/**
 * @def SYS_SIZE
 * @brief System register size.
 */
#define SYS_SIZE 0x20000000

/**
 * @def DMAC_SIZE
 * @brief DMAC size.
 */
#define DMAC_SIZE 0x20000000

/**
 * @def NET_SIZE
 * @brief Network size.
 */
#define NET_SIZE 0x80000000

/**
 * @def BANK_SIZE
 * @brief Bank size.
 */
#define BANK_SIZE 0x00000800 /* 2KB (maybe) */


#endif /* __MCUBE_AXIS_ADDR_H__ */
