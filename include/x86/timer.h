/**
 * @file include/x86/timer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_TIMER_H__
#define __MCUBE_X86_TIMER_H__

#define PIT_HZ 1193180 /* 1.19318MHz */

#define PIT_CNT0 0x0040
#define PIT_CTRL 0x0043

/* enable each timer */
#define LAPIC_TIMER 1
#define PIT_TIMER 0
#define HPET_TIMER 0

#ifndef __ASSEMBLY__


/**
 * @fn    timer_enable
 * @brief Enable timer interrupts.
 */
void timer_enable(void);

/**
 * @fn    timer_disable
 * @brief Disable timer interrupts.
 */
void timer_disable(void);


static inline unsigned long tsc2usec(unsigned long tsc)
{
  return tsc / CPU_CLOCK_MHZ_PER_USEC;
}

static inline unsigned long tsc2nsec(unsigned long tsc)
{
  return (1000 * tsc) / CPU_CLOCK_MHZ_PER_USEC;
}

static inline unsigned long usec2tsc(unsigned long usec)
{
  return usec * CPU_CLOCK_MHZ_PER_USEC;
}

static inline unsigned long nsec2tsc(unsigned long nsec)
{
  return (nsec * CPU_CLOCK_MHZ_PER_USEC) / 1000;
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
  //  return rdtscp();
  return rdtsc();
  //	return read_HPET_counter();
}

void init_pit_timer(unsigned long tick_us);
void start_pit_timer(unsigned int ch);
void stop_pit_timer(unsigned int ch);



#endif	/* !__ASSEMBLY__ */

#endif	/* __MCUBE_X86_TIMER_H__ */
