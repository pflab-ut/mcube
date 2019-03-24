/**
 * @file include/sim/timer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_TIMER_H__
#define __MCUBE_SIM_TIMER_H__


#define NR_TIMERS 4
#define TIMER_MAXID 3

#define SYSTEM_TIMER 0
#define IRQ_SYSTEM_TIMER 0

#define PERIODIC_INT_START  0

#define ONESHOT_INT_START  1


#ifndef __ASSEMBLY__

#include <time.h>

static inline uint64_t tsc2usec(uint64_t tsc)
{
  return ((uint64_t)((tsc) * CPU_USEC_PER_CLOCK_MHZ + 0.5));
}

static inline uint64_t tsc2nsec(uint64_t tsc)
{
  return ((uint64_t)((tsc) * CPU_NSEC_PER_CLOCK_MHZ + 0.5));
}


static inline void delay(unsigned long us)
{
  volatile unsigned long cur = rdtsc();

  while (tsc2usec(rdtsc() - cur) < us) {
    pause();
  }
}

void init_tsc(void);

extern double clk_per_nsec, nsec_per_clk;

static inline unsigned long get_current_cpu_time(void)
{
  return rdtsc();
}


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_TIMER_H__ */
