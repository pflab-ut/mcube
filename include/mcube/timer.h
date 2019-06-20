/**
 * @file include/mcube/timer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_TIMER_H__
#define __MCUBE_MCUBE_TIMER_H__

#ifndef __ASSEMBLY__

#if !CONFIG_ARCH_SIM

/**
 * @struct tm
 * @brief Time information.
 */
struct tm {
  /**
   * Seconds (0-60).
   */
  int tm_sec;

  /**
   * Minutes (0-59).
   */
  int tm_min;

  /**
   * Hours (0-23).
   */
  int tm_hour;

  /**
   * Day of the month (1-31).
   */
  int tm_mday;

  /**
   * Month (0-11).
   */
  int tm_mon;

  /**
   * Year - 1900.
   */
  int tm_year;

  /**
   * Day of the week (0-6, Sunday = 0).
   */
  int tm_wday;

  /**
   * Day in the year (0-365, 1 Jan = 0).
   */
  int tm_yday;

  /**
   * Daylight saving time.
   */
  int tm_isdst;
};
#endif /* !CONFIG_ARCH_SIM */


/**
 * @fn void start_timer(void)
 * @brief start timer.
 */
void start_timer(void);

/**
 * @fn void stop_timer(void)
 * @brief stop timer.
 */
void stop_timer(void);

/**
 * @fn void init_timer(unsigned long tick_us)
 * @brief initialize timer.
 *
 * @param tick_us Tick [us].
 */
void init_timer(unsigned long tick_us);


/**
 * @def TICK_USEC
 * @brief Tick [us].
 */
// 1us
//#define TICK_USEC 1
// 10us
//#define TICK_USEC 10
// 100us
//#define TICK_USEC 100
// 1ms
//#define TICK_USEC 1000
// 2ms
//#define TICK_USEC 2000
// 10ms
#define TICK_USEC 10000
// 100ms
//#define TICK_USEC 100000
// 1s
//#define TICK_USEC 1000000
// 10s
//#define TICK_USEC 10000000

/**
 * @def TICK_NSEC
 * @param Tick [ns].
 */
#define TICK_NSEC (1000 * TICK_USEC)

/**
 * @def SEC2MSEC(sec)
 * @brief Second to millisecond.
 *
 * @param sec Second.
 */
#define SEC2MSEC(sec) ((sec) * 1000)

/**
 * @def SEC2USEC(sec)
 * @brief Second to microsecond.
 *
 * @param sec Second.
 */
#define SEC2USEC(sec) ((sec) * 1000000)

/**
 * @def SEC2NSEC(sec)
 * @brief Second to nanosecond.
 *
 * @param sec Second.
 */
#define SEC2NSEC(sec) ((sec) * 1000000000)

/**
 * @def MSEC2SEC(msec)
 * @brief millisecond to second.
 *
 * @param msec Millisecond.
 */
#define MSEC2SEC(msec) ((msec) / 1000)

/**
 * @def MSEC2USEC(msec)
 * @brief Millisecond to microsecond.
 *
 * @param msec Millisecond.
 */
#define MSEC2USEC(msec) ((msec) * 1000)

/**
 * @def MSEC2NSEC(msec)
 * @brief Millisecond to nanosecond.
 */
#define MSEC2NSEC(msec) ((msec) * 1000000)

/**
 * @def USEC2SEC(usec)
 * @brief Microsecond to second.
 *
 * @param usec Microsecond.
 */
#define USEC2SEC(usec) ((usec) / 1000000)

/**
 * @def USEC2MSEC(usec)
 * @brief Microsecond to millisecond.
 *
 * @param usec Microsecond.
 */
#define USEC2MSEC(usec) ((usec) / 1000)

/**
 * @def USEC2NSEC(usec)
 * @brief Microsecond to nanosecond.
 *
 * @param usec microsecond.
 */
#define USEC2NSEC(usec) ((usec) * 1000)

/**
 * @def NSEC2SEC(nsec)
 * @brief Nanosecond to second.
 *
 * @param nsec Nanosecond.
 */
#define NSEC2SEC(nsec) ((nsec) / 1000000000)

/**
 * @def NSEC2MSEC(nsec)
 * @brief Nanosecond to millisecond.
 *
 * @param nsec Nanosecond.
 */
#define NSEC2MSEC(nsec) ((nsec) / 1000000)

/**
 * @def NSEC2USEC(nsec)
 * @brief Nanosecond to microsecond.
 *
 * @param nsec Nanosecond.
 */
#define NSEC2USEC(nsec) ((nsec) / 1000)


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_TIMER_H__ */
