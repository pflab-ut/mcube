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
  enable_previous_irq();
  enable_local_irq();
}


void exit_irq(void)
{
  disable_local_irq();
  disable_previous_irq();
}

int myputs(const char *s)
{
  const char *t = s;
  while (*t) {
    putchar(*t++);
  }
  return t - s;
}



/**
 * The init_arch() function initializes architecture-dependent features.
 */
void init_arch(void)
{  
  init_irq();
  init_cpu();
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
