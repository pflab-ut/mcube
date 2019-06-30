/**
 * @file arch/axis/stdio.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

int putchar(int c)
{
  asm volatile("mts $31, %0" :: "r"(c));
  return c;
}

void uart_putc(char c, uint8_t ch)
{
  if (ch != 0) {
    return;
  }

  putchar(c);
}
