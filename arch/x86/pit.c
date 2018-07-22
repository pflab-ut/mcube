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

void isr_timer(const interrupt_context_t *context)
{
  printk("isr_timer()\n");
  (void) context;
  // Do nothing for now.
  
  // Send the end-of-interrupt signal.
  outb(PIC_PORT_CMD_MASTER, PIC_CMD_EOI);
}

void init_pit_timer(unsigned long tick_us)
{
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
  set_isr(TRAP_IRQ_TIMER, isr_timer);

  // Enable the timer interrupt (IRQ0).
  enable_irq(IRQ_TIMER);
}

void start_pit_timer(unsigned int ch)
{
  // Enable the timer interrupt (IRQ0).
  enable_irq(IRQ_TIMER);
}

void stop_pit_timer(unsigned int ch)
{
  // Disable the timer interrupt (IRQ0).
  disable_irq(IRQ_TIMER);
}
