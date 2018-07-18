/**
 * @file include/mcube/barrier.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_BARRIER_H__
#define __MCUBE_MCUBE_BARRIER_H__

#ifndef	__ASSEMBLY__


void smp_barrier(int index);
void init_smp_barrier(void);

#define NR_SMP_SCHED_BARRIERS 16
extern volatile uint32_t smp_sched_barrier[NR_SMP_SCHED_BARRIERS];


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_BARRIER_H__ */
