/**
 * @file include/sim/timer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_TIMER_H__
#define __MCUBE_SIM_TIMER_H__


#ifndef __ASSEMBLY__


/**
 * @fn static inline uint64_t tsc2usec(uint64_t tsc)
 * @brief Time stamp counter to microsecond.
 *
 * @param tsc Time stamp counter.
 * @return Microsecond.
 */
static inline uint64_t tsc2usec(uint64_t tsc)
{
  return ((uint64_t)((tsc) * CPU_USEC_PER_CLOCK_MHZ + 0.5));
}

/**
 * @fn static inline uint64_t tsc2nsec(uint64_t tsc)
 * @brief Time stamp counter to nanosecond.
 *
 * @param tsc Time stamp counter.
 * @return Nanosecond.
 */
static inline uint64_t tsc2nsec(uint64_t tsc)
{
  return ((uint64_t)((tsc) * CPU_NSEC_PER_CLOCK_MHZ + 0.5));
}

/**
 * @fn static inline void delay(unsigned long us)
 * @brief Delay [us].
 *
 * @param us Microsecond.
 */
static inline void delay(unsigned long us)
{
  volatile unsigned long cur = rdtsc();

  while (tsc2usec(rdtsc() - cur) < us) {
    pause();
  }
}

/**
 * @fn static inline unsigned long get_current_cpu_time(void)
 * @brief get current CPU time.
 *
 * @return Current CPU time.
 */
static inline unsigned long get_current_cpu_time(void)
{
  return rdtsc();
}


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_TIMER_H__ */
