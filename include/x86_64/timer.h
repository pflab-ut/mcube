/**
 * @file include/x86_64/timer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_TIMER_H__
#define __MCUBE_X86_64_TIMER_H__


/* enable each timer */

/**
 * @def APIC_TIMER
 * @brief APIC timer.
 */
#define APIC_TIMER 1

/**
 * @def PIT_TIMER
 * @brief PIT timer.
 */
#define PIT_TIMER 0

/**
 * @def HPET_TIMER
 * @brief HPET timer.
 */
#define HPET_TIMER 0

#ifndef __ASSEMBLY__

/**
 * @fn static inline unsigned long tsc2usec(unsigned long tsc)
 * @brief Time stamp counter to microsecond.
 *
 * @param tsc Time stamp counter.
 * @return Microsecond.
 */
static inline unsigned long tsc2usec(unsigned long tsc)
{
  return tsc / CPU_CLOCK_MHZ_PER_USEC;
}

/**
 * @fn static inline unsigned long tsc2nsec(unsigned long tsc)
 * @brief Time stamp counter to nanosecond.
 *
 * @param tsc Time stamp counter.
 * @return Nanosecond.
 */
static inline unsigned long tsc2nsec(unsigned long tsc)
{
  return (1000 * tsc) / CPU_CLOCK_MHZ_PER_USEC;
}

/**
 * @fn static inline unsigned long usec2tsc(unsigned long usec)
 * @brief Microsecond to time stamp counter.
 *
 * @param usec Microsecond.
 * @return Time stamp counter.
 */
static inline unsigned long usec2tsc(unsigned long usec)
{
  return usec * CPU_CLOCK_MHZ_PER_USEC;
}

/**
 * @fn static inline unsigned long nsec2tsc(unsigned long nsec)
 * @brief Nanosecond to time stamp counter.
 *
 * @param nsec Nanosecond.
 */
static inline unsigned long nsec2tsc(unsigned long nsec)
{
  return (nsec * CPU_CLOCK_MHZ_PER_USEC) / 1000;
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


#endif	/* !__ASSEMBLY__ */

#endif	/* __MCUBE_X86_64_TIMER_H__ */
