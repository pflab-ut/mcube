/**
 * @file arch/arm/timer.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


unsigned long timer_cntfrq = 0;
unsigned long timer_tick = 0;


void start_timer(unsigned int ch)
{
  printk("timer_tick = %lu\n", timer_tick);
  set_cntv_tval_el0(timer_tick);
  enable_cntv_ctl_el0();
}

void stop_timer(unsigned int ch)
{
  disable_cntv_ctl_el0();
}

