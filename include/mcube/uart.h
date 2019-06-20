/**
 * @file include/mcube/uart.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_UART_H__
#define __MCUBE_MCUBE_UART_H__


#ifndef __ASSEMBLY__

/**
 * @var uart_getc_lock
 * @brief Lock for uart_getc().
 */
extern volatile spinlock_t uart_getc_lock;

/**
 * @var uart_putc_lock
 * @brief Lock for uart_putc().
 */
extern volatile spinlock_t uart_putc_lock;

/**
 * @fn uint8_t uart_getc(uint8_t ch)
 * @brief getc() for UART.
 *
 * @param ch Channel.
 * @return Character.
 */
uint8_t uart_getc(uint8_t ch);

/**
 * @fn void uart_putc(char c, uint8_t ch)
 * @brief putc() for UART.
 *
 * @param c Character.
 * @param ch Channel.
 */
void uart_putc(char c, uint8_t ch);

/**
 * @fn size_t uart_write(const char *data, size_t length, uint8_t ch)
 * @brief write() for UART.
 *
 * @param data Data.
 * @param length Length.
 * @param ch Channel.
 */
size_t uart_write(const char *data, size_t length, uint8_t ch);

/**
 * @fn size_t uart_read(size_t length, uint8_t ch)
 * @brief read() for UART.
 *
 * @param length Length.
 * @param ch Channel.
 * @return Character.
 */
size_t uart_read(size_t length, uint8_t ch);

/**
 * @fn uint8_t uart_pol_getc(uint32_t ch)
 * @brief getc() for UART.
 *
 * @param ch Channel.
 * @return Character.
 */
uint8_t uart_pol_getc(uint32_t ch);

/**
 * @fn void uart_pol_putc(uint8_t c, uint32_t ch)
 * @brief putc() for UART.
 *
 * @param c Character.
 * @param ch Channel.
 */
void uart_pol_putc(uint8_t c, uint32_t ch);

/**
 * @fn void init_uart(void)
 * @brief initialize UART.
 */
void init_uart(void);

#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_MCUBE_UART_H__ */

