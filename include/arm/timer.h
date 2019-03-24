/**
 * @file include/arm/timer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_TIMER_H__
#define __MCUBE_ARM_TIMER_H__



#ifndef __ASSEMBLY__

extern unsigned long timer_cntfrq;
extern unsigned long timer_tick;

void handle_timer_irq(void);

static inline unsigned long get_cntvct_el0(void)
{
  uint64_t val;
  asm volatile("mrs %0, cntvct_el0" : "=r"(val));
  return val;
}


static inline unsigned long get_cntfrq_el0(void)
{
  unsigned long data;
  asm volatile("mrs %0, cntfrq_el0" : "=r"(data));
  return data;
}

static inline unsigned long get_cntv_ctl_el0(void)
{
  unsigned long data;
  asm volatile("mrs %0, cntv_ctl_el0" : "=r"(data));
  return data;
}

static inline void set_cntv_ctl_el0(unsigned long data)
{
  asm volatile("msr cntv_ctl_el0, %0" :: "r"(data));
}

static inline unsigned long get_cntp_cval_el0(void)
{
  unsigned long data;
  asm volatile("mrs %0, cntp_cval_el0" : "=r"(data));
  return data;
}

static inline void set_cntp_cval_el0(unsigned long data)
{
  asm volatile("msr cntp_cval_el0, %0" :: "r"(data));
}

static inline unsigned int get_cntp_tval_el0(void)
{
  unsigned long data;
  asm volatile("mrs %0, cntp_tval_el0" : "=r"(data));
  return data;
}

static inline void set_cntp_tval_el0(unsigned long data)
{
  asm volatile("msr cntp_tval_el0, %0" : "=r"(data));
}

static inline unsigned long get_cntv_tval_el0(void)
{
  unsigned long data;
  asm volatile("mrs %0, cntv_tval_el0" : "=r"(data));
  return data;
}

static inline void set_cntv_tval_el0(unsigned long data)
{
  asm volatile("msr cntv_tval_el0, %0" :: "r"(data));
}

static inline unsigned long get_cntpct_el0(void)
{
  unsigned long data;
  asm volatile("mrs %0, cntpct_el0" : "=r"(data));
  return data;
}

static inline unsigned long get_pmccntr_el0(void)
{
  unsigned long data;
  asm volatile("mrs %0, pmccntr_el0" : "=r"(data));
  return data;
}


static inline void enable_cntv_ctl_el0(void)
{
  unsigned long reg = CNTV_CTL_EL0_ENABLE;
  asm volatile("msr cntv_ctl_el0, %0" :: "r"(reg));
}

static inline void disable_cntv_ctl_el0(void)
{
  unsigned long reg = CNTV_CTL_EL0_DISABLE;
  asm volatile("msr cntv_ctl_el0, %0" :: "r"(reg));
}


static inline unsigned long tsc2usec(unsigned long tsc)
{
  return ((unsigned long)((tsc) * CPU_USEC_PER_CLOCK_MHZ + 0.5));
}

static inline unsigned long tsc2nsec(unsigned long tsc)
{
  return ((unsigned long)((tsc) * CPU_NSEC_PER_CLOCK_MHZ + 0.5));
}


static inline unsigned long get_current_cpu_time(void)
{
  return get_pmccntr_el0();
}

static inline void delay(unsigned long us)
{
  unsigned long now = get_cntvct_el0();
  unsigned long timer_cntfrq = get_cntfrq_el0();

  /* do loop until us elapse. */
  while (((get_cntvct_el0() - now) * 1000 * 1000) / timer_cntfrq <= us) {
  }
}

static inline void delay_cycles(unsigned int n)
{
  unsigned int i;

  for (i = 0; i < n; i++) {
    nop();
  }
}



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_ARM_TIMER_H__ */
