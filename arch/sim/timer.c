/**
 * @file arch/sim/timer.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void init_timer(unsigned long tick_us)
{
  __uninitialized(tick_us);
}


void start_timer(void)
{
}

void stop_timer(void)
{
}
