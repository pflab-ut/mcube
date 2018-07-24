/**
 * @file arch/arm/mm.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

int putchar(int c)
{
  uart_pol_putc(c, 0);
}

