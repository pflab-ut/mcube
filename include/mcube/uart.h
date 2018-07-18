/**
 * @file include/mcube/uart.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_UART_H__
#define	__MCUBE_MCUBE_UART_H__


#ifndef __ASSEMBLY__


/**
 * @brief Thread information
 *
 * The uart_devdata structure has UART device data.
 */
struct uart_devdata {
	/** Channel. */
	unsigned int ch;
	/** Baudrate. */
	unsigned int baudrate;
};

typedef struct uart_devdata uart_devdata;

extern volatile spinlock uart_getc_lock, uart_putc_lock;

uint8_t uart_getc(uint8_t port);
void uart_putc(char c, uint8_t port);
ssize_t uart_write(const void *input_data, size_t length, void *devdata);
ssize_t uart_read(void *output_data, size_t length, void *devdata);

uint8_t uart_pol_getc(uint8_t ch);
void uart_pol_putc(uint8_t c, int32_t ch);

void init_uart(void);

#endif

#endif	/* __MCUBE_MCUBE_UART_H__ */

