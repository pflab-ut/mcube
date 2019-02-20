/**
 * @file arch/axis/timer.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

/**
 * The start_timer() function starts timer.
 */
void start_timer(__unused unsigned int ch)
{
  printk("start_timer()\n");
  enable_timer_interrupt();
  enable_timer();
}

/**
 * The stop_timer() function stops timer.
 */
void stop_timer(__unused unsigned int ch)
{
  disable_timer();
  disable_timer_interrupt();
}

/**
 * The init_timer() function initializes timer.
 */
void init_timer(unsigned long tick_us)
{
  set_timer_period(USEC_TO_CPU_CLOCK(tick_us));
}
