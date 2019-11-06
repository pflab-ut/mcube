/**
 * @file arch/axis/timer.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void init_timer(unsigned long tick_us)
{
  set_timer_period(USEC_TO_CPU_CLOCK(tick_us));
}

void start_timer(void)
{
  enable_timer_interrupt();
  enable_timer();
}

void stop_timer(void)
{
  disable_timer();
  disable_timer_interrupt();
}

