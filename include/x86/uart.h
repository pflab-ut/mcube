/**
 * @file include/x86/uart.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_X86_UART_H__
#define	__MCUBE_X86_UART_H__

#ifndef __ASSEMBLY__


/* 1.8432MHz */
#define UART_CLK 1843200

#define	STDIO_PORT 0
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

#endif	/* __MCUBE_X86_UART_H__ */
