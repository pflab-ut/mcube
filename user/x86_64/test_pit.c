/**
 * @file user/x86_64/test_pit.c
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
    pit_udelay(10 * 1000);
  }
}

/*
 * Test milli-second delays against wall-clock
 * Remember to have a stopwatch close by
 */
static void pit_test_udelay(void)
{
  printk("PIT: Testing timer delays\n\n");

  printk("Testing a 10-second delay after notice\n");
  pit_5secs_delay();

  printk("Note: Delay interval started \n");

  for (int i = 0; i < 1000; i++) {
    pit_udelay(10 * 1000);
  }

  printk("Note: Delay end \n\n");

  printk("Testing a 5-second delay after notice\n");
  pit_5secs_delay();

  printk("Note: Delay interval started \n");

  for (int i = 0; i < 5000; i++) {
    pit_udelay(1 * 1000);
  }

  printk("Note: Delay end \n\n");

  printk("Testing another 5-second delay after notice\n");
  pit_5secs_delay();

  printk("Note: Delay interval started \n");

  for (int i = 0; i < 100; i++) {
    pit_udelay(50 * 1000);
  }

  printk("Note: Delay end \n\n");
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
static void pit_test_periodic_irq(void)
{
  int i, us, vector;

  printk("PIT: Testing periodic interrupts\n\n");

  /* FIXME: We should have an IRQ model soon */
  vector = PIT_TESTS_VECTOR;
  set_intr_gate(vector, pit_periodic_handler);
  ioapic_setup_isairq(0, vector, IRQ_BOOTSTRAP);

  /* Testing showed that big delay values catches
   * more periodic timer accuracy errors .. */
  us = 50 * 1000;

  //  pit_monotonic(us);
  init_timer(us);
  start_timer();


  /* After each delay, store ticks triggered so far */
  enable_local_irq();

  for (i = 0; i < DELAY_TESTS; i++) {
    pit_udelay(us);
    ticks[i] = pit_ticks_count;
  }

  /* This should print a list of ones */
  printk("Number of ticks triggered on each delay period: ");

  for (i = 1; i < DELAY_TESTS; i++) {
    printk("%ld ", ticks[i] - ticks[i - 1]);
  }

  printk("\n\n");
}

bool test_pit(void)
{
  pit_test_periodic_irq();
  pit_test_udelay();
  return true;
}

