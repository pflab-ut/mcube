/**
 * @file include/axis/addr.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_ADDR_H__
#define __MCUBE_AXIS_ADDR_H__

/** Scratch Pad Memory (SPM) base address. */
#define MEMORY_BASE  0x00000000
/** System register base address. */
#define SYS_BASE  0x40000000
/** Direct Memory Access Controller (DMAC) base address. */
#define DMAC_BASE 0x60000000
/** Network base address. */
#define NET_BASE  0x80000000

/* NOTE: SPM_SIZE is defined by "configure". */


/** System register size. */
#define SYS_SIZE  0x20000000
/** DMAC size. */
#define DMAC_SIZE 0x20000000
/** Network size. */
#define NET_SIZE  0x80000000


/** Bank size. */
#define BANK_SIZE 0x00000800 /* 2KB (maybe) */

/** Memory offset per CPU. */
#define MEMORY_OFFSET_PER_CPU (MEMORY_SIZE / NR_INTER_KERNEL_CPUS)

#define THREAD_STACK_ADDR(cpu, id) (MEMORY_BASE + MEMORY_SIZE - (cpu) * MEMORY_OFFSET_PER_CPU - (id) * STACK_SIZE)


#endif /* __MCUBE_AXIS_ADDR_H__ */
