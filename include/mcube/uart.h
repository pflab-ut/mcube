/**
 * @file include/mcube/uart.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_UART_H__
#define	__MCUBE_MCUBE_UART_H__


/* interrupt enable */
/* bit7-4 reserved */
#define UART_INT_ENABLE_MODEM (0x1 << 3)
#define UART_INT_ENABLE_LINE (0x1 << 2)
#define UART_INT_ENABLE_SEND (0x1 << 1)
#define UART_INT_ENABLE_RECV (0x1 << 0)

/* interrupt id */
#define UART_INT_ID_FIFO_MSB (0x1 << 7)
#define UART_INT_ID_FIFO_LSB (0x1 << 6)
/* bit5-4 reserved */
/* bit3-1 */
#define UART_INT_ID_RECV_LINE (0x3 << 1)
#define UART_INT_ID_RECV_DATA (0x2 << 1)
#define UART_INT_ID_RECV_FIFO_TRIGGER (0x2 << 1)
#define UART_INT_ID_RECV_FIFO_TIMEOUT (0x6 << 1)
#define UART_INT_ID_SEND_DATA_EMPTY (0x1 << 1)
#define UART_INT_ID_SEND_FIFO_EMPTY (0x1 << 1)
#define UART_INT_ID_MODEM (0x0 << 1)
#define UART_INT_ID_PENDING (0x1 << 0)

/* line control */
#define UART_LINE_CTRL_DLAB (0x1 << 7)
#define UART_LINE_CTRL_BREAK (0x1 << 6)
#define UART_LINE_CTRL_FIX_PARITY (0x1 << 5)
#define UART_LINE_CTRL_ODD_EVEN_PARITY (0x1 << 4)
#define UART_LINE_CTRL_SEND_PARITY (0x1 << 3)
#define UART_LINE_CTRL_STOP_BIT_LEN_2 (0x1 << 2)
/* bit1-0 */
#define UART_LINE_CTRL_SEND_RECV_DATA_8BIT (0x3 << 0)
#define UART_LINE_CTRL_SEND_RECV_DATA_7BIT (0x2 << 0)
#define UART_LINE_CTRL_SEND_RECV_DATA_6BIT (0x1 << 0)
#define UART_LINE_CTRL_SEND_RECV_DATA_5BIT (0x0 << 0)

/* modem control */
/* bit7-5 reserved */
#define UART_MODEM_CTRL_LOOP_BACK (0x1 << 4)
#define UART_MODEM_CTRL_INT_ENABLE (0x1 << 3)
#define UART_MODEM_CTRL_LOOP_BACK_OUT (0x1 << 2)
#define UART_MODEM_CTRL_ASSERT_RTS (0x1 << 1)
#define UART_MODEM_CTRL_ASSERT_DTR (0x1 << 0)

/* line status */
#define UART_LINE_STATUS_RECV_FIFO (0x1 << 7)
#define UART_LINE_STATUS_SEND_DATA_COMP (0x1 << 6)
#define UART_LINE_STATUS_SEND_DATA_EMPTY (0x1 << 5)
#define UART_LINE_STATUS_BREAK_RECV (0x1 << 4)
#define UART_LINE_STATUS_FRAMING_ERROR (0x1 << 3)
#define UART_LINE_STATUS_PARITY_ERROR (0x1 << 2)
#define UART_LINE_STATUS_OVERRUN_ERROR (0x1 << 1)
#define UART_LINE_STATUS_RECV_DATA (0x1 << 0)

/* modem status */
#define UART_MODEM_STATUS_ASSERT_DCD (0x1 << 7)
#define UART_MODEM_STATUS_ASSERT_RI (0x1 << 6)
#define UART_MODEM_STATUS_ASSERT_DSR (0x1 << 5)
#define UART_MODEM_STATUS_ASSERT_CTS (0x1 << 4)
#define UART_MODEM_STATUS_CHANGE_DCD (0x1 << 3)
#define UART_MODEM_STATUS_NEGATE_RI (0x1 << 2)
#define UART_MODEM_STATUS_CHANGE_DSR (0x1 << 1)
#define UART_MODEM_STATUS_CHANGE_CTS (0x1 << 0)


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

extern uint8_t uart_getc(uint8_t port);
extern void uart_putc(char c, uint8_t port);
extern ssize_t uart_write(const void *input_data, size_t length, void *devdata);
extern ssize_t uart_read(void *output_data, size_t length, void *devdata);

extern uint8_t uart_pol_getc(uint8_t ch);
extern void uart_pol_putc(uint8_t c, int32_t ch);

extern void init_uart(void);

#endif

#endif	/* __MCUBE_MCUBE_UART_H__ */

