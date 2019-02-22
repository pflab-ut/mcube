/**
 * @file arch/arm/raspi3/timer.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_timer(unsigned long tick_us)
{
  unsigned long cpu = get_cpu_id();
  timer_cntfrq = get_cntfrq_el0();
    
  timer_tick = (timer_cntfrq * tick_us) / (1000 * 1000);
  printk("timer_cntfrq = %d timer_tick = %d\n", timer_cntfrq, timer_tick);
  /* clear interrupt flag and reload local timer */
  mmio_out32(LP_LOCAL_TIMER_WRITE_FLAGS,
             LP_LOCAL_TIMER_WRITE_FLAGS_INTERRUPT_FLAG_CLEAR
             | LP_LOCAL_TIMER_WRITE_FLAGS_LOCAL_TIMER_RELOADED);
  /* IRQ routing to core [cpu] */
  mmio_out32(LP_GPU_INTERRUPT_ROUTING,
             LP_GPU_INTERRUPT_ROUTING_FIQ_CORE(cpu)
             | LP_GPU_INTERRUPT_ROUTING_IRQ_CORE(cpu));
  /* route cor0cntv to core [cpu] irq */
  mmio_out32(LP_CORE_TIMERS_INTERRUPT_CTRL(cpu),
             LP_CORE_TIMERS_INTERRUPT_CTRL_CNTVIRQ_IRQ_CTRL);
  /* local timer interrupt goes to core [cpu] IRQ */
  mmio_out32(LP_LOCAL_INTERRUPT_ROUTING,
             LP_LOCAL_INTERRUPT_ROUTING_CORE_IRQ(cpu));
  
  enable_timer_interrupt();
}

