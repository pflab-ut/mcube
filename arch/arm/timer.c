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
  set_cntv_tval_el0(timer_tick);
  enable_cntv_ctl_el0();
  enable_local_irq();
}

void stop_timer(unsigned int ch)
{
  disable_cntv_ctl_el0();
}


