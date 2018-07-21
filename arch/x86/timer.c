/**
 * @file arch/x86/timer.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

unsigned long cpu_bus_freq_mhz = 0;
unsigned long sys_tsc = 0;

unsigned long timer_count[NR_INTRA_KERNEL_CPUS];
unsigned long tcur[NR_INTRA_KERNEL_CPUS], tprev[NR_INTRA_KERNEL_CPUS];

#define LAPIC_TIMER 0
#define HPET_TIMER 0
#define PIT_TIMER 1

                         
static void measure_cpu_frequency(void)
{
}


void init_timer(unsigned long tick_us)
{
#ifdef LAPIC_TIMER
  init_lapic_timer(tick_us);
#elif HPET_TIMER
  init_hpet_timer(tick_us);
#elif PIT_TIMER
  init_pit_timer(tick_us);
#endif /* LAPIC_TIMER */
  measure_cpu_frequency();
}

void start_timer(unsigned int ch)
{
#ifdef LAPIC_TIMER
  start_lapic_timer(ch);
#elif HPET_TIMER
  start_hpet_timer(ch);
#elif PIT_TIMER
  start_pit_timer(ch);
#endif /* LAPIC_TIMER */
}

void stop_timer(unsigned int ch)
{
#ifdef LAPIC_TIMER
  stop_lapic_timer(ch);
#elif HPET_TIMER
  stop_hpet_timer(ch);
#elif PIT_TIMER
  stop_pit_timer(ch);
#endif /* LAPIC_TIMER */
}


