/**
 * @file include/axis/timer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_TIMER_H__
#define __MCUBE_AXIS_TIMER_H__

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
  return (tsc * 1000) / CPU_CLOCK_MHZ_PER_USEC;
}

/**
 * @fn static inline void delay(unsigned long us)
 * @brief Delay [us].
 *
 * @param us Microsecond.
 */
static inline void delay(unsigned long us)
{
  volatile unsigned long cur = get_time_stamp_counter();

  while (tsc2usec(get_time_stamp_counter() - cur) < us)
    ;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_AXIS_TIMER_H__ */
