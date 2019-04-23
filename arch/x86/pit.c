/**
 * @file arch/x86/pit.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * i8253/i8254-compatible PIT
 *
 * Copyright (C) 2009-2010 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * Check Intel's "82C54 CHMOS Programmable Interval Timer" datasheet
 * for more details.
 *
 * The PIT contains three timers (counters). Each timer is attached to a
 * GATE and an OUT pin, leading to the pins GATE-0, OUT-0, GATE-1, OUT-1,
 * and GATE-2, OUT-2.
 *
 * If GATE-x is positive, it enables the timer x to count, otherwise the
 * timer's count value stand still.
 *
 * In a PC compatible machine, GATE-0 and GATE-1 are always positive.
 * GATE-2 is controlled by Port 0x61 bit 0.
 *
 * OUT-x is the output pin of timer x. OUT-0 is traditionally mapped to
 * IRQ 0. OUT-1 was typically connected to DRAM refresh logic and is now
 * obsolete. OUT-2 is connected to port 0x61 bit 5.
 *
 * NOTE! Port 0x61 bit 1 is an input pin to a glue AND logic with the
 * OUT-2 pin, which leads to outputting counter 2 ticks to the system
 * speaker if set.
 *
 * Delays were tested against wall clock time.
 *
 * Finally, remember that for legacy hardware, it typically takes about
 * 1-us to access an I/O port.
 */

#include <mcube/mcube.h>

/*
 * Test PIT's monotonic mode code
 */

/*
 * Increase the counter for each periodic PIT tick.
 */
volatile int pit_ticks_count = 0;

void __pit_periodic_handler(void)
{
  pit_ticks_count++;
}

/*
 * Start counter 2: raise the GATE-2 pin.
 * Disable glue between OUT-2 and the speaker in the process
 */
static inline void timer2_start(void)
{
  uint8_t val;

  val = (inb(0x61) | PIT_GATE2) & ~PIT_SPEAKER;
  outb(val, 0x61);
}

/*
 * Freeze counter 2: clear the GATE-2 pin.
 */
static inline void timer2_stop(void)
{
  uint8_t val;

  val = inb(0x61) & ~PIT_GATE2;
  outb(val, 0x61);
}

/*
 * Set the given PIT counter with a count representing given
 * milliseconds value relative to the PIT clock rate.
 *
 * @counter_reg: PIT_COUNTER{0, 1, 2}
 *
 * Due to default oscillation frequency and the max counter
 * size of 16 bits, maximum delay is around 53 milliseconds.
 *
 * Countdown begins once counter is set and GATE-x is up.
 */
static void pit_set_counter(uint64_t us, int counter_reg)
{
  uint32_t counter;
  uint8_t counter_low, counter_high;

  /* counter = ticks per second * seconds to delay
   *         = PIT_CLOCK_RATE * (us / (1000 * 1000))
   *         = PIT_CLOCK_RATE / ((1000 * 1000) / us)
   * We use last form to avoid float arithmetic */
  assert(us > 0);
  assert(((1000 * 1000) / us) > 0);
  //  printk("us = %d\n", us);
  counter = PIT_CLOCK_RATE / ((1000 * 1000) / us);
  //  printk("counter = %u\n", counter);

  assert(counter <= UINT16_MAX);
  counter_low = counter & 0xff;
  counter_high = counter >> 8;

  outb(counter_low, counter_reg);
  outb(counter_high, counter_reg);
}

/*
 * Did we program PIT's counter0 to monotonic mode?
 */
static bool timer0_monotonic;

/*
 * Delay/busy-loop for @us microseconds.
 */
void pit_udelay(uint64_t us)
{
  union pit_cmd cmd = { .raw = 0 };

  /* GATE-2 down */
  timer2_stop();

  cmd.bcd = 0;
  cmd.mode = MODE_0;
  cmd.rw = RW_16bit;
  cmd.timer = 2;
  outb(cmd.raw, PIT_CONTROL);

  pit_set_counter(us, PIT_COUNTER2);

  /* GATE-2 up */
  timer2_start();

  while ((inb(0x61) & PIT_OUT2) == 0) {
    cpu_pause();
  }
}

/*
 * Trigger PIT IRQ pin (OUT-0) after given timeout
 */
void pit_oneshot(uint64_t us)
{
  union pit_cmd cmd = { .raw = 0 };

  /* No control over GATE-0: it's always positive */

  if (timer0_monotonic == true) {
    panic("PIT: Programming timer0 as one-shot will "
          "stop currently setup monotonic mode");
  }

  cmd.bcd = 0;
  cmd.mode = MODE_0;
  cmd.rw = RW_16bit;
  cmd.timer = 0;
  outb(cmd.raw, PIT_CONTROL);

  pit_set_counter(us, PIT_COUNTER0);
}

void init_pit_timer(unsigned long tick_us)
{
  pit_set_counter(tick_us, PIT_COUNTER0);
}

void start_pit_timer(void)
{
  union pit_cmd cmd = { .raw = 0 };
  /* No control over GATE-0: it's always positive */

  timer0_monotonic = true;

  cmd.bcd = 0;
  cmd.mode = MODE_2;
  cmd.rw = RW_16bit;
  cmd.timer = 0;
  outb(cmd.raw, PIT_CONTROL);
}

void stop_pit_timer(void)
{
  union pit_cmd cmd = { .raw = 0 };

  cmd.bcd = 0;
  cmd.mode = MODE_0;
  cmd.rw = RW_16bit;
  cmd.timer = 0;
  outb(cmd.raw, PIT_CONTROL);
}
