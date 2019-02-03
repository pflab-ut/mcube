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

void serial_putc(char c)
{
  putchar(c);
}
