/**
 * @file include/x86/uart.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_UART_H__
#define __MCUBE_X86_UART_H__


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


/* 1.8432MHz */
#define UART_CLK 1843200

#define  STDIO_PORT 0
#define NR_UART_PORTS 2

#define COM1_START 0x03f8
#define COM1_RECV_DATA COM1_START
#define COM1_SEND_DATA COM1_START
#define COM1_BAUDRATE_LSB COM1_START
#define COM1_BAUDRATE_MSB (COM1_START + 0x1)
#define COM1_INT_ENABLE (COM1_START + 0x1)
#define COM1_INT_ID (COM1_START + 0x2)
#define COM1_FIFO_CTRL (COM1_START + 0x2)
#define COM1_LINE_CTRL (COM1_START + 0x3)
#define COM1_MODEM_CTRL (COM1_START + 0x4)
#define COM1_LINE_STATUS (COM1_START + 0x5)
#define COM1_MODEM_STATUS (COM1_START + 0x6)
#define COM1_SCRATCH (COM1_START + 0x7)

#define COM2_START 0x02f8
#define COM2_RECV_DATA COM2_START
#define COM2_SEND_DATA COM2_START
#define COM2_BAUDRATE_LSB COM2_START
#define COM2_BAUDRATE_MSB (COM2_START + 0x1)
#define COM2_INT_ENABLE (COM2_START + 0x1)
#define COM2_INT_ID (COM2_START + 0x2)
#define COM2_FIFO_CTRL (COM2_START + 0x2)
#define COM2_LINE_CTRL (COM2_START + 0x3)
#define COM2_MODEM_CTRL (COM2_START + 0x4)
#define COM2_LINE_STATUS (COM2_START + 0x5)
#define COM2_MODEM_STATUS (COM2_START + 0x6)
#define COM2_SCRATCH (COM2_START + 0x7)


#define BAUDRATE_38400_MSB 0x0
#define BAUDRATE_38400_LSB 0x3

#define BAUDRATE_19200_MSB 0x0
#define BAUDRATE_19200_LSB 0x6

#define BAUDRATE_LSB(speed) ((UART_CLK / 16 / (speed)) & 0xff)
#define BAUDRATE_MSB(speed) (((UART_CLK / 16 / (speed)) & 0xff00) >> 8)


extern struct dev_ops uart_ops;
extern struct uart_devdata uart_data[NR_UART_PORTS];

#endif /* !__ASSEMBLY__ */



#endif /* __MCUBE_X86_UART_H__ */
