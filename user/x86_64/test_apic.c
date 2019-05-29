/**
 * @file user/x86_64/test_apic.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Local APIC configuration
 *
 * Copyright (C) 2009-2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#include <mcube/mcube.h>

/*
 * Give the observer some time ..
 */
static void apic_5secs_delay(void)
{
  for (int i = 0; i < 500; i++) {
    apic_udelay(10 * 1000);
  }
}

/*
 * Test APIC delays against wall-clock time
 * Remember to have a stopwatch close by
 */
static void apic_test_delay(void)
{
  printk("APIC: Testing timer delays\n\n");

  printk("Testing a 10-second delay after notice\n");
  apic_5secs_delay();

  printk("Note: Delay interval started \n");

  for (int i = 0; i < 1000; i++) {
    apic_udelay(10 * 1000);
  }

  printk("Note: Delay end \n\n");

  printk("Testing a 10-second delay using u-seconds\n");
  apic_5secs_delay();

  printk("Note: Delay interval started \n");

  for (int i = 0; i < 100000; i++) {
    apic_udelay(100);
  }

  printk("Note: Delay end \n\n");

  printk("Testing a 5-second delay after notice\n");
  apic_5secs_delay();

  printk("Note: Delay interval started \n");

  for (int i = 0; i < 5000; i++) {
    apic_udelay(1 * 1000);
  }

  printk("Note: Delay end \n\n");

  printk("Testing another 5-second delay after notice\n");
  apic_5secs_delay();

  printk("Note: Delay interval started \n");

  for (int i = 0; i < 5; i++) {
    apic_udelay(1000 * 1000);
  }

  printk("Note: Delay end \n\n");
}

/*
 * Test APIC timer periodic ticks against PIT delays
 */



/*
 * ticks[i]: number of periodic timer ticks triggered after
 * the `i'th delay interval.
 */
#define DELAY_TESTS  100
static uint64_t ticks[DELAY_TESTS];

/*
 * Test the APIC periodic timer against equivalent-time
 * PIT-programmed delays.
 */
static void apic_test_periodic_mode(void)
{
  int i, us, vector;

  printk("APIC: Testing periodic interrupts\n\n");

  /* FIXME: We should have an IRQ model soon */
  vector = APIC_TESTS_VECTOR;
  set_intr_gate(vector, apic_timer_handler);

  /* Testing showed that big delay values catches
   * more periodic timer accuracy errors .. */
  us = 50 * 1000;
  //  apic_monotonic(us, vector);
  init_apic_timer(us, vector);
  start_timer();

  /* After each delay, store ticks triggered so far */
  enable_local_irq();

  for (i = 0; i < DELAY_TESTS; i++) {
    pit_udelay(us);
    ticks[i] = apic_ticks_count;
  }

  /* This should print a list of ones */
  printk("Number of ticks triggered on each delay period: ");

  for (i = 1; i < DELAY_TESTS; i++) {
    printk("%ld ", ticks[i] - ticks[i - 1]);
  }

  printk("\n\n");
}

bool test_apic(void)
{
  apic_test_periodic_mode();
  apic_test_delay();
  return true;
}

