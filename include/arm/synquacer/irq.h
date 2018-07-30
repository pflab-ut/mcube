/**
 * @file include/arm/synquacer/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_ARM_SYNQUACER_IRQ_H__
#define	__MCUBE_ARM_SYNQUACER_IRQ_H__


#define SYSTEM_TIMER_IRQ0	(1 << 0)
#define SYSTEM_TIMER_IRQ1	(1 << 1)
#define SYSTEM_TIMER_IRQ2	(1 << 2)
#define SYSTEM_TIMER_IRQ3	(1 << 3)

#ifndef __ASSEMBLY__


static inline void enable_timer_interrupt(void)
{
}

static inline void disable_timer_interrupt(void)
{
}


#endif /* !__ASSEMBLY__ */

#endif /*	__MCUBE_ARM_SYNQUACER_IRQ_H__ */
