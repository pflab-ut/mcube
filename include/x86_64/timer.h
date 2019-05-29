/**
 * @file include/x86_64/timer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_TIMER_H__
#define __MCUBE_X86_64_TIMER_H__


/* enable each timer */
#define APIC_TIMER 1
#define PIT_TIMER 0
#define HPET_TIMER 0

#ifndef __ASSEMBLY__


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
  return rdtsc();
}


#endif	/* !__ASSEMBLY__ */

#endif	/* __MCUBE_X86_64_TIMER_H__ */
