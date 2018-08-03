/**
 * @file include/arm/uart.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_ARM_UART_H__
#define	__MCUBE_ARM_UART_H__

#define PL011_UART 1
#define MINI_UART 0


#if CONFIG_ARCH_ARM_SYNQUACER && MINI_UART
#error "Synquacer does not have Mini UART."
#endif /* CONFIG_ARCH_ARM_SYNQUACER && MINI_UART */

/* PL011 UART and Mini UART */
#define NR_UART_PORTS 2

  
// The offsets for reach register for the UART.
#define UART0_DATA_REG (UART0_BASE + 0x00)
#define UART0_RECEIVE_STATUS_REG_ERROR_CLEAR_REG (UART0_BASE + 0x04)
#define UART0_FLAG_REG (UART0_BASE + 0x18)
#define UART0_ILPR_REG (UART0_BASE + 0x20) /* not in use */
#define UART0_INTEGER_BAUD_RATE_DIVISOR (UART0_BASE + 0x24)
#define UART0_FRACTIONAL_BAUD_RATE_DIVISOR (UART0_BASE + 0x28)
#define UART0_LINE_CTRL_REG (UART0_BASE + 0x2c)
#define UART0_CTRL_REG (UART0_BASE + 0x30)
#define UART0_INTERRUPT_FIFO_LEVEL_SELECT_REG (UART0_BASE + 0x34)
#define UART0_INTERRUPT_MASK_SET_CLEAR_REG (UART0_BASE + 0x38)
#define UART0_RAW_INTERRUPT_STATUS_REG (UART0_BASE + 0x3c)
#define UART0_MASKED_INTERRUPT_STATUS_REG (UART0_BASE + 0x40)
#define UART0_INTERRUPT_CLEAR_REG (UART0_BASE + 0x44)
#define UART0_DMA_CTRL_REG (UART0_BASE + 0x48)
#define UART0_INTEGRATION_TEST_CTRL_REG (UART0_BASE + 0x80)
#define UART0_INTEGRATION_TEST_INPUT_REG (UART0_BASE + 0x84)
#define UART0_INTEGRATION_TEST_OUTPUT_REG (UART0_BASE + 0x88)
#define UART0_TEST_DATA_REG (UART0_BASE + 0x8c)

/* UART Data Register */
/* 31:12 reserved */
#define UART_DATA_REG_OVERRUN_ERROR (0x1 << 11)
#define UART_DATA_REG_BREAK_ERROR (0x1 << 10)
#define UART_DATA_REG_PARITY_ERROR (0x1 << 9)
#define UART_DATA_REG_FRAMING_ERROR (0x1 << 8)
#define UART_DATA_REG_DATA_MASK 0xff

/* UART Receive Status Register/Error Clear Register */
/* 31:4 reserved */
#define UART_RSRECR_OVERRUN_ERROR (0x1 << 3)
#define UART_RSRECR_BREAK_ERROR (0x1 << 2)
#define UART_RSRECR_PARITY_ERROR (0x1 << 1)
#define UART_RSRECR_FRAMING_ERROR (0x1 << 0)

/* UART Flag Register */
/* 31:9 reserved */
#define UART_FLAG_REG_RI (0x1 << 8) /* Unsupported */
#define UART_FLAG_REG_TRANSMIT_FIFO_EMPTY (0x1 << 7)
#define UART_FLAG_REG_RECEIVE_FIFO_FULL (0x1 << 6)
#define UART_FLAG_REG_TRANSMIT_FIFO_FULL (0x1 << 5)
#define UART_FLAG_REG_RECEIVE_FIFO_EMPTY (0x1 << 4)
#define UART_FLAG_REG_UART_BUSY (0x1 << 3)
#define UART_FLAG_REG_DCD (0x1 << 2) /* Unsupported */
#define UART_FLAG_REG_DSR (0x1 << 1) /* Unsupported */
#define UART_FLAG_REG_CLEAR_TO_SEND (0x1 << 0)

/* UART ILPR Register */
/* 31:0 reserved */

/* UART Integer Baud Rate Divisor */
/* 31:16 reserved */
#define UART_INTEGER_BAUD_RATE_DIVISOR_MASK 0xffff

/* UART Fractional Baud Rate Divisor */
/* 31:6 reserved */
#define UART_FRACTIONAL_BAUD_RATE_DIVISOR_MASK 0x3f

/* UART Line Control Register */
/* 31:8 reserved */
#define UART_LINE_CTRL_REG_STICK_PARITY_SELECT (0x1 << 7)
#define UART_LINE_CTRL_REG_WORD_LENGTH (0x3 << 5)
#define UART_LINE_CTRL_REG_ENABLE_FIFO (0x1 << 4)
#define UART_LINE_CTRL_REG_TWO_STOP_BITS_SELECT (0x1 << 3)
#define UART_LINE_CTRL_REG_EVEN_PARITY_SELECT (0x1 << 2)
#define UART_LINE_CTRL_REG_PARITY_ENABLE (0x1 << 1)
#define UART_LINE_CTRL_REG_SEND_BREAK (0x1 << 0)

/* UART Control Register */
/* 31:16 reserved */
#define UART_CTRL_REG_CTS_HARDWARE_FLOW_CTRL_ENABLE (0x1 << 15)
#define UART_CTRL_REG_RTS_HARDWARE_FLOW_CTRL_ENABLE (0x1 << 14)
#define UART_CTRL_REG_OUT2 (0x1 << 13) /* Unsupported */
#define UART_CTRL_REG_OUT1 (0x1 << 12) /* Unsupported */
#define UART_CTRL_REG_REQUEST_TO_SEND (0x1 << 11)
#define UART_CTRL_REG_DTR (0x1 << 10) /* Unsupported */
#define UART_CTRL_REG_RECEIVE_ENABLE (0x1 << 9)
#define UART_CTRL_REG_TRANSMIT_ENABLE (0x1 << 8)
#define UART_CTRL_REG_LOOPBACK_ENABLE (0x1 << 7)
/* 6:3 reserved */
#define UART_CTRL_REG_SIRLP (0x1 << 2) /* Unsupported */
#define UART_CTRL_REG_SIREN (0x1 << 1) /* Unsupported */
#define UART_CTRL_REG_UART_ENABLE (0x1 << 0)

/* UART Interrupt FIFO Level Select Register */
/* 31:12 reserved */
#define UART_INTERRUPT_FIFO_LEVEL_SELECT_REG_RXIFPSEL (0x7 << 9)
#define UART_INTERRUPT_FIFO_LEVEL_SELECT_REG_TXIFPSEL (0x7 << 6)
#define UART_INTERRUPT_FIFO_LEVEL_SELECT_REG_RXIFLSEL (0x7 << 3)
#define UART_INTERRUPT_FIFO_LEVEL_SELECT_REG_TXIFLSEL (0x7 << 0)

/* UART Interrupt Mask Set Clear Register */
/* 31:11 reserved */
#define UART_IMSC_REG_OVERRUN_ERROR_INTERRUPT_MASK (0x1 << 10)
#define UART_IMSC_REG_BREAK_ERROR_INTERRUPT_MASK (0x1 << 9)
#define UART_IMSC_REG_PARITY_ERROR_INTERRUPT_MASK (0x1 << 8)
#define UART_IMSC_REG_FRAMING_ERROR_INTERRUPT_MASK (0x1 << 7)
#define UART_IMSC_REG_RECEIVE_TIMEOUT_INTERRUPT_MASK (0x1 << 6)
#define UART_IMSC_REG_TRANSMIT_INTERRUPT_MASK (0x1 << 5)
#define UART_IMSC_REG_RECEIVE_INTERRUPT_MASK (0x1 << 4)
#define UART_IMSC_REG_DSRMIM (0x1 << 3) /* Unsupported */
#define UART_IMSC_REG_DCDMIM (0x1 << 2) /* Unsupported */
#define UART_IMSC_REG_CTSMIM (0x1 << 1)
#define UART_IMSC_REG_RIMM (0x1 << 0) /* Unsupported */


/* UART Raw Interrupt Status Register */
/* 31:11 reserved */
#define UART_RIS_REG_OVERRUN_ERROR_INTERRUPT_STATUS (0x1 << 10)
#define UART_RIS_REG_BREAK_ERROR_INTERRUPT_STATUS (0x1 << 9)
#define UART_RIS_REG_PARITY_ERROR_INTERRUPT_STATUS (0x1 << 8)
#define UART_RIS_REG_FRAMING_ERROR_INTERRUPT_STATUS (0x1 << 7)
#define UART_RIS_REG_RECEIVE_TIMEOUT_INTERRUPT_STATUS (0x1 << 6)
#define UART_RIS_REG_TRANSMIT_INTERRUPT_STATUS (0x1 << 5)
#define UART_RIS_REG_RECEIVE_INTERRUPT_STATUS (0x1 << 4)
#define UART_RIS_REG_DSRRMIS (0x1 << 3) /* Unsupported */
#define UART_RIS_REG_DCDRMIS (0x1 << 2) /* Unsupported */
#define UART_RIS_REG_CTSRMIS (0x1 << 1)
#define UART_RIS_REG_RIRMIS (0x1 << 0) /* Unsupported */

/* UART Masked Interrupt Status Register */
/* 31:11 reserved */
#define UART_MIS_REG_OVERRUN_ERROR_MASKED_INTERRUPT_STATUS (0x1 << 10)
#define UART_MIS_REG_BREAK_ERROR_MASKED_INTERRUPT_STATUS (0x1 << 9)
#define UART_MIS_REG_PARITY_ERROR_MASKED_INTERRUPT_STATUS (0x1 << 8)
#define UART_MIS_REG_FRAMING_ERROR_MASKED_INTERRUPT_STATUS (0x1 << 7)
#define UART_MIS_REG_RECEIVE_TIMEOUT_MASKED_INTERRUPT_STATUS (0x1 << 6)
#define UART_MIS_REG_TRANSMIT_MASKED_INTERRUPT_STATUS (0x1 << 5)
#define UART_MIS_REG_RECEIVE_MASKED_INTERRUPT_STATUS (0x1 << 4)
#define UART_MIS_REG_DSRMMIS (0x1 << 3) /* Unsupported */
#define UART_MIS_REG_DCDMMIS (0x1 << 2) /* Unsupported */
#define UART_MIS_REG_CTSMMIS (0x1 << 1)
#define UART_MIS_REG_RIMMIS (0x1 << 0) /* Unsupported */

/* UART Interrupt Clear Register */
/* 31:11 reserved */
#define UART_ICR_REG_OVERRUN_ERROR_INTERRUPT_CLEAR (0x1 << 10)
#define UART_ICR_REG_BREAK_ERROR_INTERRUPT_CLEAR (0x1 << 9)
#define UART_ICR_REG_PARITY_ERORR_INTERRUPT_CLEAR (0x1 << 8)
#define UART_ICR_REG_FRAMING_ERROR_INTERRUPT_CLEAR (0x1 << 7)
#define UART_ICR_REG_RECEIVE_TIMEOUT_INTERRUPT_CLEAR (0x1 << 6)
#define UART_ICR_REG_TRANSMIT_INTERRUPT_CLEAR (0x1 << 5)
#define UART_ICR_REG_RECEIVE_MASKED_INTERRUPT_STATUS (0x1 << 4)
#define UART_ICR_REG_DSRMIC (0x1 << 3) /* Unsupported */
#define UART_ICR_REG_DCDMIC (0x1 << 2) /* Unsupported */
#define UART_ICR_REG_CTSMIC (0x1 << 1)
#define UART_ICR_REG_RIMIC (0x1 << 0) /* Unsupported */

/* UART DMA Control Register */
/* 31:3 reserved */
#define UART_DMA_CTRL_REG_DMAONERR (0x1 << 2) /* Unsupported */
#define UART_DMA_CTRL_REG_TXDMAE (0x1 << 1) /* Unsupported */
#define UART_DMA_CTRL_REG_RSDMAE (0x1 << 0) /* Unsupported */

/* UART Integration Test Control Register */
/* 31:2 reserved */
#define UART_ITCR_REG_INTEGRATION_TEST_FIFO_ENABLE (0x1 << 1)
#define UART_ITCR_REG_INTEGRATION_TEST_ENABLE (0x1 << 0)

/* UART Integration Test Input Register */
/* 31:4 reserved */
#define UART_ITIP_REG_UARTCTS_PRIMARY_INPUT (0x1 << 3)
/* 2:1 reserved */
#define UART_ITIP_REG_UARTRXD_PRIMARY_INPUT (0x1 << 0)

/* UART Integration Test Output Register */
/* 31:12 reserved */
#define UART_ITOP_UARTMSINTR (0x1 << 11)
#define UART_ITOP_UARTRXINTR (0x1 << 10)
#define UART_ITOP_UARTTXINTR (0x1 << 9)
#define UART_ITOP_UARTRTINTR (0x1 << 8)
#define UART_ITOP_UARTEINTR (0x1 << 7)
#define UART_ITOP_UARTINTR (0x1 << 6)
/* 5:4 reserved */
#define UART_ITOP_UARTRTS (0x1 << 3)
/* 2:1 reserved */
#define UART_ITOP_UARTTXD (0x1 << 0)

/* UART Test Data Register */
/* 31:11 reserved */
#define UART_TEST_DATA_REG_MASK 0x3ff


#ifndef __ASSEMBLY__


extern struct uart_devdata uart_data[NR_UART_PORTS];

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_ARM_UART_H__ */
