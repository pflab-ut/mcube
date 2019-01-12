/**
 * @file arch/axis/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_irq(void)
{
  int i;
  set_interrupt_vector_base_address((unsigned long) vector);

  for (i = 0; i < NR_SOFTWARE_INTERRUPTS; i++) {
    enable_software_interrupt(i);
  }
  register_callback_handler(do_sched_by_software_interrupt, 0);
  enable_previous_irq();
  enable_local_irq();
}

void exit_irq(void)
{
  disable_local_irq();
  disable_previous_irq();
}


void init_spinlock(void)
{
  sched_lock = INIT_SPINLOCK;
  global_malloc_lock = INIT_SPINLOCK;
}


/**
 * The init_arch() function initializes architecture-dependent features.
 */
void init_arch(void)
{
  init_spinlock();
  init_irq();
  init_cpu();
  init_malloc();
}

void init_arch_ap(void)
{  
}

/**
 * The exit_arch() function exits architecture-dependent features.
 */
void exit_arch(void)
{
  exit_irq();
}

void exit_arch_ap(void)
{
  exit_irq();
}
