/**
 * @file include/axis/irqflags.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_IRQFLAGS_H__
#define __MCUBE_AXIS_IRQFLAGS_H__

#define DIVIDE_ERROR_IRQ 0
#define DEBUG_IRQ 1
#define NMI_INT_IRQ 2
#define BREAKPOINT_IRQ 3
#define OVERFLOW_IRQ 4
#define BOUND_RANGE_EXCEEDED_IRQ 5
#define INVALID_CODE_IRQ 6
#define DEVICE_NOT_AVAILABLE_IRQ 7
#define DOUBLE_FAULT_IRQ 8
#define COPROCESSOR_SEGMENT_OVERRUN_IRQ 9
#define INVALID_TSS_IRQ 10
#define SEGMENT_NOT_PRESENT_IRQ 11
#define STACK_SEGMENT_FAULT_IRQ 12
#define GENERAL_PROTECTION_IRQ 13
#define PAGE_FAULT_IRQ 14
/* 15: reserved */
#define FLOATING_POINT_ERROR_IRQ 16
#define ALIGNMENT_CHECK_IRQ 17
#define MACHINE_CHECK_IRQ 18
#define SIMD_FLOATING_POINT_EXCEPTION_IRQ 19
/* 20-31: reserved */
/* 32-255: maskable interrupts */
#define SCHED_IRQ 40

#define NR_IRQS 8


#ifndef __ASSEMBLY__

static inline void enable_local_irq(void)
{
}

static inline void disable_local_irq(void)
{
}

static inline void save_local_irq64(uint64_t *flags)
{
}

static inline void restore_local_irq64(uint64_t *flags)
{
}


static inline void save_local_irq(uint32_t *flags)
{
  disable_interrupt();
}


static inline void restore_local_irq(uint32_t *flags)
{
	enable_interrupt();
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_AXIS_IRQFLAGS_H__ */
