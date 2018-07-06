/**
 * @file arch/axis/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_irq(void)
{
  int i;
  set_interrupt_vector_base_address((uint32_t) common_interrupt);

  for (i = 0; i < NR_SOFTWARE_INTERRUPTS; i++) {
    enable_software_interrupt(i);
  }
  enable_previous_interrupt();
  enable_interrupt();
}


void exit_irq(void)
{
  disable_interrupt();
  disable_previous_interrupt();
}

/**
 * The init_arch() function initializes architecture-dependent features.
 */
void init_arch(void)
{
  init_irq();
}

/**
 * The exit_arch() function exits architecture-dependent features.
 */
void exit_arch(void)
{
  exit_irq();
}
