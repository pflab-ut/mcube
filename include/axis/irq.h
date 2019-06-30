/**
 * @file include/axis/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_IRQ_H__
#define __MCUBE_AXIS_IRQ_H__

/**
 * @def DIVIDE_ERROR_IRQ
 * @brief Divide error IRQ.
 */
#define DIVIDE_ERROR_IRQ 0

/**
 * @def DEBUG_IRQ
 * @brief Debug IRQ.
 */
#define DEBUG_IRQ 1

/**
 * @def NMI_INT_IRQ
 * @brief Non-maskable interrupt IRQ.
 */
#define NMI_INT_IRQ 2

/**
 * @def BREAKPOINT_IRQ
 * @brief Breakpoint IRQ.
 */
#define BREAKPOINT_IRQ 3

/**
 * @def OVERFLOW_IRQ
 * @brief Overflow IRQ.
 */
#define OVERFLOW_IRQ 4

/**
 * @def BOUND_RANGE_EXCEEDED_IRQ
 * @brief Bound range exceeded IRQ.
 */
#define BOUND_RANGE_EXCEEDED_IRQ 5

/**
 * @def INVALID_CODE_IRQ
 * @brief Invalid code IRQ.
 */
#define INVALID_CODE_IRQ 6

/**
 * @def DEVICE_NOT_AVAILABLE_IRQ
 * @brief Device not available IRQ.
 */
#define DEVICE_NOT_AVAILABLE_IRQ 7

/**
 * @def DOUBLE_FAULT_IRQ
 * @brief Double fault IRQ.
 */
#define DOUBLE_FAULT_IRQ 8

/**
 * @def COPROCESSOR_SEGMENT_OVERRUN_IRQ
 * @brief Coprocessor segment overrun IRQ.
 */
#define COPROCESSOR_SEGMENT_OVERRUN_IRQ 9

/**
 * @def INVALID_TSS_IRQ
 * @brief Invalid TSS IRQ.
 */
#define INVALID_TSS_IRQ 10

/**
 * @def SEGMENT_NOT_PRESENT_IRQ
 * @brief Segment not present IRQ.
 */
#define SEGMENT_NOT_PRESENT_IRQ 11

/**
 * @def STACK_SEGMENT_FAULT_IRQ
 * @brief Stack segment fault IRQ.
 */
#define STACK_SEGMENT_FAULT_IRQ 12

/**
 * @def GENERAL_PROTECTION_IRQ
 * @brief General protection IRQ.
 */
#define GENERAL_PROTECTION_IRQ 13

/**
 * @def PAGE_FAULT_IRQ
 * @brief Page fault IRQ.
 */
#define PAGE_FAULT_IRQ 14

/* 15: reserved */

/**
 * @def FLOATING_POINT_ERROR_IRQ
 * @brief Floating point error IRQ.
 */
#define FLOATING_POINT_ERROR_IRQ 16

/**
 * @def ALIGNMENT_CHECK_IRQ
 * @brief Alignment check IRQ.
 */
#define ALIGNMENT_CHECK_IRQ 17

/**
 * @def MACHINE_CHECK_IRQ
 * @brief Machine check IRQ.
 */
#define MACHINE_CHECK_IRQ 18

/**
 * @def SIMD_FLOATING_POINT_EXCEPTION_IRQ
 * @brief SIMD floating point exception IRQ.
 */
#define SIMD_FLOATING_POINT_EXCEPTION_IRQ 19

/* 20-31: reserved */
/* 32-255: maskable interrupts */

/**
 * @def NR_IRQS
 * @brief Number of IRQs.
 */
#define NR_IRQS 8

#ifndef __ASSEMBLY__

/**
 * @fn static inline void enable_local_irq(void)
 * @brief enable local IRQ.
 */
static inline void enable_local_irq(void)
{
  unsigned long data;
  asm volatile("mfs %0, $0" : "=r"(data));
  data |= (0x1 << 0);
  asm volatile("mts $0, %0" :: "r"(data));
}

/**
 * @fn static inline void disable_local_irq(void)
 * @brief disable local IRQ.
 */
static inline void disable_local_irq(void)
{
  unsigned long data;
  asm volatile("mfs %0, $0" : "=r"(data));
  data &= ~(0x1 << 0);
  asm volatile("mts $0, %0" :: "r"(data));
}

/**
 * @fn static inline void enable_previous_irq(void)
 * @brief enable previous IRQ.
 */
static inline void enable_previous_irq(void)
{
  unsigned long data;
  asm volatile("mfs %0, $0" : "=r"(data));
  data |= (0x1 << 1);
  asm volatile("mts $0, %0" :: "r"(data));
}

/**
 * @fn static inline void disable_previous_irq(void)
 * @brief disable previous IRQ.
 */
static inline void disable_previous_irq(void)
{
  unsigned long data;
  asm volatile("mfs %0, $0" : "=r"(data));
  data &= ~(0x1 << 1);
  asm volatile("mts $0, %0" :: "r"(data));
}

/**
 * @fn static inline bool is_irq_enabled(__unused unsigned long flags)
 * @brief Is IRQ enabled?
 *
 * @param flags Flags.
 */
static inline bool is_irq_enabled(__unused unsigned long flags)
{
  unsigned long data;
  asm volatile("mfs %0, $0" : "=r"(data));
  return data & 0x1;
}

/**
 * @fn static inline void save_local_irq(union rflags *flags)
 * @brief save local IRQ.
 *
 * @param flags Flags.
 */
static inline void save_local_irq(union rflags *flags)
{
  asm volatile("mfs %0, $0" : "=r"(*flags));

  if (is_irq_enabled(flags->raw)) {
    disable_local_irq();
  }
}

/**
 * @fn static inline void restore_local_irq(union rflags *flags)
 * @brief restore local IRQ.
 *
 * @param flags Flags.
 */
static inline void restore_local_irq(union rflags *flags)
{
  asm volatile("mts $0, %0" :: "r"(*flags));

  if (is_irq_enabled(flags->raw)) {
    enable_local_irq();
  }
}

/**
 * @fn void common_interrupt_handler(void)
 * @brief Common interrupt handler.
 */
void common_interrupt_handler(void);

/**
 * @fn void do_sched_by_software_interrupt(void)
 * @brief do scheduling by software interrupt.
 */
void do_sched_by_software_interrupt(void);

/**
 * @fn void interrupt_vector(void)
 * @brief Interrupt vector.
 */
void interrupt_vector(void);

#endif /* !__ASSEMBLY__ */




#endif /* __MCUBE_AXIS_IRQ_H__ */

