/**
 * @file arch/x86/pit.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

// 8253 timer ports
#define TIMER_PORT_DATA_CH0  0x40   ///< Channel 0 data port.
#define TIMER_PORT_DATA_CH1  0x41   ///< Channel 1 data port.
#define TIMER_PORT_DATA_CH2  0x42   ///< Channel 2 data port.
#define TIMER_PORT_CMD       0x43   ///< Timer command port.


void handle_pit_timer(interrupt_context_t *context)
{
  unsigned long cpu = get_cpu_id();
  static int i = 0;
  i++;
  if (i % 1000 == 0) {
    printk("handle_pit_timer()\n");
    
    if (current_th[cpu] != &kernel_th[cpu]) {
      PDEBUG("current_th: id = %lu sched.remaining = %ld\n",
             current_th[cpu]->id, current_th[cpu]->sched.remaining);
      current_th[cpu]->sched.remaining -=
        CPU_CLOCK_TO_USEC(get_current_cpu_time() - current_th[cpu]->sched.begin_cpu_time);
      if (current_th[cpu]->sched.remaining <= 0) {
        do_end_job(current_th[cpu]);
      }
    }
    update_jiffies();
    if (sched_time <= sys_jiffies) {
      //    printk("handle_LAPIC_timer_tick(): sched_end: cpu = %lu\n", cpu);
      sched_end = TRUE;
      current_th[cpu] = &kernel_th[cpu];
      stop_pit_timer(0);
      outb(PIC_PORT_CMD_MASTER, PIC_CMD_EOI);
      return;
    } else {
      do_release();
      do_sched();
    }
    do_switch_thread_arch(context);
  }
  
  // Send the end-of-interrupt signal.
  outb(PIC_PORT_CMD_MASTER, PIC_CMD_EOI);
}

void init_pit_timer(unsigned long tick_us)
{
  printk("init_pit_timer()\n");
  // Compute the clock count value.
	/* upper bound of count is 65535. */
	/* 11932 = 100Hz = 10ms */
	/* 1193 = 1KHz = 1ms */
  //  uint16_t count = (uint16_t)(MAX_FREQUENCY / frequency);
  uint16_t count = 1193;

  // Channel=0, AccessMode=lo/hi, OperatingMode=rate-generator
  outb(TIMER_PORT_CMD, 0x34);

  // Output the lo/hi count value
  outb(TIMER_PORT_DATA_CH0, (uint8_t)count);
  outb(TIMER_PORT_DATA_CH0, (uint8_t)(count >> 8));

  // Assign the interrupt service routine.
  set_isr(TRAP_PIT_IRQ, handle_pit_timer);

  // Enable the timer interrupt (IRQ0).
  enable_irq(PIT_IRQ);
}

void start_pit_timer(unsigned int ch)
{
  // Enable the timer interrupt (IRQ0).
  enable_irq(PIT_IRQ);
}

void stop_pit_timer(unsigned int ch)
{
  // Disable the timer interrupt (IRQ0).
  disable_irq(PIT_IRQ);
}
