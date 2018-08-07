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
#define LAPIC_TIMER 0
#define PIT_TIMER 1
#define HPET_TIMER 0

#ifndef __ASSEMBLY__

//----------------------------------------------------------------------------
//  @function   timer_init
/// @brief      Initialize the timer controller so that it interrupts the
///             kernel at the requested frequency.
///
/// @details    Interrupts are enabled at the end of the function, so
///             timer_enable does not need to be called after timer_init.
///
///             Due to the clock granularity (1193181Hz), the requested
///             frequency may not be perfectly met.
///
/// @param[in]  frequency   The interrupt frequency in Hz. Clamped to the
///                         range [19:1193181].
//----------------------------------------------------------------------------
void timer_init(uint32_t frequency);

//----------------------------------------------------------------------------
//  @function   timer_enable
/// @brief      Enable timer interrupts.
//----------------------------------------------------------------------------
void timer_enable(void);

//----------------------------------------------------------------------------
//  @function   timer_disable
/// @brief      Disable timer interrupts.
//----------------------------------------------------------------------------
void timer_disable(void);


static inline unsigned long tsc2usec(unsigned long tsc)
{
	return ((unsigned long) ((tsc) * CPU_USEC_PER_CLOCK_MHZ + 0.5));
}

static inline unsigned long tsc2nsec(unsigned long tsc)
{
	return ((unsigned long) ((tsc) * CPU_NSEC_PER_CLOCK_MHZ + 0.5));
}

static inline unsigned long usec2tsc(unsigned long usec)
{
	return usec * CPU_CLOCK_MHZ_PER_USEC;
}

static inline unsigned long nsec2tsc(unsigned long nsec)
{
	return nsec * CPU_CLOCK_MHZ_PER_NSEC;
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
  //	return rdtscp();
	return rdtsc();
	//	return read_HPET_counter();
}

void init_pit_timer(unsigned long tick_us);
void start_pit_timer(unsigned int ch);
void stop_pit_timer(unsigned int ch);

extern unsigned long cpu_bus_freq_mhz;
extern unsigned long sys_tsc;

extern unsigned long timer_count[NR_INTRA_KERNEL_CPUS];
extern unsigned long tcur[NR_INTRA_KERNEL_CPUS], tprev[NR_INTRA_KERNEL_CPUS];


#endif	/* !__ASSEMBLY__ */

#endif	/* __MCUBE_X86_TIMER_H__ */
