/**
 * @file include/arm/uart.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_UART_H__
#define __MCUBE_ARM_UART_H__



#if CONFIG_ARCH_ARM_SYNQUACER && MINI_UART
#error "Synquacer does not have Mini UART."
#endif /* CONFIG_ARCH_ARM_SYNQUACER && MINI_UART */

/* PL011 UART and Mini UART */
#define NR_UART_PORTS 2


#define PL011_UART_HZ (4 * 1000 * 1000)


// The offsets for reach register for the UART.
#define UART0_DATA_REG (UART0_BASE + 0x00)
#define UART0_RSRECR_REG (UART0_BASE + 0x04)
#define UART0_FLAG_REG (UART0_BASE + 0x18)
#define UART0_ILPR_REG (UART0_BASE + 0x20) /* not in use */
#define UART0_IBRD_REG (UART0_BASE + 0x24)
#define UART0_FBRD_REG (UART0_BASE + 0x28)
#define UART0_LCRH_REG (UART0_BASE + 0x2c)
#define UART0_CTRL_REG (UART0_BASE + 0x30)
#define UART0_IFLS_REG (UART0_BASE + 0x34)
#define UART0_IMSC_REG (UART0_BASE + 0x38)
#define UART0_RIS_REG (UART0_BASE + 0x3c)
#define UART0_MIS_REG (UART0_BASE + 0x40)
#define UART0_ICR_REG (UART0_BASE + 0x44)
#define UART0_DMA_CTRL_REG (UART0_BASE + 0x48)
#define UART0_ITCR_REG (UART0_BASE + 0x80)
#define UART0_ITIP_REG (UART0_BASE + 0x84)
#define UART0_ITOP_REG (UART0_BASE + 0x88)
#define UART0_TDR_REG (UART0_BASE + 0x8c)

/* UART Data Register */
/* 31-12: reserved */
/* 11: Overrun error.
 * This bit is set to 1 if data is received and the receive FIFO is already full.
 * This is cleared to 0 once there is an empty space in the FIFO
 * and a new character can be written to it.
 */
#define UART_DATA_REG_OVERRUN_ERROR (0x1 << 11)
/* 10: Break error.
 * This bit is set to 1 if a break condition was detected, indicating that the
 * received data input was held LOW for longer than a full-word transmission time
 * (defined as start, data, parity and stop bits).
 * In FIFO mode, this error is associated with the character at the top of the FIFO.
 * When a break occurs, only one 0 character is loaded into the FIFO.
 * The next character is only enabled after the receive data input
 * goes to a 1 (marking state), and the next valid start bit is received.
 */
#define UART_DATA_REG_BREAK_ERROR (0x1 << 10)
/* 9: Parity error.
 * When set to 1, it indicates that the parity of the received data character
 * does not match the parity that the EPS and SPS bits in the Line Control Register,
 * UART_LCRH select.
 * In FIFO mode, this error is associated with the character at the top of the FIFO.
 */
#define UART_DATA_REG_PARITY_ERROR (0x1 << 9)
/* 8: Framing error.
 * When set to 1, it indicates that the received character
 * did not have a valid stop bit (a valid stop bit is 1).
 * In FIFO mode, this error is associated with the character at the top of the FIFO.
 */
#define UART_DATA_REG_FRAMING_ERROR (0x1 << 8)
/* 7-0: DATA Receive (read) data character.
 * Transmit (write) data character.
 */
#define UART_DATA_REG_DATA_MASK 0xff

/* UART Receive Status Register/Error Clear Register */
/* 31-4: reserved */
/* 3: Overrun error.
 * This bit is set to 1 if data is received and the receive FIFO is already full.
 * This is cleared to 0 once there is an empty space in the FIFO
 * and a new character can be written to it.
 */
#define UART_RSRECR_OVERRUN_ERROR (0x1 << 3)
/* 2: Break error.
 * This bit is set to 1 if a break condition was detected, indicating that
 * the received data input was held LOW for longer than a full-word transmission time
 * (defined as start, data, parity and stop bits).
 * In FIFO mode, this error is associated with the character at the top of the FIFO
 * . When a break occurs, only one 0 character is loaded into the FIFO.
 * The next character is only enabled after the receive data input
 * goes to a 1 (marking state), and the next valid start bit is received.
 */
#define UART_RSRECR_BREAK_ERROR (0x1 << 2)
/* 1: Parity error.
 * When set to 1, it indicates that the parity of the received data character
 * does not match the parity that the EPS and SPS bits in the Line Control Register,
 * UART_LCRH select.
 * In FIFO mode, this error is associated with the character at the top of the FIFO.
 */
#define UART_RSRECR_PARITY_ERROR (0x1 << 1)
/* Framing error.
 * When set to 1, it indicates that the received character did not have
 * a valid stop bit (a valid stop bit is 1).
 * In FIFO mode, this error is associated with the character at the top of the FIFO.
 */
#define UART_RSRECR_FRAMING_ERROR (0x1 << 0)

/* UART Flag Register */
/* 31-9: reserved */
/* 8: Unsupported */
#define UART_FLAG_REG_RI (0x1 << 8)
/* 7: Transmit FIFO empty.
 * The meaning of this bit depends on the state of the FEN bit
 * in the Line Control Register, UARTLCR_LCRH.
 * If the FIFO is disabled, this bit is set when the transmit holding register is empty.
 * If the FIFO is enabled, the TXFE bit is set when the transmit FIFO is empty.
 * This bit does not indicate if there is data in the transmit shift register.
 */
#define UART_FLAG_REG_TRANSMIT_FIFO_EMPTY (0x1 << 7)
/* 6: Receive FIFO full.
 * The meaning of this bit depends on the state of the FEN bit
 * in the UARTLCR_LCRH Register.
 * If the FIFO is disabled, this bit is set when the receive holding register is full.
 * If the FIFO is enabled, the RXFF bit is set when the receive FIFO is full.
 */
#define UART_FLAG_REG_RECEIVE_FIFO_FULL (0x1 << 6)
/* 5: Transmit FIFO full.
 * The meaning of this bit depends on the state of the FEN bit
 * in the UARTLCR_LCRH Register.
 * If the FIFO is disabled, this bit is set when the transmit holding register is full.
 * If the FIFO is enabled, the TXFF bit is set when the transmit FIFO is full.
 */
#define UART_FLAG_REG_TRANSMIT_FIFO_FULL (0x1 << 5)
/* 4: Receive FIFO empty.
 * The meaning of this bit depends on the state of the FEN bit in the UARTLCR_H Register.
 * If the FIFO is disabled, this bit is set when the receive holding register is empty.
 * If the FIFO is enabled, the RXFE bit is set when the receive FIFO is empty.
 */
#define UART_FLAG_REG_RECEIVE_FIFO_EMPTY (0x1 << 4)
/* 3: UART busy.
 * If this bit is set to 1, the UART is busy transmitting data.
 * This bit remains set until the complete byte, including all the stop bits,
 * has been sent from the shift register.
 * This bit is set as soon as the transmit FIFO becomes non-empty,
 * regardless of whether the UART is enabled or not.
 */
#define UART_FLAG_REG_UART_BUSY (0x1 << 3)
/* 2: Unsupported */
#define UART_FLAG_REG_DCD (0x1 << 2)
/* 1: Unsupported */
#define UART_FLAG_REG_DSR (0x1 << 1)
/* 0: Clear to send.
 * This bit is the complement of the UART clear to send, nUARTCTS,
 * modem status input. That is, the bit is 1 when nUARTCTS is LOW.
 */
#define UART_FLAG_REG_CLEAR_TO_SEND (0x1 << 0)

/* UART ILPR Register */
/* 31-0: reserved */

/* UART Integer Baud Rate Divisor */
/* 31-16: reserved */
/* 15-0: The integer baud rate divisor. */
#define UART_IBRD_MASK 0xffff

/* UART Fractional Baud Rate Divisor */
/* 31-6: reserved */
/* 5-0: The fractional baud rate divisor. */
#define UART_FBRD_MASK 0x3f

/* UART Line Control Register */
/* 31-8: reserved */
/* 7: Stick parity select.
 * 0 = stick parity is disabled
 * 1 = either:
 * if the EPS bit is 0 then the parity bit is transmitted and checked as a 1.
 * if the EPS bit is 1 then the parity bit is transmitted and checked as a 0.
 */
#define UART_LCRH_REG_STICK_PARITY_SELECT (0x1 << 7)
/* 6-5: Word length.
 * These bits indicate the number of data bits transmitted or received
 * in a frame as follows:
 * 11 = 8 bits
 * 10 = 7 bits
 * 01 = 6 bits
 * 00 = 5 bits.
 */
#define UART_LCRH_REG_WORD_LENGTH_MASK (0x3 << 5)
/* 4: Enable FIFOs.
 * 0 = FIFOs are disabled (character mode) that is, the FIFOs become 1-byte-deep
 * holding registers.
 * 1 = transmit and receive FIFO buffers are enabled (FIFO mode).
 */
#define UART_LCRH_REG_ENABLE_FIFO (0x1 << 4)
/* 3: Two stop bits select.
 * If this bit is set to 1, two stop bits are transmitted at the end of
 * the frame. The receive logic does not check for two stop bits being received.
 */
#define UART_LCRH_REG_TWO_STOP_BITS_SELECT (0x1 << 3)
/* 2: Even parity select.
 * Controls the type of parity the UART uses during transmission and reception:
 * 0 = odd parity. The UART generates or checks for an odd number of 1s
 * in the data and parity bits.
 * 1 = even parity. The UART generates or checks for an even number of 1s in the
 * data and parity bits.
 * This bit has no effect when the PEN bit disables parity checking and generation.
 */
#define UART_LCRH_REG_EVEN_PARITY_SELECT (0x1 << 2)
/* 1: Parity enable.
 * 0 = parity is disabled and no parity bit added to the data frame.
 * 1 = parity checking and generation is enabled.
 */
#define UART_LCRH_REG_PARITY_ENABLE (0x1 << 1)
/* 0: Send break.
 * If this bit is set to 1, a low-level is continually output on the TXD output,
 * after completing transmission of the current character.
 */
#define UART_LCRH_REG_SEND_BREAK (0x1 << 0)

/* UART Control Register */
/* 31-16: reserved */
/* 15: CTS hardware flow control enable.
 * If this bit is set to 1, CTS hardware flow control is enabled.
 * Data is only transmitted when the nUARTCTS signal is asserted.
 */
#define UART_CTRL_REG_CTS_HARDWARE_FLOW_CTRL_ENABLE (0x1 << 15)
/* 14: RTS hardware flow control enable.
 * If this bit is set to 1, RTS hardware flow control is enabled.
 * Data is only requested when there is space in the receive FIFO
 * for it to be received.
 */
#define UART_CTRL_REG_RTS_HARDWARE_FLOW_CTRL_ENABLE (0x1 << 14)
/* 13: Unsupported */
#define UART_CTRL_REG_OUT2 (0x1 << 13)
/* 12: Unsupported */
#define UART_CTRL_REG_OUT1 (0x1 << 12)
/* 11: Request to send.
 * This bit is the complement of the UART request to send, nUARTRTS,
 * modem status output. That is, when the bit is programmed to a 1 then
 * nUARTRTS is LOW.
 */
#define UART_CTRL_REG_REQUEST_TO_SEND (0x1 << 11)
/* 10: Unsupported */
#define UART_CTRL_REG_DTR (0x1 << 10)
/* 9: Receive enable.
 * If this bit is set to 1, the receive section of the UART is enabled.
 * Data reception occurs for UART signals.
 * When the UART is disabled in the middle of reception,
 * it completes the current character before stopping.
 */
#define UART_CTRL_REG_RECEIVE_ENABLE (0x1 << 9)
/* 8: Transmit enable.
 * If this bit is set to 1, the transmit section of the UART is enabled.
 * Data transmission occurs for UART signals.
 * When the UART is disabled in the middle of transmission,
 * it completes the current character before stopping.
 */
#define UART_CTRL_REG_TRANSMIT_ENABLE (0x1 << 8)
/* 7: Loopback enable.
 * If this bit is set to 1, the UARTTXD path is fed through to the UARTRXD path.
 * In UART mode, when this bit is set, the modem outputs are also fed
 * through to the modem inputs. This bit is cleared to 0 on reset, to disable loopback.
 */
#define UART_CTRL_REG_LOOPBACK_ENABLE (0x1 << 7)
/* 6-3: reserved */
/* 2: Unsupported */
#define UART_CTRL_REG_SIRLP (0x1 << 2)
/* 1: Unsupported */
#define UART_CTRL_REG_SIREN (0x1 << 1)
/* 0: UART enable:
 * 0 = UART is disabled. If the UART is disabled in the middle of transmission
 * or reception, it completes the current character before stopping.
 * 1 = the UART is enabled.
*/
#define UART_CTRL_REG_UART_ENABLE (0x1 << 0)

/* UART Interrupt FIFO Level Select Register */
/* 31-12: reserved */
/* 11-9: Unsupported */
#define UART_IFLS_REG_RXIFPSEL_MASK (0x7 << 9)
/* 8-6: Unsupported */
#define UART_IFLS_REG_TXIFPSEL_MASK (0x7 << 6)
/* 5-3: Receive interrupt FIFO level select.
 * The trigger points for the receive interrupt are as follows:
 * 000 = Receive FIFO becomes 1/8 full
 * 001 = Receive FIFO becomes 1/4 full
 * 010 = Receive FIFO becomes 1/2 full
 * 011 = Receive FIFO becomes 3/4 full
 * 100 = Receive FIFO becomes 7/8 full
 * 101-111 = reserved.
 */
#define UART_IFLS_REG_RXIFLSEL_MASK (0x7 << 3)
/* 2-0: Transmit interrupt FIFO level select.
 * The trigger points for the transmit interrupt are as follows:
 * 000 = Transmit FIFO becomes 1/8 full
 * 001 = Transmit FIFO becomes 1/4 full
 * 010 = Transmit FIFO becomes 1/2 full
 * 011 = Transmit FIFO becomes 3/4 full
 * 100 = Transmit FIFO becomes 7/8 full
 * 101-111 = reserved.
 */
#define UART_IFLS_REG_TXIFLSEL_MASK (0x7 << 0)

/* UART Interrupt Mask Set Clear Register */
/* 31-11: reserved */
/* 10: Overrun error interrupt mask.
 * A read returns the current mask for the interrupt.
 * On a write of 1, the mask of the UARTOEINTR interrupt is set.
 * A write of 0 clears the mask.
 */
#define UART_IMSC_REG_OVERRUN_ERROR_INTERRUPT_MASK (0x1 << 10)
/* 9: Break error interrupt mask.
 * A read returns the current mask for the UARTBEINTR interrupt.
 * On a write of 1, the mask of the interrupt is set.
 * A write of 0 clears the mask.
 */
#define UART_IMSC_REG_BREAK_ERROR_INTERRUPT_MASK (0x1 << 9)
/* 8: Parity error interrupt mask.
 * A read returns the current mask for the UARTPEINTR interrupt.
 * On a write of 1, the mask of the interrupt is set.
 * A write of 0 clears the mask.
 */
#define UART_IMSC_REG_PARITY_ERROR_INTERRUPT_MASK (0x1 << 8)
/* 7: Framing error interrupt mask.
 * A read returns the current mask for the UARTFEINTR interrupt.
 * On a write of 1, the mask of the interrupt is set.
 * A write of 0 clears the mask.
 */
#define UART_IMSC_REG_FRAMING_ERROR_INTERRUPT_MASK (0x1 << 7)
/* 6: Receive timeout interrupt mask.
 * A read returns the current mask for the UARTRTINTR interrupt.
 * On a write of 1, the mask of the interrupt is set.
 * A write of 0 clears the mask.
 */
#define UART_IMSC_REG_RECEIVE_TIMEOUT_INTERRUPT_MASK (0x1 << 6)
/* 5: Transmit interrupt mask.
 * A read returns the current mask for the UARTTXINTR interrupt.
 * On a write of 1, the mask of the interrupt is set.
 * A write of 0 clears the mask.
 */
#define UART_IMSC_REG_TRANSMIT_INTERRUPT_MASK (0x1 << 5)
/* 4: Receive interrupt mask.
 * A read returns the current mask for the UARTRXINTR interrupt.
 * On a write of 1, the mask of the interrupt is set.
 * A write of 0 clears the mask.
 */
#define UART_IMSC_REG_RECEIVE_INTERRUPT_MASK (0x1 << 4)
/* 3: Unsupported */
#define UART_IMSC_REG_DSRMIM (0x1 << 3)
/* 2: Unsupported */
#define UART_IMSC_REG_DCDMIM (0x1 << 2)
/* 1: nUARTCTS modem interrupt mask.
 * A read returns the current mask for the UARTCTSINTR interrupt.
 * On a write of 1, the mask of the interrupt is set.
 * A write of 0 clears the mask.
 */
#define UART_IMSC_REG_CTSMIM (0x1 << 1)
/* 0: Unsupported */
#define UART_IMSC_REG_RIMM (0x1 << 0)

/* UART Raw Interrupt Status Register */
/* 31-11: reserved */
/* 10: Overrun error interrupt status.
 * Returns the raw interrupt state of the UARTOEINTR interrupt.
 */
#define UART_RIS_REG_OVERRUN_ERROR_INTERRUPT_STATUS (0x1 << 10)
/* 9: Break error interrupt status.
 * Returns the raw interrupt state of the UARTBEINTR interrupt.
 */
#define UART_RIS_REG_BREAK_ERROR_INTERRUPT_STATUS (0x1 << 9)
/* 8: Parity error interrupt status.
 * Returns the raw interrupt state of the UARTPEINTR interrupt.
 */
#define UART_RIS_REG_PARITY_ERROR_INTERRUPT_STATUS (0x1 << 8)
/* 7: Framing error interrupt status.
 * Returns the raw interrupt state of the UARTFEINTR interrupt.
 */
#define UART_RIS_REG_FRAMING_ERROR_INTERRUPT_STATUS (0x1 << 7)
/* 6: Receive timeout interrupt status.
 * Returns the raw interrupt state of the UARTRTINTR interrupt.
 */
#define UART_RIS_REG_RECEIVE_TIMEOUT_INTERRUPT_STATUS (0x1 << 6)
/* 5: Transmit interrupt status.
 * Returns the raw interrupt state of the UARTTXINTR interrupt.
 */
#define UART_RIS_REG_TRANSMIT_INTERRUPT_STATUS (0x1 << 5)
/* 4: Receive interrupt status.
 * Returns the raw interrupt state of the UARTRXINTR interrupt.
 */
#define UART_RIS_REG_RECEIVE_INTERRUPT_STATUS (0x1 << 4)
/* 3: Unsupported */
#define UART_RIS_REG_DSRRMIS (0x1 << 3)
/* 2: Unsupported */
#define UART_RIS_REG_DCDRMIS (0x1 << 2)
/* 1: nUARTCTS modem interrupt status.
 * Returns the raw interrupt state of the UARTCTSINTR interrupt.
 */
#define UART_RIS_REG_CTSRMIS (0x1 << 1)
/* 0: Unsupported */
#define UART_RIS_REG_RIRMIS (0x1 << 0)

/* UART Masked Interrupt Status Register */
/* 31-11: reserved */
/* 10: Overrun error masked interrupt status.
 * Returns the masked interrupt state of the UARTOEINTR interrupt.
 */
#define UART_MIS_REG_OVERRUN_ERROR_MASKED_INTERRUPT_STATUS (0x1 << 10)
/* 9: Break error masked interrupt status.
 * Returns the masked interrupt state of the UARTBEINTR interrupt.
 */
#define UART_MIS_REG_BREAK_ERROR_MASKED_INTERRUPT_STATUS (0x1 << 9)
/* 8: Parity error masked interrupt status.
 * Returns the masked interrupt state of the UARTPEINTR interrupt.
 */
#define UART_MIS_REG_PARITY_ERROR_MASKED_INTERRUPT_STATUS (0x1 << 8)
/* 7: Framing error masked interrupt status.
 * Returns the masked interrupt state of the UARTFEINTR interrupt.
 */
#define UART_MIS_REG_FRAMING_ERROR_MASKED_INTERRUPT_STATUS (0x1 << 7)
/* 6: Receive timeout masked interrupt status.
 * Returns the masked interrupt state of the UARTRTINTR interrupt.
 */
#define UART_MIS_REG_RECEIVE_TIMEOUT_MASKED_INTERRUPT_STATUS (0x1 << 6)
/* 5: Transmit masked interrupt status.
 * Returns the masked interrupt state of the UARTTXINTR interrupt.
 */
#define UART_MIS_REG_TRANSMIT_MASKED_INTERRUPT_STATUS (0x1 << 5)
/* 4: Receive masked interrupt status.
 * Returns the masked interrupt state of the UARTRXINTR interrupt.
 */
#define UART_MIS_REG_RECEIVE_MASKED_INTERRUPT_STATUS (0x1 << 4)
/* 3: Unsupported */
#define UART_MIS_REG_DSRMMIS (0x1 << 3)
/* 2: Unsupported */
#define UART_MIS_REG_DCDMMIS (0x1 << 2)
/* 1: nUARTCTS modem masked interrupt status.
 * Returns the masked interrupt state of the UARTCTSINTR interrupt.
 */
#define UART_MIS_REG_CTSMMIS (0x1 << 1)
/* 0: Unsupported */
#define UART_MIS_REG_RIMMIS (0x1 << 0)

/* UART Interrupt Clear Register */
/* 31-11: reserved */
/* 10: Overrun error interrupt clear.
 * Clears the UARTOEINTR interrupt.
 */
#define UART_ICR_REG_OVERRUN_ERROR_INTERRUPT_CLEAR (0x1 << 10)
/* 9: Break error interrupt clear.
 * Clears the UARTBEINTR interrupt.
 */
#define UART_ICR_REG_BREAK_ERROR_INTERRUPT_CLEAR (0x1 << 9)
/* 8: Parity error interrupt clear.
 * Clears the UARTPEINTR interrupt.
 */
#define UART_ICR_REG_PARITY_ERORR_INTERRUPT_CLEAR (0x1 << 8)
/* 7: Framing error interrupt clear.
 * Clears the UARTFEINTR interrupt.
 */
#define UART_ICR_REG_FRAMING_ERROR_INTERRUPT_CLEAR (0x1 << 7)
/* 6: Receive timeout interrupt clear.
 * Clears the UARTRTINTR interrupt.
 */
#define UART_ICR_REG_RECEIVE_TIMEOUT_INTERRUPT_CLEAR (0x1 << 6)
/* 5: Transmit interrupt clear.
 * Clears the UARTTXINTR interrupt.
 */
#define UART_ICR_REG_TRANSMIT_INTERRUPT_CLEAR (0x1 << 5)
/* 4: Receive masked interrupt status.
 * Returns the masked interrupt state of the UARTRXINTR interrupt.
 */
#define UART_ICR_REG_RECEIVE_MASKED_INTERRUPT_STATUS (0x1 << 4)
/* 3: Unsupported */
#define UART_ICR_REG_DSRMIC (0x1 << 3)
/* 2: Unsupported */
#define UART_ICR_REG_DCDMIC (0x1 << 2)
/* 1: nUARTCTS modem masked interrupt status.
 * Returns the masked interrupt state of the UARTCTSINTR interrupt.
 */
#define UART_ICR_REG_CTSMIC (0x1 << 1)
/* 0: Unsupported */
#define UART_ICR_REG_RIMIC (0x1 << 0)

/* UART DMA Control Register */
/* 31-3: reserved */
/* 2: Unsupported */
#define UART_DMA_CTRL_REG_DMAONERR (0x1 << 2)
/* 1: Unsupported */
#define UART_DMA_CTRL_REG_TXDMAE (0x1 << 1)
/* 0: Unsupported */
#define UART_DMA_CTRL_REG_RSDMAE (0x1 << 0)

/* UART Integration Test Control Register */
/* 31-2: reserved */
/* 1: Test FIFO enable.
 * When this bit it 1, a write to the Test Data Register, UART_DR writes
 * data into the receive FIFO, and reads from the UART_DR register
 * reads data out of the transmit FIFO.
 * When this bit is 0, data cannot be read directly from the transmit FIFO
 * or written directly to the receive FIFO (normal operation).
 */
#define UART_ITCR_REG_INTEGRATION_TEST_FIFO_ENABLE (0x1 << 1)
/* 0: Integration test enable.
 * When this bit is 1, the UART is placed in integration test mode,
 * otherwise it is in normal operation.
 */
#define UART_ITCR_REG_INTEGRATION_TEST_ENABLE (0x1 << 0)

/* UART Integration Test Input Register */
/* 31-4: reserved */
/* 3: Reads return the value of the nUARTCTS primary input. */
#define UART_ITIP_REG_UARTCTS_PRIMARY_INPUT (0x1 << 3)
/* 2-1: reserved */
/* 0: Reads return the value of the UARTRXD primary input. */
#define UART_ITIP_REG_UARTRXD_PRIMARY_INPUT (0x1 << 0)

/* UART Integration Test Output Register */
/* 31-12: reserved */
/* 11: Intra-chip output.
 * Writes specify the value to be driven on UARTMSINTR.
 * Reads return the value of UARTMSINTR at the output of the test multiplexor.
 */
#define UART_ITOP_UARTMSINTR (0x1 << 11)
/* 10: Intra-chip output.
 * Writes specify the value to be driven on UARTRXINTR.
 * Reads return the value of UARTRXINTR at the output of the test multiplexor.
 */
#define UART_ITOP_UARTRXINTR (0x1 << 10)
/* 9: Intra-chip output.
 * Writes specify the value to be driven on UARTTXINTR.
 * Reads return the value of UARTTXINTR at the output of the test multiplexor.
 */
#define UART_ITOP_UARTTXINTR (0x1 << 9)
/* 8: Intra-chip output.
 * Writes specify the value to be driven on UARTRTINTR.
 * Reads return the value of UARTRTINTR at the output of the test multiplexor.
 */
#define UART_ITOP_UARTRTINTR (0x1 << 8)
/* 7: Intra-chip output.
 * Writes specify the value to be driven on UARTEINTR.
 * Reads return the value of UARTEINTR at the output of the test multiplexor.
 */
#define UART_ITOP_UARTEINTR (0x1 << 7)
/* 6: Intra-chip output.
 * Writes specify the value to be driven on UARTINTR.
 * Reads return the value of UARTINTR at the output of the test multiplexor.
 */
#define UART_ITOP_UARTINTR (0x1 << 6)
/* 5-4: reserved */
/* 3: Primary output.
 * Writes specify the value to be driven on nUARTRTS.
 */
#define UART_ITOP_UARTRTS (0x1 << 3)
/* 2-1: reserved */
/* 0: Primary output.
 * Writes specify the value to be driven on UARTTXD.
 */
#define UART_ITOP_UARTTXD (0x1 << 0)

/* UART Test Data Register */
/* 31-11: reserved */
/* 10-0: When the ITCR1 bit is set to 1, data is written into the receive FIFO
 * and read out of the transmit FIFO.
 */
#define UART_TDR_MASK 0x3ff



#endif /* __MCUBE_ARM_UART_H__ */
