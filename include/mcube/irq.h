/**
 * @file include/mcube/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_IRQ_H__
#define __MCUBE_MCUBE_IRQ_H__

/**
 * @def IRQ_HANDLED
 * @brief handle IRQ.
 */
#define IRQ_HANDLED 1

/**
 * @def IRQ_UNHANDLED
 * @brief unhandle IRQ.
 */
#define IRQ_UNHANDLED 0

/**
 * @def NR_CALLBACKS
 * @brief Number of callbacks.
 */
#define NR_CALLBACKS 8

#ifndef __ASSEMBLY__

/**
 * @fn static inline void enable_local_irq(void)
 * @brief enable local IRQ.
 */
static inline void enable_local_irq(void);

/**
 * @fn static inline void disable_local_irq(void)
 * @brief disable local IRQ.
 */
static inline void disable_local_irq(void);

/**
 * @union rflags
 * @brief Flags.
 */
union rflags {
  /**
   * Raw data.
   */
  uint64_t raw;
  struct {
    /**
     * Flags.
     */
    uint32_t carry_flag: 1,   /* Last math op resulted carry */
             __reserved1_0: 1, /* Always 1 */
             parity_flag: 1,   /* Last op resulted even parity */
             __reserved0_0: 1, /* Must be zero */
             auxiliary_flag: 1, /* BCD carry */
             __reserved0_1: 1, /* Must be zero */
             zero_flag: 1,   /* Last op resulted zero */
             sign_flag: 1,   /* Last op resulted negative */
             trap_flag: 1,   /* Enable single-step mode */
             irqs_enabled: 1,   /* Maskable interrupts enabled? */
             direction_flag: 1, /* String ops direction */
             overflow_flag: 1, /* Last op MSB overflowed */
             io_privilege: 2,   /* IOPL of current task */
             nested_task: 1,   /* Controls chaining of tasks */
             __reserved0_2: 1, /* Must be zero */
             resume_flag: 1,   /* Debug exceptions related */
             virtual_8086: 1,   /* Enable/disable 8086 mode */
             alignment_check: 1, /* Enable alignment checking? */
             virtual: 2,   /* Virtualization fields */
             id_flag: 1,   /* CPUID supported if writable */
             __reserved0_3: 10; /* Must be zero */

    /**
     * Must be zero.
     */
    uint32_t __reserved0_4;
  } __packed /** packed. */;
};

/**
 * @fn static inline void save_local_irq(union rflags *flags)
 * @brief save local irq flags.
 *
 * @param flags Flags.
 */
static inline void save_local_irq(union rflags *flags);

/**
 * @fn static inline void restore_local_irq(union rflags *flags)
 * @brief restore local irq flags.
 *
 * @param flags Flags.
 */
static inline void restore_local_irq(union rflags *flags);

/**
 * @fn void wait_until_next_interrupt(void)
 * @brief wait until next interrupt.
 */
void wait_until_next_interrupt(void);

/**
 * @fn void init_irq(void)
 * @brief initialize IRQ.
 */
void init_irq(void);

/**
 * @fn void exit_irq(void)
 * @brief exit IRQ.
 */
void exit_irq(void);

struct full_regs;

/**
 * @fn asmlinkage int do_irq(struct full_regs *regs)
 * @brief do IRQ.
 *
 * @param regs Full registers.
 * @return IRQ_HANDLED if success, and otherwise if failure.
 */
asmlinkage int do_irq(struct full_regs *regs);

/**
 * @typedef void (*callback_t)(void)
 * @brief Typedef of callback function.
 */
typedef void (*callback_t)(void);

/**
 * @fn void register_callback_handler(callback_t func, unsigned long id)
 * @brief register callback handler.
 *
 * @param func Callback function.
 * @param id Callback ID.
 */
void register_callback_handler(callback_t func, unsigned long id);

/**
 * @var Callback[NR_CALLBACKS]
 * @brief Pointer to callback functions.
 */
extern callback_t Callback[NR_CALLBACKS];

/**
 * @fn int handle_uart_interrupt(void)
 * @brief handle UART interrupt.
 *
 * @return @c IRQ_HANDLED if success.
 */
int handle_uart_interrupt(void);

/**
 * @fn int handle_dmac_interrupt(void)
 * @brief handle DMAC interrupt.
 *
 * @return @c IRQ_HANDLED if success.
 */
int handle_dmac_interrupt(void);

/**
 * @fn int handle_timer_interrupt(void)
 * @brief handle timer interrupt.
 *
 * @return @c IRQ_HANDLED if success.
 */
int handle_timer_interrupt(void);

/**
 * @fn int handle_software_interrupt(unsigned long id)
 * @brief handle software interrupt.
 *
 * @param id ID.
 * @return @c IRQ_HANDLED if success.
 */
int handle_software_interrupt(unsigned long id);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_IRQ_H__ */
