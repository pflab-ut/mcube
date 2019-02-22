/**
 * @file include/mcube/uart.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_UART_H__
#define __MCUBE_MCUBE_UART_H__


#ifndef __ASSEMBLY__


/**
 * @struct uart_devdata
 * @brief UART information
 */
struct uart_devdata {
  /** Channel. */
  uint32_t ch;
  /** Baudrate. */
  uint32_t baudrate;
};

typedef struct uart_devdata uart_devdata;

extern volatile spinlock_t uart_getc_lock, uart_putc_lock;

uint8_t uart_getc(uint8_t port);
void uart_putc(char c, uint8_t port);
size_t uart_write(void *dev, const void *data, size_t length);
size_t uart_read(void *dev, void *data, size_t length);

uint8_t uart_pol_getc(uint32_t ch);
void uart_pol_putc(uint8_t c, uint32_t ch);

void init_uart(void);

#endif

#endif /* __MCUBE_MCUBE_UART_H__ */

