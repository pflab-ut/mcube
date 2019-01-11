/**
 * @file arch/arm/synquacer/timer.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_timer(unsigned long tick_us)
{
  timer_cntfrq = get_cntfrq_el0();
  //  printk("get_timer_frequency() = %lu\n", timer_cntfrq);
  
  timer_tick = (timer_cntfrq * TICK_USEC) / (1000 * 1000);
  //  printk("timer_tick = %lu\n", timer_tick);
#if 0
  /* route cor0cntv to core0 irq */
  mmio_out64(TIMER_CORE0_TIMERS_INTERRUPT_CTRL, 1 << 3);

  //  printk("timer_local_timer_ctrl_status = %x\n", mmio_in32(TIMER_LOCAL_TIMER_CTRL_STATUS));
  /* clear interrupt flag and reload local timer */
  mmio_out32(TIMER_LOCAL_TIMER_WRITE_FLAGS, 0x3 << 30);
  /* local timer interrupt goes to Core 0 IRQ */
  mmio_out32(TIMER_LOCAL_INTERRUPT_ROUTING, 0);
#endif
  
  enable_timer_interrupt();
}

