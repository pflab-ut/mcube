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

void timer_init(void);
void handle_timer_irq(void);

static inline unsigned long get_cntvct_el0(void)
{
  uint64_t val;
  asm volatile("mrs %0, cntvct_el0" : "=r" (val));
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
  asm volatile ("mrs %0, cntv_tval_el0" : "=r"(data));
  return data;
}

static inline void set_cntv_tval_el0(unsigned long data)
{
  asm volatile ("msr cntv_tval_el0, %0" :: "r"(data));
}

static inline void enable_cntv_ctl_el0(void)
{
  unsigned long cntv_ctl = 1;
  cntv_ctl = 1;
  asm volatile ("msr cntv_ctl_el0, %0" :: "r" (cntv_ctl));
}

static inline void disable_cntv_ctl_el0(void)
{
  unsigned long cntv_ctl = 0;
  asm volatile ("msr cntv_ctl_el0, %0" :: "r" (cntv_ctl));
}


static inline unsigned long tsc2usec(unsigned long tsc)
{
	return ((unsigned long) ((tsc) * CPU_USEC_PER_CLOCK_MHZ + 0.5));
}

static inline unsigned long tsc2nsec(unsigned long tsc)
{
	return ((unsigned long) ((tsc) * CPU_NSEC_PER_CLOCK_MHZ + 0.5));
}

static inline unsigned long get_current_cpu_time(void)
{
  /* NOTE: Frequency of Raspberry Pi3 is 62.5MHz (Timer Frequency). */
  unsigned long data;
  asm volatile("mrs %0, cntpct_el0" : "=r"(data));
  return data;  
}

static inline void delay(unsigned long us)
{
  unsigned long now = get_current_cpu_time();
  unsigned long timer_cntfrq = get_cntfrq_el0();
  /* do loop until us elapse. */
  while (((get_current_cpu_time() - now) * 1000 * 1000) / timer_cntfrq <= us) {
  }
}


static inline void enable_timer_interrupt(void)
{
  mmio_out32(TIMER_LOCAL_TIMER_CTRL_STATUS,
             mmio_in32(TIMER_LOCAL_TIMER_CTRL_STATUS) | (0x3 << 28));
}

static inline void disable_timer_interrupt(void)
{
  mmio_out32(TIMER_LOCAL_TIMER_CTRL_STATUS,
             mmio_in32(TIMER_LOCAL_TIMER_CTRL_STATUS) & ~(0x3 << 28));
}


#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_ARM_TIMER_H__ */
