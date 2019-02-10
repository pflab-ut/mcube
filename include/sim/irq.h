/**
 * @file include/sim/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_IRQ_H__
#define __MCUBE_SIM_IRQ_H__

#ifndef __ASSEMBLY__


static inline void enable_local_irq(void)
{
}

static inline void disable_local_irq(void)
{
}

static inline int is_irq_enabled(__unused unsigned long flags)
{
  return 1;
}

static inline void save_local_irq(__unused unsigned long *flags)
{
}


static inline void restore_local_irq(__unused unsigned long *flags)
{
}



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_IRQ_H__ */
