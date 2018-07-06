/**
 * @file include/sim/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_IRQ_H__
#define __MCUBE_SIM_IRQ_H__

#ifndef __ASSEMBLY__

#define NR_IRQS 1

extern struct irq_descriptor irq_desc[NR_IRQS];

/* dummy */
#define SCHED_IRQ 0


static inline void enable_interrupt(void)
{
}

static inline void disable_interrupt(void)
{
}



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_IRQ_H__ */
