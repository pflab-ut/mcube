/**
 * @file include/arm/raspi3/dmac.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_ARM_RASPI3_DMAC_H__
#define	__MCUBE_ARM_RASPI3_DMAC_H__


#define DMAC_CH_BASE(x) (DMAC_BASE + (x) * 0x100)

#define DMAC_CH_CS(x) (DMAC_CH_BASE(x) + 0x0)
#define DMAC_CH_CONBLK_ADDR(x) (DMAC_CH_BASE(x) + 0x4)
#define DMAC_CH_TI(x) (DMAC_CH_BASE(x) + 0x8)
#define DMAC_CH_SRC_ADDR(x) (DMAC_CH_BASE(x) + 0xc)
#define DMAC_CH_DST_ADDR(x) (DMAC_CH_BASE(x) + 0x10)
#define DMAC_CH_TRANSFER_LEN(x) (DMAC_CH_BASE(x) + 0x14)
#define DMAC_CH_STRIDE(x) (DMAC_CH_BASE(x) + 0x18)
#define DMAC_CH_NEXTCONBK(x) (DMAC_CH_BASE(x) + 0x1c)
#define DMAC_CH_DEBUG(x) (DMAC_CH_BASE(x) + 0x20)

#define DMAC_INT_STATUS (DMAC_BASE + 0xfe0)
#define DMAC_ENABLE (DMAC_BASE + 0xff0)


/* DMA Control and Status Register */
/* 31: DMA Channel Reset.
 * Writing a 1 to this bit will reset the DMA.
 * The bit cannot be read, and will self clear.
 */
#define DMAC_CH_CS_RESET (0x1 << 31)
/* 30: Abort DMA
 * Writing a 1 to this bit will abort the current DMA CB.
 * The DMA will load the next CB and attempt to continue.
 * The bit cannot be read, and will self clear.
 */
#define DMAC_CH_CS_ABORT (0x1 << 30)
/* 29: Disable debug pause signal.
 * When set to 1, the DMA will not stop when the debug pause signal is asserted.
 */
#define DMAC_CH_CS_DISDEBUG (0x1 << 29)
/* 28: Wait for outstanding writes
 * When set to 1, the DMA will keep a tally of the AXI writes going out and
 * the write responses coming in.
 * At the very end of the current DMA transfer it will wait until
 * the last outstanding write response has been received before indicating
 * the transfer is complete.
 * Whilst waiting it will load the next CB address (but will not fetch the CB),
 * clear the active flag (if the next CB address = zero), and it will defer
 * setting the END flag or the INT flag until the last outstanding write response
 * has been received.
 * In this mode, the DMA will pause if it has more than 13 outstanding writes
 * at any one time.
 */
#define DMAC_CH_CS_WAIT_FOR_OUTSTANDING_WRITES (0x1 << 28)
/* 27-24: reserved */
/* 23-20: AXI Panic Priority Level.
 * Sets the priority of panicking AXI bus transactions.
 * This value is used when the panic bit of the selected peripheral
 * channel is 1.
 * Zero is the lowest priority.
 */
#define DMAC_CH_CS_PANIC_PRIORITY_MASK (0xf << 20)
/* 19-16: AXI Priority Level.
 * Sets the priority of normal AXI bus transactions.
 * This value is used when the panic bit of the selected peripheral channel is zero.
 * Zero is the lowest priority.
 */
#define DMAC_CH_CS_PRIORITY_MASK (0xf << 16)
/* 15-9: reserved */
/* 8: DMA Error.
 * Indicates if the DMA has detected an error.
 * The error flags are available in the debug register, and have to be cleared
 * by writing to that register.
 * 1 = DMA channel has an error flag set.
 * 0 = DMA channel is ok.
 */
#define DMAC_CH_CS_ERROR (0x1 << 8)
/* 7: reserved */
/* 6: DMA is Waiting for the Last Write to be Received.
 * Indicates if the DMA is currently waiting for any outstanding writes to be
 * received, and is not transferring data.
 * 1 = DMA channel is waiting.
 */
#define DMAC_CH_CS_WAITING_FOR_OUTSTANDING_WRITES (0x1 << 6)
/* 5: DMA Paused by DREQ State.
 * Indicates if the DMA is currently paused and not transferring data due to the
 * DREQ being inactive.
 * 1 = DMA channel is paused.
 * 0 = DMA channel is running.
 */
#define DMAC_CH_CS_DREQ_STOPS_DMA (0x1 << 5)
/* 4: DMA Paused State.
 * Indicates if the DMA is currently paused and not transferring data.
 * This will occur if: the active bit has been cleared, if the DMA is
 * currently executing wait cycles or if the debug_pause signal has been
 * set by the debug block, or the number of outstanding writes has exceeded the
 * max count.
 * 1 = DMA channel is paused.
 * 0 = DMA channel is running.
 */
#define DMAC_CH_CS_PAUSED (0x1 << 4)
/* 3: DREQ State.
 * Indicates the state of the selected DREQ (Data Request) signal,
 * ie. the DREQ selected by the PERMAP field of the transfer info.
 * 1 = Requesting data. This will only be valid once the DMA has started
 * and the PERMAP field has been loaded from the CB. It will remain valid,
 * indicating the selected DREQ signal, until a new CB is loaded.
 * If PERMAP is set to zero (unpaced transfer) then this bit will read back as 1.
 * 0 = No data request.
 */
#define DMAC_CH_CS_DREQ (0x1 << 3)
/* 2: Interrupt Status.
 * This is set when the transfer for the CB ends and INTEN is set to 1.
 * Once set it must be manually cleared down, even if the next CB has INTEN = 0.
 * Write 1 to clear.
 */
#define DMAC_CH_CS_INT (0x1 << 2)
/* 1: DMA End Flag
 * Set when the transfer described by the current control block is complete.
 * Write 1 to clear.
 */
#define DMAC_CH_CS_END (0x1 << 1)
/* 0: Activate the DMA.
 * This bit enables the DMA. The DMA will start if this bit is set
 * and the CB_ADDR is non zero.
 * The DMA transfer can be paused and resumed by clearing, then setting it again.
 * This bit is automatically cleared at the end of the complete DMA transfer,
 * ie. after a NEXTCONBK = 0x0000_0000 has been loaded.
 */
#define DMAC_CH_CS_ACTIVE (0x1 << 0)

/* DMA Control Block Address Register */
/* 31-0: Control Block Address.
 * This tells the DMA where to find a Control Block stored in memory.
 * When the ACTIVE bit is set and this address is non zero, the DMA will
 * begin its transfer by loading the contents of the addressed CB into the
 * relevant DMA channel registers.
 * At the end of the transfer this register will be updated with the ADDR field
 * of the NEXTCONBK control block register.
 * If this field is zero, the DMA will stop. Reading this register will return
 * the address of the currently active CB (in the linked list of CB s).
 * The address must be 256 bit aligned, so the bottom 5 bits of the address must be zero.
 */
#define DMAC_CH_CONBLK_ADDR_MASK 0xffffffff

/* DMA Transfer Information (Channels 0-6) */
/* 31-27: reserved */
/* 26: Don't Do wide writes as a 2 beat burst.
 * This prevents the DMA from issuing wide writes as 2 beat AXI bursts.
 * This is an inefficient access mode, so the default is to use the bursts.
 */
#define DMAC_CH_TI_NO_WIDE_BURSTS (0x1 << 26)
/* 25: Add Wait Cycles.
 * This slows down the DMA throughput by setting the number of dummy cycles
 * burnt after each DMA read or write operation is completed.
 * A value of 0 means that no wait cycles are to be added.
 */
#define DMAC_CH_TI_WAITS_MASK (0x1f << 21)
/* 20-16: Peripheral Mapping.
 * Indicates the peripheral number (1-31) whose ready signal shall be used
 * to control the rate of the transfers, and whose panic signals will be output
 * on the DMA AXI bus. Set to 0 for a continuous un-paced transfer.
 */
#define DMAC_CH_TI_PREMAP_MASK (0x1f << 16)
/* 15-12: Burst Transfer Length.
 * Indicates the burst length of the DMA transfers.
 * The DMA will attempt to transfer data as bursts of this number of words.
 * A value of zero will produce a single transfer.
 * Bursts are only produced for specific conditions.
 */
#define DMAC_CH_TI_BURST_LENGTH_MASK (0xf << 12)
/* 11: Ignore Reads.
 * 1 = Do not perform source reads. In addition, destination writes will zero
 * all the write strobes. This is used for fast cache fill operations.
 * 0 = Perform source reads.
 */
#define DMAC_CH_TI_SRC_IGNORE (0x1 << 11)
/* 10: Control Source Reads with DREQ.
 * 1 = The DREQ selected by PER_MAP will gate the source reads.
 * 0 = DREQ has no effect.
 */
#define DMAC_CH_TI_SRC_DREQ (0x1 << 10)
/* 9: Source Transfer Width.
 * 1 = Use 128-bit source read width.
 * 0 = Use 32-bit source read width.
 */
#define DMAC_CH_TI_SRC_WIDTH (0x1 << 9)
/* 8: Source Address Increment.
 * 1 = Source address increments after each read. The address will increment
 * by 4, if S_WIDTH=0 else by 32.
 * 0 = Source address does not change.
 */
#define DMAC_CH_TI_SRC_INC (0x1 << 8)
/* 7: Ignore Writes.
 * 1 = Do not perform destination writes.
 * 0 = Write data to destination.
 */
#define DMAC_CH_TI_DST_IGNORE (0x1 << 7)
/* 6: Control Destination Writes with DREQ.
 * 1 = The DREQ selected by PERMAP will gate the destination writes.
 * 0 = DREQ has no effect.
 */
#define DMAC_CH_TI_DST_DREQ (0x1 << 6)
/* 5: Destination Transfer Width.
 * 1 = Use 128-bit destination write width.
 * 0 = Use 32-bit destination write width.
 */
#define DMAC_CH_TI_DST_WIDTH (0x1 << 5)
/* 4: Destination Address Increment.
 * 1 = Destination address increments after each write.
 * The address will increment by 4, if DEST_WIDTH=0 else by 32.
 * 0 = Destination address does not change.
 */
#define DMAC_CH_TI_DST_INC (0x1 << 4)
/* 3: Wait for a Write Response.
 * When set this makes the DMA wait until it receives the AXI write response
 * for each write. This ensures that multiple writes cannot get stacked
 * in the AXI bus pipeline.
 * 1 = Wait for the write response to be received before proceeding.
 * 0 = Don t wait; continue as soon as the write data is sent.
 */
#define DMAC_CH_TI_WAIT_RESP (0x1 << 3)
/* 2: reserved */
/* 1: 2D Mode.
 * 1 = 2D mode interpret the TXFR_LEN register as YLENGTH number of transfers
 * each of XLENGTH, and add the strides to the address after each transfer.
 * 0 = Linear mode interpret the TXFR register as a single transfer
 * of total length {YLENGTH, XLENGTH}.
 */
#define DMAC_CH_TI_TDMODE (0x1 << 1)
/* 0: Interrupt Enable
 * 1 = Generate an interrupt when the transfer described
 * by the current Control Block completes.
 * 0 = Do not generate an interrupt.
 */
#define DMAC_CH_TI_INTEN (0x1 << 0) 


/* DMA Transfer Information (Channels 7-14) */
/* 31-26: reserved */
/* 25-21: Add Wait Cycles.
 * This slows down the DMA throughput by setting the number of dummy cycles burnt
 * after each DMA read or write operation is completed.
 * A value of 0 means that no wait cycles are to be added.
 */
#define DMAC_CH_TI2_WAITS_MASK (0x1f << 21)
/* 20-16: Peripheral Mapping.
 * Indicates the peripheral number (1-31) whose ready signal shall be used
 * to control the rate of the transfers, and whose panic signals will be output
 * on the DMA AXI bus. Set to 0 for a continuous un-paced transfer.
 */
#define DMAC_CH_TI2_PREMAP_MASK (0x1f << 16)
/* 15-12: Burst Transfer Length.
 * Indicates the burst length of the DMA transfers.
 * The DMA will attempt to transfer data as bursts of this number of words.
 * A value of zero will produce a single transfer.
 * Bursts are only produced for specific conditions.
 */
#define DMAC_CH_TI2_BURST_LENGTH_MASK (0xf << 12)
/* 11: Ignore Reads.
 * 1 = Do not perform source reads. In addition, destination writes will zero
 * all the write strobes. This is used for fast cache fill operations.
 * 0 = Perform source reads.
 */
#define DMAC_CH_TI2_SRC_IGNORE (0x1 << 11)
/* 10: Control Source Reads with DREQ.
 * 1 = The DREQ selected by PER_MAP will gate the source reads.
 * 0 = DREQ has no effect.
 */
#define DMAC_CH_TI2_SRC_DREQ (0x1 << 10)
/* 9: Source Transfer Width
 * 1 = Use 128-bit source read width.
 * 0 = Use 32-bit source read width.
 */
#define DMAC_CH_TI2_SRC_WIDTH (0x1 << 9)
/* 8: Source Address Increment
 * 1 = Source address increments after each read.
 * The address will increment by 4, if S_WIDTH=0 else by 32.
 * 0 = Source address does not change.
 */
#define DMAC_CH_TI2_SRC_INC (0x1 << 8)
/* 7: Ignore Writes.
 * 1 = Do not perform destination writes.
 * 0 = Write data to destination.
 */
#define DMAC_CH_TI2_DST_IGNORE (0x1 << 7)
/* 6: Control Destination Writes with DREQ.
 * 1 = The DREQ selected by PERMAP will gate the destination writes.
 * 0 = DREQ has no effect.
 */
#define DMAC_CH_TI2_DST_DREQ (0x1 << 6)
/* 5: Destination Transfer Width.
 * 1 = Use 128-bit destination write width.
 * 0 = Use 32-bit destination write width.
 */
#define DMAC_CH_TI2_DST_WIDTH (0x1 << 5)
/* 4: Destination Address Increment.
 * 1 = Destination address increments after each write
 * The address will increment by 4, if DEST_WIDTH=0 else by 32.
 * 0 = Destination address does not change.
 */
#define DMAC_CH_TI2_DST_INC (0x1 << 4)
/* 3: Wait for a Write Response.
 * When set this makes the DMA wait until it receives the AXI write response
 * for each write. This ensures that multiple writes cannot get stacked
 * in the AXI bus pipeline.
 * 1 = Wait for the write response to be received before proceeding.
 * 0 = Don t wait; continue as soon as the write data is sent.
 */
#define DMAC_CH_TI2_WAIT_RESP (0x1 << 3)
/* 2: reserved */
/* 1: 2D Mode.
 * 1 = 2D mode interpret the TXFR_LEN register as YLENGTH number of transfers
 * each of XLENGTH, and add the strides to the address after each transfer.
 * 0 = Linear mode interpret the TXFR register as a single transfer
 * of total length {YLENGTH ,XLENGTH}.
 */
#define DMAC_CH_TI2_TDMODE (0x1 << 1)
/* 0: Interrupt Enable.
 * 1 = Generate an interrupt when the transfer described
 * by the current Control Block completes.
 * 0 = Do not generate an interrupt.
 */
#define DMAC_CH_TI2_INTEN (0x1 << 0)


/* DMA Source Address */
/* 31-0: DMA Source Address.
 * Source address for the DMA operation.
 * Updated by the DMA engine as the transfer progresses.
 */
#define DMAC_CH_SRC_ADDR_MASK 0xffffffff

/* DMA Destination Address */
/* 31-0: DMA Destination Address.
 * Destination address for the DMA operation.
 * Updated by the DMA engine as the transfer progresses.
 */
#define DMAC_CH_DST_ADDR_MASK 0xffffffff

/* DMA Transfer Length (Channels 0-6) */
/* 31-30: reserved */
/* 29-16: When in 2D mode, This is the Y transfer length,
 * indicating how many xlength transfers are performed.
 * When in normal linear mode this becomes the top bits of the XLENGTH.
 */
#define DMAC_CH_TRANSFER_LEN_YLENGTH_MASK (0x3fff << 16)
/* 15-0: Transfer Length in bytes. */
#define DMAC_CH_TRANSFER_LEN_XLENGTH_MASK 0xffff

/* DMA Transfer Length (Channels 7-14) */
/* 31-16: reserved */
/* 15-0: Transfer Length in bytes. */
#define DMAC_CH_TRANSFER_LEN2_XLENGTH_MASK 0xffff


/* DMA 2D Stride */
/* 31-16: Destination Stride (2D Mode)
 * Signed (2 s complement) byte increment to apply to the destination address
 * at the end of each row in 2D mode.
 */
#define DMAC_CH_STRIDE_DST_STRIDE_MASK (0xffff << 16)
/* 15-0: Source Stride (2D Mode)
 * Signed (2 s complement) byte increment to apply to the source address
 * at the end of each row in 2D mode.
 */
#define DMAC_CH_STRIDE_SRC_STRIDE_MASK 0xffff

/* DMA Next Control Block Address */
/* 31-0: Address of next CB for chained DMA operations. */
#define DMAC_CH_NEXTCONBK_ADDR_MASK 0xffffffff

/* DMA Debug Register (Channels 0-6) */
/* 31-29: reserved */
/* 28: DMA Lite.
 * Set if the DMA is a reduced performance LITE engine.
 */
#define DMAC_CH_DEBUG_LITE (0x1 << 28)
/* 27-25: DMA Version
 * DMA version number, indicating control bit filed changes.
 */
#define DMAC_CH_DEBUG_VERSION_MASK (0x7 << 25)
/* 24-16: DMA State Machine State.
 * Returns the value of the DMA engines state machine for this channel.
 */
#define DMAC_CH_DEBUG_DMA_STATE_MASK (0x1ff << 16)
/* 15-8: DMA ID
 * Returns the DMA AXI ID of this DMA channel.
 */
#define DMAC_CH_DEBUG_DMA_ID (0xff << 8)
/* 7-4: DMA Outstanding Writes Counter.
 * Returns the number of write responses that have not yet been received.
 * This count is reset at the start of each new DMA transfer or with a DMA reset.
 */
#define DMAC_CH_DEBUG_OUTSTANDING_WRITES (0xf << 4)
/* 3: reserved */
/* 2: Slave Read Response Error.
 * Set if the read operation returned an error value on the read response bus.
 * It can be cleared by writing a 1.
 */
#define DMAC_CH_DEBUG_READ_ERROR (0x1 << 2)
/* 1: FIFO Error.
 * Set if the optional read FIFO records an error condition.
 * It can be cleared by writing a 1,
 */
#define DMAC_CH_DEBUG_FIFO_ERROR (0x1 << 1)
/* 0: Read Last Not Set Error
 * If the AXI read last signal was not set when expected,
 * then this error bit will be set. It can be cleared by writing a 1.
 */
#define DMAC_CH_DEBUG_READ_LAST_NOT_SET_ERROR (0x1 << 0)

/* DMA Lite Debug Register (Channels 7-14) */
/* 31-29: reserved */
/* 28: DMA Lite.
 * Set if the DMA is a reduced performance LITE engine.
 */
#define DMAC_CH_DEBUG2_LITE (0x1 << 28)
/* 27-25: DMA Version.
 * DMA version number, indicating control bit filed changes.
 */
#define DMAC_CH_DEBUG2_VERSION (0x7 << 25)
/* 24-16: DMA State Machine State.
 * Returns the value of the DMA engines state machine for this channel.
 */
#define DMAC_CH_DEBUG2_DMA_STATE (0x1ff << 16)
/* 15-8: DMA ID.
 * Returns the DMA AXI ID of this DMA channel.
 */
#define DMAC_CH_DEBUG2_DMA_ID (0xff << 8)
/* 7-4: DMA Outstanding Writes Counter.
 * Returns the number of write responses that have not yet been received.
 * This count is reset at the start of each new DMA transfer or with a DMA reset.
 */
#define DMAC_CH_DEBUG2_OUTSTANDING_WRITES (0xf << 4)
/* 3: reserved */
/* 2: Slave Read Response Error.
 * Set if the read operation returned an error value on the read response bus.
 * It can be cleared by writing a 1.
 */
#define DMAC_CH_DEBUG2_READ_ERROR (0x1 << 2)
/* 1: FIFO Error.
 * Set if the optional read Fifo records an error condition.
 * It can be cleared by writing a 1.
 */
#define DMAC_CH_DEBUG2_FIFO_ERROR (0x1 << 1)
/* 0: Read Last Not Set Error.
 * If the AXI read last signal was not set when expected,
 * then this error bit will be set. It can be cleared by writing a 1.
 */
#define DMAC_CH_DEBUG2_READ_LAST_NOT_SET_ERROR (0x1 << 0)

/* Interrupt Status Register */
/* 31-16: reserved */
/* 15-0: Interrupt Status of DMA Engines 15-0 */
#define DMAC_INT_STATUS_INT(x) (0x1 << (x))

/* 31-15: reserved */
/* 14-0: Enable DMA Engines 14-0 */
#define DMAC_ENABLE_EN(x) (0x1 << (x))


#define DMAC_IRQ_SHIFT 16
#define DMAC_IRQ(x) (0x1 << ((x) + DMAC_IRQ_SHIFT))


#define NR_DMAC_CHS 16


#ifndef __ASSEMBLY__


/**
 * @struct dmac_info
 * @brief DMAC information.
 */
struct dmac_info {
  uint32_t transfer_information;
  uint32_t src_addr;
  uint32_t dst_addr;
  uint32_t transfer_length;
  uint32_t stride;
  uint32_t next_control_block;
  uint32_t debug;
  uint32_t padding;
} __attribute__ ((aligned(32)));

#endif /* !__ASSEMBLY__ */


#endif /*	__MCUBE_ARM_RASPI3_DMAC_H__ */

