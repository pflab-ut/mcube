/**
 * @file include/aarch64/timer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_TIMER_H__
#define __MCUBE_AARCH64_TIMER_H__


#ifndef __ASSEMBLY__

/**
 * @var timer_cntfrq
 * @brief Counter-timer frequency.
 */
extern unsigned long timer_cntfrq;

/**
 * @var timer_tick
 * @brief Timer tick.
 */
extern unsigned long timer_tick;

/**
 * @fn void handle_timer_irq(void)
 * @brief Timer IRQ handler.
 */
void handle_timer_irq(void);

/**
 * @fn static inline unsigned long get_cntvct_el0(void)
 * @brief get counter-timer virtual count.
 *
 * @return Counter-timer virtual count.
 */
static inline unsigned long get_cntvct_el0(void)
{
  uint64_t val;
  asm volatile("mrs %0, cntvct_el0" : "=r"(val));
  return val;
}

/**
 * @fn static inline unsigned long get_cntfrq_el0(void)
 * @brief get counter-timer frequency.
 *
 * @return Counter-timer frequency.
 */
static inline unsigned long get_cntfrq_el0(void)
{
  unsigned long data;
  asm volatile("mrs %0, cntfrq_el0" : "=r"(data));
  return data;
}

/**
 * @fn static inline unsigned long get_cntv_ctl_el0(void)
 * @brief get counter-timer virtual timer control.
 *
 * @return Counter-timer virtual timer control.
 */
static inline unsigned long get_cntv_ctl_el0(void)
{
  unsigned long data;
  asm volatile("mrs %0, cntv_ctl_el0" : "=r"(data));
  return data;
}

/**
 * @fn static inline void set_cntv_ctl_el0(unsigned long data)
 * @brief set counter-timer virtual timer control.
 *
 * @param data Data.
 */
static inline void set_cntv_ctl_el0(unsigned long data)
{
  asm volatile("msr cntv_ctl_el0, %0" :: "r"(data));
}

/**
 * @fn static inline unsigned long get_cntp_cval_el0(void)
 * @brief get counter-timer physical timer compare value.
 *
 * @return Counter-timer physical timer compare value.
 */
static inline unsigned long get_cntp_cval_el0(void)
{
  unsigned long data;
  asm volatile("mrs %0, cntp_cval_el0" : "=r"(data));
  return data;
}

/**
 * @fn static inline void set_cntp_cval_el0(unsigned long data)
 * @brief set counter-timer physical timer compare value.
 *
 * @param data Data.
 */
static inline void set_cntp_cval_el0(unsigned long data)
{
  asm volatile("msr cntp_cval_el0, %0" :: "r"(data));
}

/**
 * @fn static inline unsigned int get_cntp_tval_el0(void)
 * @brief get counter-timer physical timer timer value.
 *
 * @return Counter-timer physical timer timer value.
 */
static inline unsigned int get_cntp_tval_el0(void)
{
  unsigned long data;
  asm volatile("mrs %0, cntp_tval_el0" : "=r"(data));
  return data;
}

/**
 * @fn static inline void set_cntp_tval_el0(unsigned long data)
 * @brief set counter-timer physical timer timer value.
 *
 * @param data Data.
 */
static inline void set_cntp_tval_el0(unsigned long data)
{
  asm volatile("msr cntp_tval_el0, %0" : "=r"(data));
}

/**
 * @fn static inline unsigned long get_cntv_tval_el0(void)
 * @brief get counter-timer virtual timer timer value.
 *
 * @return Counter-timer virtual timer time rvalue.
 */
static inline unsigned long get_cntv_tval_el0(void)
{
  unsigned long data;
  asm volatile("mrs %0, cntv_tval_el0" : "=r"(data));
  return data;
}

/**
 * @fn static inline void set_cntv_tval_el0(unsigned long data)
 * @brief set counter-timer virtual timer timer value.
 *
 * @param data Data.
 */
static inline void set_cntv_tval_el0(unsigned long data)
{
  asm volatile("msr cntv_tval_el0, %0" :: "r"(data));
}

/**
 * @fn static inline unsigned long get_cntpct_el0(void)
 * @brief counter-timer physical count.
 *
 * @return Counter-timer physical count.
 */
static inline unsigned long get_cntpct_el0(void)
{
  unsigned long data;
  asm volatile("mrs %0, cntpct_el0" : "=r"(data));
  return data;
}

/**
 * @fn static inline unsigned long get_pmccntr_el0(void)
 * @brief get performance monitors cycle count register.
 *
 * @return Performance monitors cycle count register.
 */
static inline unsigned long get_pmccntr_el0(void)
{
  unsigned long data;
  asm volatile("mrs %0, pmccntr_el0" : "=r"(data));
  return data;
}

/**
 * @fn static inline void enable_cntv_ctl_el0(void)
 * @brief enable counter-timer physical timer control.
 */
static inline void enable_cntv_ctl_el0(void)
{
  unsigned long reg = CNTV_CTL_EL0_ENABLE;
  asm volatile("msr cntv_ctl_el0, %0" :: "r"(reg));
}

/**
 * @fn static inline void disable_cntv_ctl_el0(void)
 * @brief disable counter-timer physical timer control.
 */
static inline void disable_cntv_ctl_el0(void)
{
  unsigned long reg = CNTV_CTL_EL0_DISABLE;
  asm volatile("msr cntv_ctl_el0, %0" :: "r"(reg));
}

/**
 * @fn static inline unsigned long tsc2usec(unsigned long tsc)
 * @brief Time stamp counter to microsecond.
 *
 * @param tsc Time stamp counter.
 * @return Microsecond.
 */
static inline unsigned long tsc2usec(unsigned long tsc)
{
  return ((unsigned long)((tsc) * CPU_USEC_PER_CLOCK_MHZ + 0.5));
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
  return ((unsigned long)((tsc) * CPU_NSEC_PER_CLOCK_MHZ + 0.5));
}


/**
 * @fn static inline void delay(unsigned long us)
 * @brief Delay [us].
 *
 * @param us Microsecond.
 */
static inline void delay(unsigned long us)
{
  unsigned long now = get_cntvct_el0();
  unsigned long timer_cntfrq = get_cntfrq_el0();

  /* do loop until us elapse. */
  while (((get_cntvct_el0() - now) * 1000 * 1000) / timer_cntfrq <= us) {
  }
}

/**
 * @fn static inline void delay_cycles(unsigned long cycles)
 * @brief Delay [cycle].
 *
 * @param cycles Delay [cycles].
 */
static inline void delay_cycles(unsigned long cycles)
{
  unsigned int i;

  for (i = 0; i < cycles; i++) {
    nop();
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
  return get_pmccntr_el0();
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_AARCH64_TIMER_H__ */
