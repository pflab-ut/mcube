/**
 * @file include/mcube/timer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_TIMER_H__
#define	__MCUBE_MCUBE_TIMER_H__

#ifndef __ASSEMBLY__

extern seqlock_struct xtime_lock;

static inline void update_jiffies(void)
{
#if CONFIG_ARCH_SIM
  exec_jiffies = 0;
#endif
	sys_jiffies++;
}


void start_timer(unsigned int ch);
void stop_timer(unsigned int ch);
void init_timer(unsigned long tick_us);

irqreturn_t handle_timer_tick(int irq, void *dummy);


extern unsigned long sys_tsc;

#define TIMER_ONESHOT 0x0
#define TIMER_PERIODIC 0x1

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

#define TICK_NSEC (1000 * TICK_USEC)

#define SEC2MSEC(sec)  ((sec) * 1000)
#define SEC2USEC(sec)  ((sec) * 1000000)
#define SEC2NSEC(sec)  ((sec) * 1000000000)

#define MSEC2SEC(msec)  ((msec) / 1000)
#define MSEC2USEC(msec) ((msec) * 1000)
#define MSEC2NSEC(msec) ((msec) * 1000000)

#define USEC2SEC(usec)  ((usec) / 1000000)
#define USEC2MSEC(usec) ((usec) / 1000)
#define USEC2NSEC(usec) ((usec) * 1000)

#define NSEC2SEC(nsec)  ((nsec) / 1000000000)
#define NSEC2MSEC(nsec) ((nsec) / 1000000)
#define NSEC2USEC(nsec) ((nsec) / 1000)



#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_TIMER_H__ */
