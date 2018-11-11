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
  init_irq();
  init_timer(TICK_USEC);
  init_mm();
}

void exit_arch(void)
{
}
