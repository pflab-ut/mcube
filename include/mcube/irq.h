/**
 * @file include/mcube/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_IRQ_H__
#define __MCUBE_MCUBE_IRQ_H__

#ifndef __ASSEMBLY__

void wait_until_next_interrupt(void);

void init_irq(void);
static void enable_interrupt(void);
static void disable_interrupt(void);


asmlinkage int do_irq(unsigned long irq, struct full_regs *regs);
unsigned int __do_irq(unsigned long irq);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_IRQ_H__ */
