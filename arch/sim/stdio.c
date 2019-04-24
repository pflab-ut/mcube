/**
 * @file arch/sim/stdio.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void uart_putc(char c, __unused uint8_t ch)
{
  putchar(c);
}
