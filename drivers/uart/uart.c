/**
 * @file drivers/uart/uart.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


uint8_t uart_getc(uint8_t ch)
{
  uint8_t c;
  c = uart_pol_getc(ch);
  return c;
}


void uart_putc(char c, uint8_t ch)
{
  if (c == '\n') {
    uart_pol_putc('\r', ch);
  }

  uart_pol_putc(c, ch);
}

size_t uart_write(const char *data, size_t length, uint8_t ch)
{
  size_t num;

  for (num = 0; num < length; num++) {
    uart_putc(data[num], ch);
  }

  return num;
}

size_t uart_read(size_t length, uint8_t ch)
{
  __uninitialized(length);
  return uart_getc(ch);
}

