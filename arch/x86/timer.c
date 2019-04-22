/**
 * @file arch/x86/timer.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_timer(unsigned long tick_us)
{
#if APIC_TIMER
  init_apic_timer(tick_us, APIC_TESTS_VECTOR);
#elif PIT_TIMER
  init_pit_timer(tick_us);
#elif HPET_TIMER
  init_hpet_timer(tick_us);
#else
#error "Unknown timer"
#endif
}

void start_timer(void)
{
#if APIC_TIMER
  start_apic_timer();
#elif PIT_TIMER
  start_pit_timer();
#elif HPET_TIMER
  start_hpet_timer();
#else
#error "Unknown timer"
#endif
}

void stop_timer(void)
{
#if APIC_TIMER
  stop_apic_timer();
#elif PIT_TIMER
  stop_pit_timer();
#elif HPET_TIMER
  stop_hpet_timer();
#else
#error "Unknown timer"
#endif
}
