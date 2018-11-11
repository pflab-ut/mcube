/**
 * @file include/arm/raspi3/timer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_ARM_RASPI3_TIMER_H__
#define	__MCUBE_ARM_RASPI3_TIMER_H__


#define TIMER_CTRL_REG (GENERIC_TIMER_BASE + 0x0)
/* GENERIC_TIMER_BASE + 0x4: reserved */
#define TIMER_CORE_TIMER_PRESCALER (GENERIC_TIMER_BASE + 0x8)
#define TIMER_GPU_INTERRUPTS_ROUTING (GENERIC_TIMER_BASE + 0xc)
#define TIMER_PERFORMANCE_MONITOR_INTERRUPTS_ROUTING_SET (GENERIC_TIMER_BASE + 0x10)
#define TIMER_PERFORMANCE_MONITOR_INTERRUPTS_ROUTING_CLEAR (GENERIC_TIMER_BASE + 0x14)
/* GENERIC_TIMER_BASE + 0x18: reserved */
#define TIMER_CORE_TIMER_ACCESS_LOWER_32BITS (GENERIC_TIMER_BASE + 0x1c)
#define TIMER_CORE_TIMER_ACCESS_HIGHER_32BITS (GENERIC_TIMER_BASE + 0x20)
#define TIMER_LOCAL_INTERRUPT_ROUTING (GENERIC_TIMER_BASE + 0x24)
/* GENERIC_TIMER_BASE + 0x28: reserved */
#define TIMER_AXI_OUTSTANDING_COUNTERS (GENERIC_TIMER_BASE + 0x2c)
#define TIMER_AXI_OUTSTANDING_IRQ (GENERIC_TIMER_BASE + 0x30)
#define TIMER_LOCAL_TIMER_CTRL_STATUS (GENERIC_TIMER_BASE + 0x34)
#define TIMER_LOCAL_TIMER_WRITE_FLAGS (GENERIC_TIMER_BASE + 0x38)
/* GENERIC_TIMER_BASE + 0x3c: reserved */
#define TIMER_CORE0_TIMERS_INTERRUPT_CTRL (GENERIC_TIMER_BASE + 0x40)
#define TIMER_CORE1_TIMERS_INTERRUPT_CTRL (GENERIC_TIMER_BASE + 0x44)
#define TIMER_CORE2_TIMERS_INTERRUPT_CTRL (GENERIC_TIMER_BASE + 0x48)
#define TIMER_CORE3_TIMERS_INTERRUPT_CTRL (GENERIC_TIMER_BASE + 0x4c)
#define TIMER_CORE0_MAILBOXES_INTERRUPT_CTRL (GENERIC_TIMER_BASE + 0x50)
#define TIMER_CORE1_MAILBOXES_INTERRUPT_CTRL (GENERIC_TIMER_BASE + 0x54)
#define TIMER_CORE2_MAILBOXES_INTERRUPT_CTRL (GENERIC_TIMER_BASE + 0x58)
#define TIMER_CORE3_MAILBOXES_INTERRUPT_CTRL (GENERIC_TIMER_BASE + 0x5c)
#define TIMER_CORE0_IRQ_SRC (GENERIC_TIMER_BASE + 0x60)
#define TIMER_CORE1_IRQ_SRC (GENERIC_TIMER_BASE + 0x64)
#define TIMER_CORE2_IRQ_SRC (GENERIC_TIMER_BASE + 0x68)
#define TIMER_CORE3_IRQ_SRC (GENERIC_TIMER_BASE + 0x6c)
#define TIMER_CORE0_FIQ_SRC (GENERIC_TIMER_BASE + 0x70)
#define TIMER_CORE1_FIQ_SRC (GENERIC_TIMER_BASE + 0x74)
#define TIMER_CORE2_FIQ_SRC (GENERIC_TIMER_BASE + 0x78)
#define TIMER_CORE3_FIQ_SRC (GENERIC_TIMER_BASE + 0x7c)
#define TIMER_CORE0_MAILBOX0_WRITE_SET (GENERIC_TIMER_BASE + 0x80)
#define TIMER_CORE0_MAILBOX1_WRITE_SET (GENERIC_TIMER_BASE + 0x84)
#define TIMER_CORE0_MAILBOX2_WRITE_SET (GENERIC_TIMER_BASE + 0x88)
#define TIMER_CORE0_MAILBOX3_WRITE_SET (GENERIC_TIMER_BASE + 0x8c)
#define TIMER_CORE1_MAILBOX0_WRITE_SET (GENERIC_TIMER_BASE + 0x90)
#define TIMER_CORE1_MAILBOX1_WRITE_SET (GENERIC_TIMER_BASE + 0x94)
#define TIMER_CORE1_MAILBOX2_WRITE_SET (GENERIC_TIMER_BASE + 0x98)
#define TIMER_CORE1_MAILBOX3_WRITE_SET (GENERIC_TIMER_BASE + 0x9c)
#define TIMER_CORE2_MAILBOX0_WRITE_SET (GENERIC_TIMER_BASE + 0xa0)
#define TIMER_CORE2_MAILBOX1_WRITE_SET (GENERIC_TIMER_BASE + 0xa4)
#define TIMER_CORE2_MAILBOX2_WRITE_SET (GENERIC_TIMER_BASE + 0xa8)
#define TIMER_CORE2_MAILBOX3_WRITE_SET (GENERIC_TIMER_BASE + 0xac)
#define TIMER_CORE3_MAILBOX0_WRITE_SET (GENERIC_TIMER_BASE + 0xb0)
#define TIMER_CORE3_MAILBOX1_WRITE_SET (GENERIC_TIMER_BASE + 0xb4)
#define TIMER_CORE3_MAILBOX2_WRITE_SET (GENERIC_TIMER_BASE + 0xb8)
#define TIMER_CORE3_MAILBOX3_WRITE_SET (GENERIC_TIMER_BASE + 0xbc)
#define TIMER_CORE0_MAILBOX0_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xc0)
#define TIMER_CORE0_MAILBOX1_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xc4)
#define TIMER_CORE0_MAILBOX2_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xc8)
#define TIMER_CORE0_MAILBOX3_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xcc)
#define TIMER_CORE1_MAILBOX0_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xd0)
#define TIMER_CORE1_MAILBOX1_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xd4)
#define TIMER_CORE1_MAILBOX2_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xd8)
#define TIMER_CORE1_MAILBOX3_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xdc)
#define TIMER_CORE2_MAILBOX0_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xe0)
#define TIMER_CORE2_MAILBOX1_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xe4)
#define TIMER_CORE2_MAILBOX2_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xe8)
#define TIMER_CORE2_MAILBOX3_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xec)
#define TIMER_CORE3_MAILBOX0_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xf0)
#define TIMER_CORE3_MAILBOX1_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xf4)
#define TIMER_CORE3_MAILBOX2_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xf8)
#define TIMER_CORE3_MAILBOX3_READ_WRITE_HIGH_TO_CLEAR (GENERIC_TIMER_BASE + 0xfc)


/* Timer Control Register */
/* 31-10: reserved */
/* 9: Timer increment.
 * 1 : 64-bit Core timer increments by 2
 * 0 : 64-bit Core timer increments by 1
 */
#define TIMER_CTRL_REG_TIMER_INCREMENT_BY_2 (0x1 << 9)
#define TIMER_CTRL_REG_TIMER_INCREMENT_BY_1 (0x0 << 8)
/* 8: Core timer clock source.
 * 1 : 64-bit Core timer runs from the APB clock
 * 0 : 64-bit Core timer runs from the Crystal clock
 */
#define TIMER_CTRL_REG_CORE_TIMER_CLOCK_SRC_FROM_APB_CLOCK (0x1 << 8)
#define TIMER_CTRL_REG_CORE_TIMER_CLOCK_SRC_FROM_CRYSTAL_CLOCK (0x0 << 8)
/* 7-0: reserved */

/* Core Timer Pre-Scaler */
/* 31-0: Core timer prescaler.
 * timer_frequency = (2^31 / prescaler) * input frequency (Pre-scaler <= 2^31)
 */
#define TIMER_CORE_TIMER_PRESCALER_FREQ(prescaler, input_freq) \
  (((0x1 << 31) / (prescaler)) * (input_freq))


/* GPU Interrupts Routing */
/* 31-4: reserved */
/* 3-2: GPU FIQ routing:
 * 00 : GPU FIQ goes to FIQ input of core 0
 * 01 : GPU FIQ goes to FIQ input of core 1
 * 10 : GPU FIQ goes to FIQ input of core 2
 * 11 : GPU FIQ goes to FIQ input of core 3
 */ 
#define TIMER_GPU_INTERRUPT_ROUTING_FIQ_CORE0 (0x0 << 2)
#define TIMER_GPU_INTERRUPT_ROUTING_FIQ_CORE1 (0x1 << 2)
#define TIMER_GPU_INTERRUPT_ROUTING_FIQ_CORE2 (0x2 << 2)
#define TIMER_GPU_INTERRUPT_ROUTING_FIQ_CORE3 (0x3 << 2)

/* 1-0: GPU IRQ routing:
 * 00 : GPU IRQ goes to IRQ input of core 0
 * 01 : GPU IRQ goes to IRQ input of core 1
 * 10 : GPU IRQ goes to IRQ input of core 2
 * 11 : GPU IRQ goes to IRQ input of core 3
*/
#define TIMER_GPU_INTERRUPT_ROUTING_IRQ_CORE0 (0x0 << 0)
#define TIMER_GPU_INTERRUPT_ROUTING_IRQ_CORE1 (0x1 << 0)
#define TIMER_GPU_INTERRUPT_ROUTING_IRQ_CORE2 (0x2 << 0)
#define TIMER_GPU_INTERRUPT_ROUTING_IRQ_CORE3 (0x3 << 0)


/* PMU Interrupt Routing Write-set */
/* PMU Interrupt Routing Write-clear */
/* 31-8: reserved */
/* 7: nPMUIRQ[3] FIQ control.
 * If set, this bit overrides the IRQ bit (3).
 * 0 : FIQ disabled
 * 1 : FIQ Enabled
 */
/* 6: nPMUIRQ[2] FIQ control.
 * If set, this bit overrides the IRQ bit (2).
 * 0 : FIQ disabled
 * 1 : FIQ Enabled
 */
/* 5: nPMUIRQ[1] FIQ control.
 * If set, this bit overrides the IRQ bit (1).
 * 0 : FIQ disabled
 * 1 : FIQ Enabled
 */
/* 4: nPMUIRQ[0] FIQ control.
 * If set, this bit overrides the IRQ bit (0).
 * 0 : FIQ disabled
 * 1 : FIQ Enabled
 */
/* 3: nPMUIRQ[3] IRQ control.
 * This bit is only valid if bit 7 is clear otherwise it is ignored.
 * 0 : IRQ disabled
 * 1 : IRQ Enabled
 */
/* 2: nPMUIRQ[2] IRQ control.
 * This bit is only valid if bit 6 is clear otherwise it is ignored.
 * 0 : IRQ disabled
 * 1 : IRQ Enabled
 */
/* 1: nPMUIRQ[1] IRQ control.
 * This bit is only valid if bit 5 is clear otherwise it is ignored.
 * 0 : IRQ disabled
 * 1 : IRQ Enabled
 */
/* 0: nPMUIRQ[0] IRQ control.
 * This bit is only valid if bit 4 is clear otherwise it is ignored.
 * 0 : IRQ disabled
 * 1 : IRQ Enabled
 */
#define TIMER_PERFORMANCE_MONITOR_INTERRUPTS_ROUTING_SET_ENABLE(x) (0x1 << (x))
#define TIMER_PERFORMANCE_MONITOR_INTERRUPTS_ROUTING_CLEAR_ENABLE(x) (0x1 << (x))


/* Core timer read: LS 32 bits, Write: LS-32 holding register */
/* 31-0: 64-bit core timer read/write, LS 32 bits.
 * When reading returns the current 32 LS bit of the 64 timer and
 * triggers storing a copy of the MS 32 bits.
 * When writing: stores a copy of the 32 bits written.
 * That copy is transferred to the timer when the MS 32 bits are written.
 */
#define TIMER_CORE_TIMER_ACCESS_LOWER_32BITS_MASK 0xffffffff
/* 31-0: 64-bit core timer read/write, MS 32 bits
 * When reading returns the status of the core timer-read-hold register.
 * That register is loaded when the user does a read of the LS-32 timer bits.
 * There is little sense in reading this register without first doing a read
 * from the LS-32 bit register.
 * When writing the value is written to the timer, as well as the value
 * previously written to the LS-32 write-holding bit register.
 * There is little sense in writing this register without first doing a write
 * to the LS-32 bit register.
 */
#define TIMER_CORE_TIMER_ACCESS_HIGHER_32BITS_MASK 0xffffffff


/* Local Interrupt Routing Register */
/* 31-3: reserved */
/* 2-0: Interrupt Routing.
 * 000: Local timer interrupt goes to Core 0 IRQ
 * 001: Local timer interrupt goes to Core 1 IRQ
 * 010: Local timer interrupt goes to Core 2 IRQ
 * 011: Local timer interrupt goes to Core 3 IRQ
 * 100: Local timer interrupt goes to Core 0 FIQ
 * 101: Local timer interrupt goes to Core 1 FIQ
 * 110: Local timer interrupt goes to Core 2 FIQ
 * 111: Local timer interrupt goes to Core 3 FIQ
 */
#define TIMER_LOCAL_INTERRUPT_ROUTING_CORE0_IRQ 0x0
#define TIMER_LOCAL_INTERRUPT_ROUTING_CORE1_IRQ 0x1
#define TIMER_LOCAL_INTERRUPT_ROUTING_CORE2_IRQ 0x2
#define TIMER_LOCAL_INTERRUPT_ROUTING_CORE3_IRQ 0x3
#define TIMER_LOCAL_INTERRUPT_ROUTING_CORE0_FIQ 0x4
#define TIMER_LOCAL_INTERRUPT_ROUTING_CORE1_FIQ 0x5
#define TIMER_LOCAL_INTERRUPT_ROUTING_CORE2_FIQ 0x6
#define TIMER_LOCAL_INTERRUPT_ROUTING_CORE3_FIQ 0x7


/* AXI Outstanding Counters */
/* 31-26: 0 */
/* 16-15: Outstanding writes counter */
/* 15-10: 0 */
/* 9-0: Outstanding reads counter */
#define TIMER_AXI_OUTSTANDING_COUNTERS_WRITES_MASK (0x3 << 15)
#define TIMER_AXI_OUTSTANDING_COUNTERS_READS_MASK 0x3ff

/* AXI Outstanding Interrupt */
/* 31-21: 0 */
/* 20: AXI-outstanding interrupt enable */
#define TIMER_AXI_OUTSTANDING_IRQ_AXI_OUTSTANDING_INTERRUPT_ENABLE (0x1 << 20)
/* 19-0: AXI-outstanding time-out MS24 bits */
#define TIMER_AXI_OUTSTANDING_IRQ_AXI_OUTSTANDING_TIMEOUT_MS24BITS_MASK 0xfffff

/* Local Timer Control and Status Register */
/* 31: Interrupt flag (Read-Only) */
#define TIMER_LOCAL_TIMER_CTRL_STATUS_INTERRUPT_FLAG (0x1 << 31)
/* 30: reserved */
/* 29: Interrupt Enable (1= enabled) */
#define TIMER_LOCAL_TIMER_CTRL_STATUS_INTERRUPT_ENABLE (0x1 << 29)
/* 28: Timer Enable (1= enabled) */
#define TIMER_LOCAL_TIMER_CTRL_STATUS_TIMER_ENABLE (0x1 << 28)
/* 27-0: Re-load Value */
#define TIMER_LOCAL_TIMER_CTRL_STATUS_RELOAD_VALUE_MASK 0xfffffff


/* Timer Local Timer IRQ Clear and Reload (write-only) */
/* 31: Interrupt flag clear when written as 1 (write-only) */
#define TIMER_LOCAL_TIMER_WRITE_FLAGS_INTERRUPT_FLAG_CLEAR (0x1 << 31)
/* 30: Local timer-reloaded when written as 1 (write only) */
#define TIMER_LOCAL_TIMER_WRITE_FLAGS_INTERRUPT_FLAG_CLEAR (0x1 << 31)
/* 29-0: reserved */  



/* Core Timers Interrupt Control Registers */
/* 31-8: reserved */
/* 7: nCNTVIRQ FIQ control.
 * If set, this bit overrides the IRQ bit (3).
 * 0 : FIQ disabled
 * 1 : FIQ Enabled
 */
/* 6: nCNTHPIRQ FIQ control.
 * If set, this bit overrides the IRQ bit (2).
 * 0 : FIQ disabled
 * 1 : FIQ Enabled
 */
/* 5: nCNTPNSIRQ FIQ control.
 * If set, this bit overrides the IRQ bit (1).
 * 0 : FIQ disabled
 * 1 : FIQ Enabled
 */
/* 4: nCNTPSIRQ FIQ control.
 * If set, this bit overrides the IRQ bit (0).
 * 0 : FIQ disabled
 * 1 : FIQ Enabled
 */
/* 3: nCNTVIRQ IRQ control.
 * This bit is only valid if bit 7 is clear otherwise it is ignored.
 * 0 : IRQ disabled
 * 1 : IRQ Enabled
 */
/* 2: nCNTHPIRQ IRQ control.
 * This bit is only valid if bit 6 is clear otherwise it is ignored.
 * 0 : IRQ disabled
 * 1 : IRQ Enabled
 */
/* 1: nCNTPNSIRQ IRQ control.
 * This bit is only valid if bit 5 is clear otherwise it is ignored.
 * 0 : IRQ disabled
 * 1 : IRQ Enabled
 */
/* 0: nCNTPSIRQ IRQ control.
 * This bit is only valid if bit 4 is clear otherwise it is ignored.
 * 0 : IRQ disabled
 * 1 : IRQ Enabled
 */
#define TIMER_CORE_TIMERS_INTERRUPT_CTRL_ENABLE(x) (0x1 << (x))

/* Core Mailboxes Interrupt Control Registers */
/* 31-8: reserved */
/* 7: Mailbox-3 FIQ control.
 * If set, this bit overrides the IRQ bit (3).
 * 0 : FIQ disabled
 * 1 : FIQ Enabled
 */
/* 6: Mailbox-2 FIQ control.
 * If set, this bit overrides the IRQ bit (2).
 * 0 : FIQ disabled
 * 1 : FIQ Enabled
 */
/* 5: Mailbox-1 FIQ control.
 * If set, this bit overrides the IRQ bit (1).
 * 0 : FIQ disabled
 * 1 : FIQ Enabled
 */
/* 4: Mailbox-0 FIQ control.
 * If set, this bit overrides the IRQ bit (0).
 * 0 : FIQ disabled
 * 1 : FIQ Enabled
 */
/* 3: Mailbox-3 IRQ control.
 * This bit is only valid if bit 7 is clear otherwise it is ignored.
 * 0 : IRQ disabled
 * 1 : IRQ Enabled
 */
/* 2: Mailbox-2 IRQ control.
 * This bit is only valid if bit 6 is clear otherwise it is ignored.
 * 0 : IRQ disabled
 * 1 : IRQ Enabled
 */
/* 1: Mailbox-1 IRQ control.
 * This bit is only valid if bit 4 is clear otherwise it is ignored.
 * 0 : IRQ disabled
 * 1 : IRQ Enabled
 */
/* 0: Mailbox-0 IRQ control.
 * This bit is only valid if bit 4 is clear otherwise it is ignored.
 * 0 : IRQ disabled
 * 1 : IRQ Enabled
 */
#define TIMER_CORE_MAILBOXES_INTERRUPT_CTRL_ENABLE(x) (0x1 << (x))


/* Core Interrupt Source Registers */
/* 31-28: reserved */
/* 27-12: Peripheral 1...15 Interrupt (Currently not used) */
#define TIMER_CORE_IRQ_SRC_PERIPHERAL(x) (0x1 << (x))
/* 11: Local Timer Interrupt */
#define TIMER_CORE_IRQ_SRC_LOCAL_TIMER_INTERRUPT (0x1 << 11)
/* 10: AXI-Outstanding Interrupt <For core 0 only!> all others are 0 */
#define TIMER_CORE_IRQ_SRC_AXI_OUTSTANDING_INTERRUPT (0x1 << 10)
/* 9: PMU Interrupt */
#define TIMER_CORE_IRQ_SRC_PMU_INTERRUPT (0x1 << 9)
/* 8: GPU Interrupt <Can be high in one core only> */
#define TIMER_CORE_IRQ_SRC_GPU_INTERRUPT (0x1 << 8)
/* 7: Mailbox 3 Interrupt */
#define TIMER_CORE_IRQ_SRC_MAILBOX3_INTERRUPT (0x1 << 7)
/* 6: Mailbox 2 Interrupt */
#define TIMER_CORE_IRQ_SRC_MAILBOX2_INTERRUPT (0x1 << 6)
/* 5: Mailbox 1 Interrupt */
#define TIMER_CORE_IRQ_SRC_MAILBOX1_INTERRUPT (0x1 << 5)
/* 4: Mailbox 0 Interrupt */
#define TIMER_CORE_IRQ_SRC_MAILBOX0_INTERRUPT (0x1 << 4)
/* 3: CNTVIRQ Interrupt */
#define TIMER_CORE_IRQ_SRC_CNTVIRQ_INTERRUPT (0x1 << 3)
/* 2: CNTHPIRQ Interrupt */
#define TIMER_CORE_IRQ_SRC_CNTHPIRQ_INTERRUPT (0x1 << 2)
/* 1: CNTPNSIRQ Interrupt */
#define TIMER_CORE_IRQ_SRC_CNTPNSIRQ_INTERRUPT (0x1 << 1)
/* 0: CNTPSIRQ Interrupt (Physical Timer -1) */
#define TIMER_CORE_IRQ_SRC_CNTPSIRQ_INTERRUPT (0x1 << 0)

/* Core Fast Interrupt Source Registers */
/* 31-28: reserved */
/* 27-12: Peripheral 1...15 Interrupt (Currently not used) */
#define TIMER_CORE_FIQ_SRC_PERIPHERAL(x) (0x1 << (x))
/* 11: Local Timer Interrupt */
#define TIMER_CORE_FIQ_SRC_LOCAL_TIMER_INTERRUPT (0x1 << 11)
/* 10: reserved */
/* 9: PMU Fast Interrupt */
#define TIMER_CORE_FIQ_SRC_PMU_INTERRUPT (0x1 << 9)
/* 8: GPU Fast Interrupt <Can be high in one core only> */
#define TIMER_CORE_FIQ_SRC_GPU_INTERRUPT (0x1 << 8)
/* 7: Mailbox 3 Fast Interrupt */
#define TIMER_CORE_FIQ_SRC_MAILBOX3_INTERRUPT (0x1 << 7)
/* 6: Mailbox 2 Fast Interrupt */
#define TIMER_CORE_FIQ_SRC_MAILBOX2_INTERRUPT (0x1 << 6)
/* 5: Mailbox 1 Fast Interrupt */
#define TIMER_CORE_FIQ_SRC_MAILBOX1_INTERRUPT (0x1 << 5)
/* 4: Mailbox 0 Fast Interrupt */
#define TIMER_CORE_FIQ_SRC_MAILBOX0_INTERRUPT (0x1 << 4)
/* 3: CNTVIRQ Fast Interrupt */
#define TIMER_CORE_FIQ_SRC_CNTVIRQ_INTERRUPT (0x1 << 3)
/* 2: CNTHPIRQ Fast Interrupt */
#define TIMER_CORE_FIQ_SRC_CNTHPIRQ_INTERRUPT (0x1 << 2)
/* 1: CNTPNSIRQ Fast Interrupt */
#define TIMER_CORE_FIQ_SRC_CNTPNSIRQ_INTERRUPT (0x1 << 1)
/* 0: CNTPSIRQ Fast Interrupt */
#define TIMER_CORE_FIQ_SRC_CNTPSIRQ_INTERRUPT (0x1 << 0)


#endif /*	__MCUBE_ARM_RASPI3_TIMER_H__ */

