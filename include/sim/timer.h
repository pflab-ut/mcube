/**
 * @file include/sim/timer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_TIMER_H__
#define __MCUBE_SIM_TIMER_H__


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

static inline unsigned long get_current_cpu_time(void)
{
  return rdtsc();
}


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_TIMER_H__ */
