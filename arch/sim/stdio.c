/**
 * @file arch/sim/stdio.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

ssize_t uart_write(const void *input_data, size_t length, void *devdata)
{
  ssize_t num;
  for (num = 0; num < length; num++) {
    putchar(((unsigned char *) input_data)[num]);
  }
  return num;
}

void serial_putc(char c)
{
  putchar(c);
}
