/**
 * @file drivers/uart/uart.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

volatile atomic_int uart_getc_lock = SPIN_UNLOCKED;
volatile atomic_int uart_putc_lock = SPIN_UNLOCKED;

struct uart_devdata uart_data[NR_UART_PORTS];


uint8_t uart_getc(uint8_t ch)
{
	uint8_t c;
	spin_lock(&uart_getc_lock);
	c = uart_pol_getc(ch);
	spin_unlock(&uart_getc_lock);
	return c;
}


void uart_putc(char c, uint8_t ch)
{
	spin_lock(&uart_putc_lock);
	if (c == '\n') {
		uart_pol_putc('\r', ch);
	}
	uart_pol_putc(c, ch);
	spin_unlock(&uart_putc_lock);
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
	return uart_pol_getc(((uart_devdata *) devdata)->ch);
}

