/**
 * @file include/aarch64/raspi3/uart.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_RASPI3_UART_H__
#define __MCUBE_AARCH64_RASPI3_UART_H__

/**
 * @def PL011_UART
 * @brief PL011 UART.
 */
#define PL011_UART 1

/**
 * @def MINI_UART
 * @brief Mini UART.
 */
#define MINI_UART 1

/**
 * @def AUX_IRQ_REG
 * @brief Auxiliary IRQ register.
 */
#define AUX_IRQ_REG (UART1_BASE + 0x00)

/**
 * @def AUX_ENABLES
 * @brief Auxiliary enables register.
 */
#define AUX_ENABLES (UART1_BASE + 0x04)

/**
 * @def AUX_MU_IO_REG
 * @brief Auxiliary mini UART I/O data register.
 */
#define AUX_MU_IO_REG (UART1_BASE + 0x40)

/**
 * @def AUX_MU_IER_REG
 * @brief Auxiliary mini UART interrupt enable register.
 */
#define AUX_MU_IER_REG (UART1_BASE + 0x44)

/**
 * @def AUX_MU_IIR_REG
 * @brief Auxiliary mini UART interrupt identify register.
 */
#define AUX_MU_IIR_REG (UART1_BASE + 0x48)

/**
 * @def AUX_MU_LCR_REG
 * @brief Auxiliary mini UART line control register.
 */
#define AUX_MU_LCR_REG (UART1_BASE + 0x4c)

/**
 * @def AUX_MU_MCR_REG
 * @brief Auxiliary mini UART modem control register.
 */
#define AUX_MU_MCR_REG (UART1_BASE + 0x50)

/**
 * @def AUX_MU_LSR_REG
 * @brief Auxiliary mini UART line status register.
 */
#define AUX_MU_LSR_REG (UART1_BASE + 0x54)

/**
 * @def AUX_MU_MSR_REG
 * @brief Auxiliary mini UART modem status register.
 */
#define AUX_MU_MSR_REG (UART1_BASE + 0x58)

/**
 * @def AUX_MU_SCRATCH_REG
 * @brief Auxiliary mini UART scratch register.
 */
#define AUX_MU_SCRATCH_REG (UART1_BASE + 0x5c)

/**
 * @def AUX_MU_CTRL_REG
 * @brief Auxiliary mini UART extra control register.
 */
#define AUX_MU_CTRL_REG (UART1_BASE + 0x60)

/**
 * @def AUX_MU_STAT_REG
 * @brief Auxiliary mini UART extra status register.
 */
#define AUX_MU_STAT_REG (UART1_BASE + 0x64)

/**
 * @def AUX_MU_BAUD_REG
 * @brief Auxiliary mini UART baudrate register.
 */
#define AUX_MU_BAUD_REG (UART1_BASE + 0x68)


/* AUX IRQ Register */
/* 31-3: reserved */
/**
 * @def AUX_IRQ_REG_SPI2_IRQ
 * @brief 2: SPI 2 IRQ.
 * If set the SPI 2 module has an interrupt pending.
 */
#define AUX_IRQ_REG_SPI2_IRQ (0x1 << 2)

/**
 * @def AUX_IRQ_REG_SPI1_IRQ
 * @brief 1: SPI 1 IRQ.
 * If set the SPI1 module has an interrupt pending.
 */
#define AUX_IRQ_REG_SPI1_IRQ (0x1 << 1)

/**
 * @def AUX_IRQ_REG_MINI_UART_IRQ
 * @brief 0: Mini UART IRQ.
 * If set the mini UART has an interrupt pending.
 */
#define AUX_IRQ_REG_MINI_UART_IRQ (0x1 << 0)

/* AUX Enable Register */
/* 31-3: reserved */
/**
 * @def AUX_ENABLES_SPI2_ENABLE
 * @brief 2: SPI2 enable.
 * If set the SPI 2 module is enabled.
 * If clear the SPI 2 module is disabled. That also
 * disables any SPI 2 module register access.
 */
#define AUX_ENABLES_SPI2_ENABLE (0x1 << 2)

/**
 * @def AUX_ENABLES_SPI1_ENABLE
 * @brief 1: SPI 1 enable.
 * If set the SPI 1 module is enabled.
 * If clear the SPI 1 module is disabled. That also
 * disables any SPI 1 module register access.
 */
#define AUX_ENABLES_SPI1_ENABLE (0x1 << 1)

/**
 * @def AUX_ENABLES_MINI_UART_ENABLE
 * @brief 0: Mini UART enable.
 * If set the mini UART is enabled. The UART will
 * immediately start receiving data, especially if the UART1_RX line is low.
 * If clear the mini UART is disabled. That also disables
 * any mini UART register access.
 */
#define AUX_ENABLES_MINI_UART_ENABLE (0x1 << 0)


/* AUX Mini UART I/O Register */
/* 31-8: reserved */
/**
 * @def AUX_MU_IO_REG_LS8BITS_BAUDRATE_MASK
 * @brief 7-0: LS 8 bits Baudrate read/write, DLAB=1.
 * Access to the LS 8 bits of the 16-bit baudrate register.
 * (Only If bit 7 of the line control register (DLAB bit) is set.)
 */
#define AUX_MU_IO_REG_LS8BITS_BAUDRATE_MASK 0xff

/**
 * @def AUX_MU_IO_REG_TRANSMIT_DATA_WRITE_MASK
 * @brief 7-0: Transmit data write, DLAB=0.
 * Data written is put in the transmit FIFO (Provided it is not full).
 * (Only If bit 7 of the line control register (DLAB bit) is clear.)
 */
#define AUX_MU_IO_REG_TRANSMIT_DATA_WRITE_MASK 0xff

/**
 * @def AUX_MU_IO_REG_RECV_DATA_READ_MASK
 * @brief 7-0: Receive data read, DLAB=0
 * Data read is taken from the receive FIFO (Provided it is not empty)
 * (Only If bit 7 of the line control register (DLAB bit) is clear.)
 */
#define AUX_MU_IO_REG_RECV_DATA_READ_MASK 0xff


/* AUX Mini UART Interrupt Enable Register */
/* 31-8: reserved */
/**
 * @def AUX_MU_IER_REG_MS8BITS_BAUDRATE_MASK
 * @brief 7-0: MS 8 bits Baudrate read/write, DLAB=1.
 * Access to the MS 8 bits of the 16-bit baudrate register.
 * (Only If bit 7 of the line control register (DLAB bit) is set.)
 */
#define AUX_MU_IER_REG_MS8BITS_BAUDRATE_MASK 0xff

/**
 * @def AUX_MU_IER_REG_ENABLE_TRANSMIT_INTERRUPTS
 * @brief 1: Enable transmit interrupts, DLAB=0.
 * If this bit is set the interrupt line is asserted whenever
 * the transmit FIFO is empty.
 * If this bit is clear no transmit interrupts are generated.
 */
#define AUX_MU_IER_REG_ENABLE_TRANSMIT_INTERRUPTS (0x1 << 1)

/**
 * @def AUX_MU_IER_REG_ENABLE_RECV_INTERRUPTS
 * @brief 0: Enable receive interrupts, DLAB=0.
 * If this bit is set the interrupt line is asserted whenever
 * the receive FIFO holds at least 1 byte.
 * If this bit is clear no receive interrupts are generated.
 */
#define AUX_MU_IER_REG_ENABLE_RECV_INTERRUPTS (0x1 << 0)


/* AUX Mini UART Interrupt Status Register */
/* 31-8: reserved */
/* 7-6: FIFO enables.
 * Both bits always read as 1 as the FIFOs are always enabled */
/* 5-4: Always read as zero */
/* 3: Always read as zero as the mini UART has no timeout function. */
/* 2-1:
 * READ: Interrupt ID bits
 * WRITE: FIFO clear bits
 * On read this register shows the interrupt ID bit.
 * 00 = No interrupts
 * 01 = Transmit holding register empty
 * 10 = Receiver holds valid byte
 * 11 = <Not possible>
 * On write:
 * Writing with bit 1 set will clear the receive FIFO.
 * Writing with bit 2 set will clear the transmit FIFO.
 */
/**
 * @def AUX_MU_IIR_REG_CLEAR_TRANSMIT_FIFO
 * @brief clear transmit FIFO.
 */
#define AUX_MU_IIR_REG_CLEAR_TRANSMIT_FIFO (0x1 << 2)

/**
 * @def AUX_MU_IIR_REG_CLEAR_RECV_FIFO
 * @brief clear receive FIFO.
 */
#define AUX_MU_IIR_REG_CLEAR_RECV_FIFO (0x1 << 1)

/**
 * @def AUX_MU_IIR_REG_INTERRUPT_PENDING
 * @brief 0: Interrupt pending. This bit is clear whenever an interrupt is pending.
 */
#define AUX_MU_IIR_REG_INTERRUPT_PENDING (0x1 << 0)

/* AUX Mini UART Line Control Register */
/* 31-8: reserved */
/**
 * @def AUX_MU_LCR_REG_DLAB_ACCESS
 * @brief 7: DLAB access.
 * If set the first to Mini UART register give access the
 * the Baudrate register. During operation this bit must
 * be cleared.
 */
#define AUX_MU_LCR_REG_DLAB_ACCESS (0x1 << 7)

/**
 * @def AUX_MU_LCR_REG_BREAK
 * @brief 6: Break.
 * If set high the UART1_TX line is pulled low
 * continuously. If held for at least 12 bits times that will
 * indicate a break condition.
 */
#define AUX_MU_LCR_REG_BREAK (0x1 << 6)

/* 5-2: reserved */

/* 1-0: data size. */
/**
 * @def AUX_MU_LCR_REG_DATA_SIZE_MASK
 * @brief Data size mask.
 */
#define AUX_MU_LCR_REG_DATA_SIZE_MASK 0x3

/**
 * @def AUX_MU_LCR_REG_8BIT_MODE
 * @brief 00 : the UART works in 7-bit mode.
 */
#define AUX_MU_LCR_REG_8BIT_MODE (0x3 << 0)

/**
 * @def AUX_MU_LCR_REG_7BIT_MODE
 * @brief 11 : the UART works in 8-bit mode.
 */
#define AUX_MU_LCR_REG_7BIT_MODE (0x0 << 0)


/* AUX Mini UART Modem Control Register */
/* 31-2: reserved */

/**
 * @def AUX_MU_MCR_REG_RTS
 * @brief 1: RTS.
 * If clear the UART1_RTS line is high
 * If set the UART1_RTS line is low.
 * This bit is ignored if the RTS is used for auto-flow control.
 * See the Mini Uart Extra Control register description).
 */
#define AUX_MU_MCR_REG_RTS (0x1 << 1)

/* 0 reserved */

/* AUX Mini UART Data Status Register */
/* 31-7: reserved */
/**
 * @def AUX_MU_LSR_REG_TRANSMITTER_IDLE
 * @brief 6: Transmitter idle.
 * This bit is set if the transmit FIFO is empty and the
 * transmitter is idle. (Finished shifting out the last bit).
 */
#define AUX_MU_LSR_REG_TRANSMITTER_IDLE (0x1 << 6)


/**
 * @def AUX_MU_LSR_REG_TRANSMITTER_EMPTY
 * @brief 5 Transmitter empty.
 * This bit is set if the transmit FIFO can accept at least one byte.
 */
#define AUX_MU_LSR_REG_TRANSMITTER_EMPTY (0x1 << 5)

/* 4-2: reserved */

/**
 * @def AUX_MU_LSR_REG_RECEIVER_OVERRUN
 * @brief 1: Receiver Overrun.
 * This bit is set if there was a receiver overrun. That is:
 * one or more characters arrived whilst the receive FIFO was full.
 * The newly arrived charters have been discarded.
 * This bit is cleared each time this register is read.
 * To do a non-destructive read of this overrun bit use
 * the Mini Uart Extra Status register.
 */
#define AUX_MU_LSR_REG_RECEIVER_OVERRUN (0x1 << 1)

/**
 * @def AUX_MU_LSR_REG_DATA_READY
 * @brief 0: Data ready.
 * This bit is set if the receive FIFO holds at least 1 symbol.
 */
#define AUX_MU_LSR_REG_DATA_READY (0x1 << 0)

/* AUX Mini UART Modem Status Register */
/* 31-6: reserved */

/**
 * @def AUX_MU_MSR_REG_CTS_STATUS
 * @brief 5: CTS status.
 * This bit is the inverse of the UART1_CTS input Thus:
 * If set the UART1_CTS pin is low.
 * If clear the UART1_CTS pin is high.
 */
#define AUX_MU_MSR_REG_CTS_STATUS (0x1 << 5)

/* 4-0: reserved */

/* AUX Mini UART Scratch Register */
/* 31-8: reserved */

/**
 * @def AUX_MU_SCRATCH_REG_SCRATCH_MASK
 * @brief 7-0: Scratch.
 * One whole byte extra on top of the 134217728 provided by the SDC.
 */
#define AUX_MU_SCRATCH_REG_SCRATCH_MASK 0xff

/* AUX Mini UART Control Register */
/* 31-8: reserved */

/**
 * @def AUX_MU_CTRL_REG_CTS_ASSERT_LEVEL
 * @brief 7: CTS assert level.
 * This bit allows one to invert the CTS auto flow operation polarity.
 * If set the CTS auto flow assert level is low.
 * If clear the CTS auto flow assert level is high.
 */
#define AUX_MU_CTRL_REG_CTS_ASSERT_LEVEL (0x1 << 7)

/**
 * @def AUX_MU_CTRL_REG_RTS_ASSERT_LEVEL
 * @brief 6: RTS assert level.
 * This bit allows one to invert the RTS auto flow operation polarity.
 * If set the RTS auto flow assert level is low.
 * If clear the RTS auto flow assert level is high.
 */
#define AUX_MU_CTRL_REG_RTS_ASSERT_LEVEL (0x1 << 6)

/**
 * @def AUX_MU_CTRL_REG_RTS_AUTO_FLOW_LEVEL_MASK
 * @brief 5-4: RTS AUTO flow level.
 * These two bits specify at what receiver FIFO level the
 * RTS line is de-asserted in auto-flow mode.
 * 00 = De-assert RTS when the receive FIFO has 3 empty spaces left.
 * 01 = De-assert RTS when the receive FIFO has 2 empty spaces left.
 * 10 = De-assert RTS when the receive FIFO has 1 empty space left.
 * 11 = De-assert RTS when the receive FIFO has 4 empty spaces left.
 */
#define AUX_MU_CTRL_REG_RTS_AUTO_FLOW_LEVEL_MASK (0x3 << 5)

/**
 * @def AUX_MU_CTRL_REG_ENABLE_TRANSMIT_AUTO_FLOW_CTRL_USING_CTS
 * @brief 3: Enable transmit Auto flow-control using CTS.
 * If this bit is set the transmitter will stop if the CTS line is de-asserted.
 * If this bit is clear the transmitter will ignore the status of the CTS line.
 */
#define AUX_MU_CTRL_REG_ENABLE_TRANSMIT_AUTO_FLOW_CTRL_USING_CTS (0x1 << 3)

/**
 * @def AUX_MU_CTRL_REG_ENABLE_RECV_AUTO_FLOW_CTRL_USING_RTS
 * @brief 2: Enable receive Auto flow-control using RTS.
 * If this bit is set the RTS line will de-assert if the receive FIFO reaches
 * it 'auto flow' level. In fact the RTS line will behave as an RTR (Ready To Receive)
 * line. If this bit is clear the RTS line is controlled by
 * the AUX_MU_MCR_REG register bit 1.
 */
#define AUX_MU_CTRL_REG_ENABLE_RECV_AUTO_FLOW_CTRL_USING_RTS (0x1 << 2)

/**
 * @def AUX_MU_CTRL_REG_TRANSMITTER_ENABLE
 * @brief 1: Transmitter enable.
 * If this bit is set the mini UART transmitter is enabled.
 * If this bit is clear the mini UART transmitter is disabled.
 */
#define AUX_MU_CTRL_REG_TRANSMITTER_ENABLE (0x1 << 1)

/**
 * @def AUX_MU_CTRL_REG_RECEIVER_ENABLE
 * @brief 0: Receiver enable.
 * If this bit is set the mini UART receiver is enabled.
 * If this bit is clear the mini UART receiver is disabled.
 */
#define AUX_MU_CTRL_REG_RECEIVER_ENABLE (0x1 << 0)

/* 31-28: reserved */

/**
 * @def AUX_MU_STAT_REG_TRANSMIT_FIFO_FILL_LEVEL_MASK
 * @brief 27-24: Transmit FIFO fill level.
 * These bits shows how many symbols are stored in the transmit FIFO.
 * The value is in the range 0-8.
 */
#define AUX_MU_STAT_REG_TRANSMIT_FIFO_FILL_LEVEL_MASK (0xf << 24)

/* 23-20: reserved */

/**
 * @def AUX_MU_STAT_REG_RECEIVE_FIFO_FILL_LEVEL_MASK
 * @brief 19-16: Receive FIFO fill level.
 * These bits shows how many symbols are stored in the receive FIFO.
 * The value is in the range 0-8.
 */
#define AUX_MU_STAT_REG_RECEIVE_FIFO_FILL_LEVEL_MASK (0xf << 16)

/* 15-10: reserved */

/**
 * @def AUX_MU_STAT_REG_TRANSMITTER_DONE
 * @brief 9: Transmitter done.
 * This bit is set if the transmitter is idle and the transmit FIFO is empty.
 * It is a logic AND of bits 2 and 8.
 */
#define AUX_MU_STAT_REG_TRANSMITTER_DONE (0x1 << 9)

/**
 * @def AUX_MU_STAT_REG_TRANSMITTER_FIFO_IS_EMPTY
 * @brief 8: Transmit FIFO is empty.
 * If this bit is set the transmitter FIFO is empty. Thus it can accept 8 symbols.
 */
#define AUX_MU_STAT_REG_TRANSMITTER_FIFO_IS_EMPTY (0x1 << 8)

/**
 * @def AUX_MU_STAT_REG_CTS_LINE
 * @brief 7: CTS line.
 * This bit shows the status of the UART1_CTS line.
 */
#define AUX_MU_STAT_REG_CTS_LINE (0x1 << 7)

/**
 * @def AUX_MU_STAT_REG_RTS_STATUS
 * @brief 6: RTS status.
 * This bit shows the status of the UART1_RTS line.
 */
#define AUX_MU_STAT_REG_RTS_STATUS (0x1 << 6)

/**
 * @def AUX_MU_STAT_REG_TRANSMIT_FIFO_IS_FULL
 * @brief 5: Transmit FIFO is full.
 * This is the inverse of bit 1.
 */
#define AUX_MU_STAT_REG_TRANSMIT_FIFO_IS_FULL (0x1 << 5)

/**
 * @def AUX_MU_STAT_REG_RECEIVER_OVERRUN
 * @brief 4: Receiver overrun.
 * This bit is set if there was a receiver overrun. That is:
 * one or more characters arrived whilst the receive FIFO was full.
 * The newly arrived characters have been discarded.
 * This bit is cleared each time the AUX_MU_LSR_REG register is read.
 */
#define AUX_MU_STAT_REG_RECEIVER_OVERRUN (0x1 << 4)

/**
 * @def AUX_MU_STAT_REG_TRANSMITTER_IS_IDLE
 * @brief 3: Transmitter is idle.
 * If this bit is set the transmitter is idle.
 * If this bit is clear the transmitter is idle.
 */
#define AUX_MU_STAT_REG_TRANSMITTER_IS_IDLE (0x1 << 3)

/**
 * @def AUX_MU_STAT_REG_RECEIVER_IS_IDLE
 * @brief 2: Receiver is idle.
 * If this bit is set the receiver is idle.
 * If this bit is clear the receiver is busy.
 * This bit can change unless the receiver is disabled.
 */
#define AUX_MU_STAT_REG_RECEIVER_IS_IDLE (0x1 << 2)

/**
 * @def AUX_MU_STAT_REG_SPACE_AVAILABLE
 * @brief 1: Space available.
 * If this bit is set the mini UART transmitter FIFO
 * can accept at least one more symbol.
 * If this bit is clear the mini UART transmitter FIFO is full.
 */
#define AUX_MU_STAT_REG_SPACE_AVAILABLE (0x1 << 1)

/**
 * @def AUX_MU_STAT_REG_SYMBOL_AVAILABLE
 * @brief 0: Symbol available.
 * If this bit is set the mini UART receive FIFO contains at least 1 symbol.
 * If this bit is clear the mini UART receiver FIFO is empty.
 */
#define AUX_MU_STAT_REG_SYMBOL_AVAILABLE (0x1 << 0)

/* AUX Mini Uart Baudrate Register */
/* 31-16: reserved */
/* 15-0: Baudrate. */

/**
 * @def AUX_MU_BAUD_REG_BAUDRATE_MASK
 * @brief Mini UART baudrate counter mask.
 */
#define AUX_MU_BAUD_REG_BAUDRATE_MASK 0xffff

/**
 * @def BAUDRATE_BPS
 * @brief Baudrate bit/s.
 */
#define BAUDRATE_BPS 115200

#ifndef __ASSEMBLY__

#if PL011_UART

/**
 * @def PL011_UART_IRQ
 * @brief PL011 UART IRQ.
 */
#define PL011_UART_IRQ 25

#elif MINI_UART

/**
 * @def MINI_UART_IRQ
 * @brief Mini UART IRQ.
 */
#define MINI_UART_IRQ 29

#else
#error "Unknown UART"
#endif

/**
 * @fn static inline uint16_t calculate_mini_uart_baudrate_counter(void)
 * @brief calculate mini UART baudrate counter.
 *
 * @return Mini UART baudrate counter.
 */
static inline uint16_t calculate_mini_uart_baudrate_counter(void)
{
  return CPU_CLOCK / (8 *  BAUDRATE_BPS) - 1;
}


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AARCH64_RASPI3_UART_H__ */

