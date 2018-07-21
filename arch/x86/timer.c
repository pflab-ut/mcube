/**
 * @file arch/x86/timer.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

unsigned long cpu_bus_freq_mhz = 0;
unsigned long sys_tsc = 0;

unsigned long timer_count[NR_INTRA_KERNEL_CPUS];
unsigned long tcur[NR_INTRA_KERNEL_CPUS], tprev[NR_INTRA_KERNEL_CPUS];

#define LAPIC_TIMER 0
#define HPET_TIMER 0
#define PIT_TIMER 1

// 8253 timer ports
#define TIMER_PORT_DATA_CH0  0x40   ///< Channel 0 data port.
#define TIMER_PORT_DATA_CH1  0x41   ///< Channel 1 data port.
#define TIMER_PORT_DATA_CH2  0x42   ///< Channel 2 data port.
#define TIMER_PORT_CMD       0x43   ///< Timer command port.

// Frequency bounds
#define MIN_FREQUENCY        19
#define MAX_FREQUENCY        1193181

void isr_timer(const interrupt_context_t *context)
{
  printk("isr_timer()\n");
  (void) context;
  // Do nothing for now.
  
  // Send the end-of-interrupt signal.
  outb(PIC_PORT_CMD_MASTER, PIC_CMD_EOI);
}

void timer_init(uint32_t frequency)
{
  // Clamp frequency to allowable range.
  if (frequency < MIN_FREQUENCY) {
    frequency = MIN_FREQUENCY;
  } else if (frequency > MAX_FREQUENCY) {
    frequency = MAX_FREQUENCY;
  }

  // Compute the clock count value.
  uint16_t count = (uint16_t)(MAX_FREQUENCY / frequency);

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

void timer_enable(void)
{
  // Enable the timer interrupt (IRQ0).
  enable_irq(IRQ_TIMER);
}

void timer_disable(void)
{
  // Disable the timer interrupt (IRQ0).
  disable_irq(IRQ_TIMER);
}

                         
static void measure_cpu_frequency(void)
{
}


void init_timer(unsigned long tick_us)
{
#ifdef LAPIC_TIMER
  init_lapic_timer(tick_us);
#elif HPET_TIMER
  init_hpet_timer(tick_us);
#elif PIT_TIMER
  init_pit_timer(tick_us);
#endif /* LAPIC_TIMER */
  measure_cpu_frequency();
}

void start_timer(unsigned int ch)
{
#ifdef LAPIC_TIMER
  start_lapic_timer(ch);
#elif HPET_TIMER
  start_hpet_timer(ch);
#elif PIT_TIMER
  start_pit_timer(ch);
#endif /* LAPIC_TIMER */
}

void stop_timer(unsigned int ch)
{
#ifdef LAPIC_TIMER
  stop_lapic_timer(ch);
#elif HPET_TIMER
  stop_hpet_timer(ch);
#elif PIT_TIMER
  stop_pit_timer(ch);
#endif /* LAPIC_TIMER */
}


