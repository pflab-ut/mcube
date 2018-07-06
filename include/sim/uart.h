/**
 * @file include/sim/uart.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_SIM_UART_H__
#define	__MCUBE_SIM_UART_H__

#ifndef __ASSEMBLY__

#define	STDIO_PORT	0


#define NR_UART_PORTS 1

extern struct dev_ops uart_ops;
extern struct uart_devdata uart_data[NR_UART_PORTS];

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_SIM_UART_H__ */
