/**
 * @file include/mcube/uart.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_UART_H__
#define __MCUBE_MCUBE_UART_H__


#ifndef __ASSEMBLY__

extern volatile spinlock_t uart_getc_lock, uart_putc_lock;

uint8_t uart_getc(uint8_t ch);
void uart_putc(char c, uint8_t ch);
size_t uart_write(const char *data, size_t length, uint8_t ch);
size_t uart_read(size_t length, uint8_t ch);

uint8_t uart_pol_getc(uint32_t ch);
void uart_pol_putc(uint8_t c, uint32_t ch);

void init_uart(void);

#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_MCUBE_UART_H__ */

