/**
 * @file arch/sim/stdio.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void uart_putc(char c, uint8_t ch)
{
  if (ch != 0) {
    return;
  }

  putchar(c);
}
