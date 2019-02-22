/**
 * @file drivers/uart/uart.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


struct uart_devdata uart_data[NR_UART_PORTS];


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

size_t uart_write(void *dev, const void *data, size_t length)
{
  size_t num;
  for (num = 0; num < length; num++) {
    uart_putc(((unsigned char *) data)[num], ((uart_devdata *) dev)->ch);
  }
        
  return num;
}

size_t uart_read(void *dev, __unused void *data, __unused size_t length)
{
  return uart_getc(((uart_devdata *) dev)->ch);
}

