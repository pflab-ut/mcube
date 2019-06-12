/**
 * @file include/mcube/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_IRQ_H__
#define __MCUBE_MCUBE_IRQ_H__

#define LOCAL_IRQ_ENABLED 1
#define LOCAL_IRQ_DISABLED 0


#define IRQ_HANDLED 1
#define IRQ_UNHANDLED 0

#define NR_CALLBACKS 8

#ifndef __ASSEMBLY__

static inline void enable_local_irq(void);
static inline void disable_local_irq(void);

/**
 * @union rflags
 * @brief Flags.
 */
union rflags {
  uint64_t raw;
  struct {
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
    uint32_t __reserved0_4;    /* Must be zero */
  } __packed;
};


static inline void save_local_irq(union rflags *flags);
static inline void restore_local_irq(union rflags *flags);


void wait_until_next_interrupt(void);
void init_irq(void);
void exit_irq(void);


struct full_regs;

asmlinkage int do_irq(struct full_regs *regs);

typedef void (*callback_t)(void);

void register_callback_handler(callback_t func, unsigned long id);

extern callback_t Callback[NR_CALLBACKS];


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_IRQ_H__ */
