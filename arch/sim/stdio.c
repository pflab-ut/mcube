/**
 * @file arch/sim/stdio.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void serial_putc(char c)
{
  putchar(c);
}
