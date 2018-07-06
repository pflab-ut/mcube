/**
 * @file include/mcube/hardirq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_HARDIRQ_H__
#define __MCUBE_MCUBE_HARDIRQ_H__

/*
 * We put the hardirq and softirq counter into the preemption
 * counter. The bitmask has the following meaning:
 *
 * - bits 0-7 are the preemption count (max preemption depth: 256)
 * - bits 8-15 are the softirq count (max # of softirqs: 256)
 *
 * The hardirq count can be overridden per architecture, the default is:
 *
 * - bits 16-27 are the hardirq count (max # of hardirqs: 4096)
 * - ( bit 28 is the PREEMPT_ACTIVE flag. )
 *
 * PREEMPT_MASK: 0x000000ff
 * SOFTIRQ_MASK: 0x0000ff00
 * HARDIRQ_MASK: 0x0fff0000
 */
#define PREEMPT_BITS    8
#define SOFTIRQ_BITS    8

#define HARDIRQ_BITS    12


#define PREEMPT_SHIFT   0
#define SOFTIRQ_SHIFT   (PREEMPT_SHIFT + PREEMPT_BITS)
#define HARDIRQ_SHIFT   (SOFTIRQ_SHIFT + SOFTIRQ_BITS)

#define __IRQ_MASK(x)   ((1UL << (x))-1)

#define PREEMPT_MASK    (__IRQ_MASK(PREEMPT_BITS) << PREEMPT_SHIFT)
#define SOFTIRQ_MASK    (__IRQ_MASK(SOFTIRQ_BITS) << SOFTIRQ_SHIFT)
#define HARDIRQ_MASK    (__IRQ_MASK(HARDIRQ_BITS) << HARDIRQ_SHIFT)

#define PREEMPT_OFFSET  (1UL << PREEMPT_SHIFT)
#define SOFTIRQ_OFFSET  (1UL << SOFTIRQ_SHIFT)
#define HARDIRQ_OFFSET  (1UL << HARDIRQ_SHIFT)

#define preemptible()  (preempt_count() == 0 && !irqs_disabled())
/* ATTENTION: */
//# define IRQ_EXIT_OFFSET HARDIRQ_OFFSET
#define IRQ_EXIT_OFFSET (HARDIRQ_OFFSET-1)


#endif /* __MCUBE_MCUBE_HARDIRQ_H__ */
