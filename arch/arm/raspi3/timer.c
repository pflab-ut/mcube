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
  printk("get_timer_frequency() = %lu\n", timer_cntfrq);
  
  timer_tick = (timer_cntfrq * TICK_USEC) / (1000 * 1000);
  printk("timer_tick = %lu\n", timer_tick);
  printk("timer_local_timer_ctrl_status = %x\n",
         mmio_in32(LP_LOCAL_TIMER_CTRL_STATUS));
  /* clear interrupt flag and reload local timer */
  mmio_out32(LP_LOCAL_TIMER_WRITE_FLAGS,
             LP_LOCAL_TIMER_WRITE_FLAGS_INTERRUPT_FLAG_CLEAR
             | LP_LOCAL_TIMER_WRITE_FLAGS_LOCAL_TIMER_RELOADED);
  printk("cpu = %lu\n", cpu);
  switch (cpu) {
  case 0:
    /* IRQ routing to core 0 */
    mmio_out32(LP_GPU_INTERRUPTS_ROUTING,
               LP_GPU_INTERRUPT_ROUTING_FIQ_CORE0
               | LP_GPU_INTERRUPT_ROUTING_IRQ_CORE0);
    /* route cor0cntv to core0 irq */
    mmio_out32(LP_CORE0_TIMERS_INTERRUPT_CTRL,
               LP_CORE_TIMERS_INTERRUPT_CTRL_CNTVIRQ_IRQ_CTRL);
    /* local timer interrupt goes to Core 0 IRQ */
    mmio_out32(LP_LOCAL_INTERRUPT_ROUTING,
               LP_LOCAL_INTERRUPT_ROUTING_CORE0_IRQ);
    break;
  case 1:
    /* IRQ routing to core 1 */
    mmio_out32(LP_GPU_INTERRUPTS_ROUTING,
               LP_GPU_INTERRUPT_ROUTING_FIQ_CORE1
               | LP_GPU_INTERRUPT_ROUTING_IRQ_CORE1);
    /* route cor0cntv to core1 irq */
    mmio_out32(LP_CORE1_TIMERS_INTERRUPT_CTRL,
               LP_CORE_TIMERS_INTERRUPT_CTRL_CNTVIRQ_IRQ_CTRL);
    /* local timer interrupt goes to Core 1 IRQ */
    mmio_out32(LP_LOCAL_INTERRUPT_ROUTING,
               LP_LOCAL_INTERRUPT_ROUTING_CORE1_IRQ);
    break;
  case 2:
    /* IRQ routing to core 2 */
    mmio_out32(LP_GPU_INTERRUPTS_ROUTING,
               LP_GPU_INTERRUPT_ROUTING_FIQ_CORE2
               | LP_GPU_INTERRUPT_ROUTING_IRQ_CORE2);
    /* route cor0cntv to core2 irq */
    mmio_out32(LP_CORE2_TIMERS_INTERRUPT_CTRL,
               LP_CORE_TIMERS_INTERRUPT_CTRL_CNTVIRQ_IRQ_CTRL);
    /* local timer interrupt goes to Core 2 IRQ */
    mmio_out32(LP_LOCAL_INTERRUPT_ROUTING,
               LP_LOCAL_INTERRUPT_ROUTING_CORE2_IRQ);
    break;
  case 3:
    /* IRQ routing to core 3 */
    mmio_out32(LP_GPU_INTERRUPTS_ROUTING,
               LP_GPU_INTERRUPT_ROUTING_FIQ_CORE3
               | LP_GPU_INTERRUPT_ROUTING_IRQ_CORE3);
    /* route cor0cntv to core3 irq */
    mmio_out32(LP_CORE3_TIMERS_INTERRUPT_CTRL,
               LP_CORE_TIMERS_INTERRUPT_CTRL_CNTVIRQ_IRQ_CTRL);
    /* local timer interrupt goes to Core 3 IRQ */
    mmio_out32(LP_LOCAL_INTERRUPT_ROUTING,
               LP_LOCAL_INTERRUPT_ROUTING_CORE3_IRQ);
    break;
  default:
    printk("Unknown CPU ID %lu\n", cpu);
    break;
  }
  enable_timer_interrupt();
}

