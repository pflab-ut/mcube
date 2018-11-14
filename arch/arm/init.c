/**
 * @file arch/arm/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>



void shell(void)
{
  int c;
  while (1) {
    c = uart_getc(0);
    if (c == '\r') {
      uart_putc('\n', 0);
    } else {
      uart_putc(c, 0);
    }
  }
}


void init_arch(void)
{
  init_cpu();
  init_uart();
  init_timer(TICK_USEC);
  init_dmac();
  init_irq();
}

void init_arch_ap(void)
{
  init_timer(TICK_USEC);
  init_irq();
}

void exit_arch(void)
{
}

void exit_arch_ap(void)
{
}
