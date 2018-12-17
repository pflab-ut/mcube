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

static inline void save_local_irq(unsigned long *flags);
static inline void restore_local_irq(unsigned long *flags);


void wait_until_next_interrupt(void);
void init_irq(void);

struct full_regs;

asmlinkage int do_irq(struct full_regs *regs);

typedef void (*callback_t)(void);

void register_callback_handler(callback_t func, unsigned long id);

extern callback_t Callback[NR_CALLBACKS];


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_IRQ_H__ */
