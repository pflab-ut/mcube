/**
 * @file include/axis/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_IRQ_H__
#define __MCUBE_AXIS_IRQ_H__


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
  unsigned long data;
  asm volatile("mfs %0, $0" : "=r"(data));
  data |= (0x1 << 0);
  asm volatile("mts $0, %0" :: "r"(data));
}

static inline void disable_local_irq(void)
{
  unsigned long data;
  asm volatile("mfs %0, $0" : "=r"(data));
  data &= ~(0x1 << 0);
  asm volatile("mts $0, %0" :: "r"(data));
}

static inline void enable_previous_irq(void)
{
  unsigned long data;
  asm volatile("mfs %0, $0" : "=r"(data));
  data |= (0x1 << 1);
  asm volatile("mts $0, %0" :: "r"(data));
}

static inline void disable_previous_irq(void)
{
  unsigned long data;
  asm volatile("mfs %0, $0" : "=r"(data));
  data &= ~(0x1 << 1);
  asm volatile("mts $0, %0" :: "r"(data));
}

static inline int is_irq_enabled(__unused unsigned long flags)
{
  unsigned long data;
  asm volatile("mfs %0, $0" : "=r"(data));
  return data & 0x1;
}

static inline void save_local_irq(unsigned long *flags)
{
  asm volatile("mfs %0, $0" : "=r"(*flags));

  if (is_irq_enabled(*flags)) {
    disable_local_irq();
  }
}

static inline void restore_local_irq(unsigned long *flags)
{
  asm volatile("mts $0, %0" :: "r"(*flags));

  if (is_irq_enabled(*flags)) {
    enable_local_irq();
  }
}

void common_interrupt_handler(void);
void do_sched_by_software_interrupt(void);
void interrupt_vector(void);

#endif /* !__ASSEMBLY__ */




#endif /* __MCUBE_AXIS_IRQ_H__ */

