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

ssize_t uart_write(const void *input_data, size_t length, void *devdata)
{
  ssize_t num;
  for (num = 0; num < length; num++) {
    uart_putc(((unsigned char *) input_data)[num], ((uart_devdata *) devdata)->ch);
  }
        
  return num;
}

ssize_t uart_read(void *output_data, size_t length, void *devdata)
{
  return uart_getc(((uart_devdata *) devdata)->ch);
}

