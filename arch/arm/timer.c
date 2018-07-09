/**
 * @file arch/arm/timer.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


unsigned long timer_cntfrq = 0;
unsigned long timer_tick = 0;


void init_timer(unsigned long tick_us)
{
  timer_cntfrq = get_cntfrq_el0();
  printk("get_timer_frequency() = %lu\n", timer_cntfrq);
  
  timer_tick = (timer_cntfrq * TICK_USEC) / (1000 * 1000);
  printk("timer_tick = %lu\n", timer_tick);
  /* route cor0cntv to core0 irq */
  mmio_out64(TIMER_CORE0_TIMERS_INTERRUPT_CTRL, 1 << 3);

  printk("timer_local_timer_ctrl_status = %x\n", mmio_in32(TIMER_LOCAL_TIMER_CTRL_STATUS));
  /* clear interrupt flag and reload local timer */
  mmio_out32(TIMER_LOCAL_TIMER_WRITE_FLAGS, 0x3 << 30);
  /* local timer interrupt goes to Core 0 IRQ */
  mmio_out32(TIMER_LOCAL_INTERRUPT_ROUTING, 0);

  enable_timer_interrupt();
}

void start_timer(unsigned int ch)
{
  set_cntv_tval_el0(timer_tick);
  enable_cntv_ctl_el0();
  enable_interrupt();
}

void stop_timer(unsigned int ch)
{
  disable_cntv_ctl_el0();
}


