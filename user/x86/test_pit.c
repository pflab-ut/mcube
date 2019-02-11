/**
 * @file user/x86/test_pit.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Copyright (C) 2009-2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#include <mcube/mcube.h>


/*
 * Give the observer some time ..
 */
static void pit_5secs_delay(void)
{
  for (int i = 0; i < 500; i++) {
    pit_mdelay(10);
  }
}

/*
 * Test milli-second delays against wall-clock
 * Remember to have a stopwatch close by
 */
static void pit_test_mdelay(void)
{
  printk("PIT: Testing timer delays\n\n");

  printk("Testing a 10-second delay after notice\n");
  pit_5secs_delay();

  printk("Note: Delay interval started \n");
  for (int i = 0; i < 1000; i++) {
    pit_mdelay(10);
  }
  printk("Note: Delay end \n\n");

  printk("Testing a 5-second delay after notice\n");
  pit_5secs_delay();

  printk("Note: Delay interval started \n");
  for (int i = 0; i < 5000; i++) {
    pit_mdelay(1);
  }
  printk("Note: Delay end \n\n");

  printk("Testing another 5-second delay after notice\n");
  pit_5secs_delay();

  printk("Note: Delay interval started \n");
  for (int i = 0; i < 100; i++) {
    pit_mdelay(50);
  }
  printk("Note: Delay end \n\n");
}

/*
 * Test PIT's monotonic mode code
 */

/*
 * Increase the counter for each periodic PIT tick.
 */
static volatile int ticks_count;

void __pit_periodic_handler(void)
{
  ticks_count++;
}

/*
 * ticks[i]: number of periodic timer ticks triggered after
 * the `i'th independently-programmed delay interval.
 */
#define DELAY_TESTS  100
static uint64_t ticks[DELAY_TESTS];

/*
 * Test the periodic timer by checking the number of ticks
 * produced after each delay interval.
 *
 * Delay intervals code is assumed to be correct.
 */
extern void pit_periodic_handler(void);
static void pit_test_periodic_irq(void)
{
  int i, ms, vector;

  printk("PIT: Testing periodic interrupts\n\n");

  /* FIXME: We should have an IRQ model soon */
  vector = PIT_TESTS_VECTOR;
  set_intr_gate(vector, pit_periodic_handler);
  ioapic_setup_isairq(0, vector, IRQ_BOOTSTRAP);

  /* Testing showed that big delay values catches
   * more periodic timer accuracy errors .. */
  ms = 50;
  pit_monotonic(ms);

  /* After each delay, store ticks triggered so far */
  local_irq_enable();
  for (i = 0; i < DELAY_TESTS; i++) {
    pit_mdelay(ms);
    ticks[i] = ticks_count;
  }

  /* This should print a list of ones */
  printk("Number of ticks triggered on each delay period: ");
  for (i = 1; i < DELAY_TESTS; i++) {
    printk("%ld ", ticks[i] - ticks[i - 1]);
  }
  printk("\n\n");
}

void pit_run_tests(void)
{
  pit_test_periodic_irq();
  pit_test_mdelay();
}

