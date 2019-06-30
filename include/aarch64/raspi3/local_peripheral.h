/**
 * @file include/aarch64/raspi3/local_peripheral.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_RASPI3_LOCAL_PERIPHERAL_H__
#define __MCUBE_AARCH64_RASPI3_LOCAL_PERIPHERAL_H__

/**
 * @def LP_CTRL_REG
 * @brief Local peripheral control register.
 */
#define LP_CTRL_REG (LOCAL_PERIPHERAL_BASE + 0x0)

/* LOCAL_PERIPHERAL_BASE + 0x4: reserved */

/**
 * @def LP_CORE_TIMER_PRESCALER
 * @brief Local peripheral core timer prescaler.
 */
#define LP_CORE_TIMER_PRESCALER (LOCAL_PERIPHERAL_BASE + 0x8)

/**
 * @def LP_GPU_INTERRUPT_ROUTING
 * @brief Local peripheral GPU interrupt routing.
 */
#define LP_GPU_INTERRUPT_ROUTING (LOCAL_PERIPHERAL_BASE + 0xc)

/**
 * @def LP_PERFORMANCE_MONITOR_INTERRUPT_ROUTING_SET
 * @brief Local peripheral performance monitor interrupt routing set.
 */
#define LP_PERFORMANCE_MONITOR_INTERRUPT_ROUTING_SET (LOCAL_PERIPHERAL_BASE + 0x10)

/**
 * @def LP_PERFORMANCE_MONITOR_INTERRUPT_ROUTING_CLEAR
 * @brief Local peripheral performance monitor interrupt routing clear.
 */
#define LP_PERFORMANCE_MONITOR_INTERRUPT_ROUTING_CLEAR (LOCAL_PERIPHERAL_BASE + 0x14)

/* LOCAL_PERIPHERAL_BASE + 0x18: reserved */

/**
 * @def LP_CORE_TIMER_ACCESS_LOWER_32BITS
 * @brief Local peripheral core timer access lower 32bits.
 */
#define LP_CORE_TIMER_ACCESS_LOWER_32BITS (LOCAL_PERIPHERAL_BASE + 0x1c)

/**
 * @def LP_CORE_TIMER_ACCESS_HIGHER_32BITS
 * @brief Local peripheral core timer access higher 32bits.
 */
#define LP_CORE_TIMER_ACCESS_HIGHER_32BITS (LOCAL_PERIPHERAL_BASE + 0x20)

/**
 * @def LP_LOCAL_INTERRUPT_ROUTING
 * @brief Local peripheral local interrupt routing.
 */
#define LP_LOCAL_INTERRUPT_ROUTING (LOCAL_PERIPHERAL_BASE + 0x24)

/* LOCAL_PERIPHERAL_BASE + 0x28: reserved */

/**
 * @def LP_AXI_OUTSTANDING_COUNTERS
 * @brief Local peripheral AXI outstanding counters.
 */
#define LP_AXI_OUTSTANDING_COUNTERS (LOCAL_PERIPHERAL_BASE + 0x2c)

/**
 * @def LP_AXI_OUTSTANDING_IRQ
 * @brief Local peripheral AXI outstanding IRQ.
 */
#define LP_AXI_OUTSTANDING_IRQ (LOCAL_PERIPHERAL_BASE + 0x30)

/**
 * @def LP_LOCAL_TIMER_CTRL_STATUS
 * @brief Local peripheral local timer control status.
 */
#define LP_LOCAL_TIMER_CTRL_STATUS (LOCAL_PERIPHERAL_BASE + 0x34)

/**
 * @def LP_LOCAL_TIMER_WRITE_FLAGS
 * @brief Local peripheral local timer write flags.
 */
#define LP_LOCAL_TIMER_WRITE_FLAGS (LOCAL_PERIPHERAL_BASE + 0x38)

/* LOCAL_PERIPHERAL_BASE + 0x3c: reserved */

/**
 * @def LP_CORE_TIMERS_INTERRUPT_CTRL(cpu)
 * @brief Local peripheral core timers interrupt control.
 *
 * @param cpu CPU.
 */
#define LP_CORE_TIMERS_INTERRUPT_CTRL(cpu) (LOCAL_PERIPHERAL_BASE + 0x4 * (cpu) + 0x40)

/**
 * @def LP_CORE_MAILBOXES_INTERRUPT_CTRL(cpu)
 * @brief Local peripheral core mailboxes interrupt control.
 *
 * @param cpu CPU.
 */
#define LP_CORE_MAILBOXES_INTERRUPT_CTRL(cpu) (LOCAL_PERIPHERAL_BASE + 0x4 * (cpu) + 0x50)

/**
 * @def LP_CORE_IRQ_SRC(cpu)
 * @brief Local peripheral core IRQ source.
 *
 * @param cpu CPU.
 */
#define LP_CORE_IRQ_SRC(cpu) (LOCAL_PERIPHERAL_BASE + 0x4 * (cpu) + 0x60)

/**
 * @def LP_CORE_FIQ_SRC(cpu)
 * @brief Local peripheral core FIQ source.
 *
 * @param cpu CPU.
 */
#define LP_CORE_FIQ_SRC(cpu) (LOCAL_PERIPHERAL_BASE + 0x4 * (cpu) + 0x70)

/**
 * @def LP_CORE_MAILBOX_WRITE_SET(cpu, mb)
 * @brief Local peripheral core mailbox write set.
 *
 * @param cpu CPU.
 * @param mb Mailbox.
 */
#define LP_CORE_MAILBOX_WRITE_SET(cpu, mb) \
  (LOCAL_PERIPHERAL_BASE + 0x10 * (cpu) + 0x4 * (mb) + 0x80)

/**
 * @def LP_CORE_MAILBOX_READ_WRITE_HIGH_TO_CLEAR(cpu, mb)
 * @brief Local peripheral core mailbox read write high to clear.
 *
 * @param cpu CPU.
 * @param mb Mailbox.
 */
#define LP_CORE_MAILBOX_READ_WRITE_HIGH_TO_CLEAR(cpu, mb) \
  (LOCAL_PERIPHERAL_BASE + 0x10 * (cpu) + 0x4 * (mb) + 0xc0)


/* Timer Control Register */
/* 31-10: reserved */
/* 9: Timer increment. */

/**
 * @def LP_CTRL_REG_TIMER_INCREMENT_BY_2
 * @brief 1 = 64-bit Core timer increments by 2.
 */
#define LP_CTRL_REG_TIMER_INCREMENT_BY_2 (0x1 << 9)

/**
 * @def LP_CTRL_REG_TIMER_INCREMENT_BY_1
 * @brief 0 = 64-bit Core timer increments by 1.
 */
#define LP_CTRL_REG_TIMER_INCREMENT_BY_1 (0x0 << 8)

/* 8: Core timer clock source. */
/**
 * @def LP_CTRL_REG_CORE_TIMER_CLOCK_SRC_FROM_APB_CLOCK
 * @brief 1 = 64-bit Core timer runs from the APB clock.
 */
#define LP_CTRL_REG_CORE_TIMER_CLOCK_SRC_FROM_APB_CLOCK (0x1 << 8)

/**
 * @def LP_CTRL_REG_CORE_TIMER_CLOCK_SRC_FROM_CRYSTAL_CLOCK
 * @brief 0 = 64-bit Core timer runs from the Crystal clock.
 */
#define LP_CTRL_REG_CORE_TIMER_CLOCK_SRC_FROM_CRYSTAL_CLOCK (0x0 << 8)

/* 7-0: reserved */

/* Core Timer Pre-Scaler */
/**
 * @def LP_CORE_TIMER_PRESCALER_FREQ(prescaler, input_freq)
 * @brief 31-0: Core timer prescaler.
 * timer_frequency = (2^31 / prescaler) * input frequency (Pre-scaler <= 2^31)
 *
 * @param prescaler Prescaler.
 * @param input_freq Input frequency.
 */
#define LP_CORE_TIMER_PRESCALER_FREQ(prescaler, input_freq) \
  (((0x1 << 31) / (prescaler)) * (input_freq))


/* GPU Interrupt Routing */
/* 31-4: reserved */
/**
 * @def LP_GPU_INTERRUPT_ROUTING_FIQ_CORE(cpu)
 * @brief 3-2: GPU FIQ routing:
 * 00 = GPU FIQ goes to FIQ input of core 0
 * 01 = GPU FIQ goes to FIQ input of core 1
 * 10 = GPU FIQ goes to FIQ input of core 2
 * 11 = GPU FIQ goes to FIQ input of core 3
 *
 * @param cpu CPU.
 */
#define LP_GPU_INTERRUPT_ROUTING_FIQ_CORE(cpu) ((cpu) << 2)

/**
 * @def LP_GPU_INTERRUPT_ROUTING_IRQ_CORE(cpu)
 * @brief 1-0: GPU IRQ routing:
 * 00 = GPU IRQ goes to IRQ input of core 0
 * 01 = GPU IRQ goes to IRQ input of core 1
 * 10 = GPU IRQ goes to IRQ input of core 2
 * 11 = GPU IRQ goes to IRQ input of core 3
 *
 * @param cpu CPU.
 */
#define LP_GPU_INTERRUPT_ROUTING_IRQ_CORE(cpu) ((cpu) << 0)


/* PMU Interrupt Routing Write-set */
/* PMU Interrupt Routing Write-clear */
/* 31-8: reserved */
/* 7: nPMUIRQ[3] FIQ control.
 * If set, this bit overrides the IRQ bit (3).
 * 0 = FIQ disabled
 * 1 = FIQ Enabled
 */
/* 6: nPMUIRQ[2] FIQ control.
 * If set, this bit overrides the IRQ bit (2).
 * 0 = FIQ disabled
 * 1 = FIQ Enabled
 */
/* 5: nPMUIRQ[1] FIQ control.
 * If set, this bit overrides the IRQ bit (1).
 * 0 = FIQ disabled
 * 1 = FIQ Enabled
 */
/* 4: nPMUIRQ[0] FIQ control.
 * If set, this bit overrides the IRQ bit (0).
 * 0 = FIQ disabled
 * 1 = FIQ Enabled
 */
/* 3: nPMUIRQ[3] IRQ control.
 * This bit is only valid if bit 7 is clear otherwise it is ignored.
 * 0 = IRQ disabled
 * 1 = IRQ Enabled
 */
/* 2: nPMUIRQ[2] IRQ control.
 * This bit is only valid if bit 6 is clear otherwise it is ignored.
 * 0 = IRQ disabled
 * 1 = IRQ Enabled
 */
/* 1: nPMUIRQ[1] IRQ control.
 * This bit is only valid if bit 5 is clear otherwise it is ignored.
 * 0 = IRQ disabled
 * 1 = IRQ Enabled
 */
/* 0: nPMUIRQ[0] IRQ control.
 * This bit is only valid if bit 4 is clear otherwise it is ignored.
 * 0 = IRQ disabled
 * 1 = IRQ Enabled
 */
/**
 * @def LP_PERFORMANCE_MONITOR_INTERRUPT_ROUTING_SET_ENABLE(x)
 * @brief Local peripheral performance monitor interrupt routing set enable.
 */
#define LP_PERFORMANCE_MONITOR_INTERRUPT_ROUTING_SET_ENABLE(x) (0x1 << (x))

/**
 * @def LP_PERFORMANCE_MONITOR_INTERRUPT_ROUTING_CLEAR_ENABLE(x)
 * @brief Local peripheral performance monitor interrupt routing clear enable.
 */
#define LP_PERFORMANCE_MONITOR_INTERRUPT_ROUTING_CLEAR_ENABLE(x) (0x1 << (x))


/* Core timer read: LS 32 bits, Write: LS-32 holding register */
/**
 * @def LP_CORE_TIMER_ACCESS_LOWER_32BITS_MASK
 * @brief 31-0: 64-bit core timer read/write, LS 32 bits.
 * When reading returns the current 32 LS bit of the 64 timer and
 * triggers storing a copy of the MS 32 bits.
 * When writing: stores a copy of the 32 bits written.
 * That copy is transferred to the timer when the MS 32 bits are written.
 */
#define LP_CORE_TIMER_ACCESS_LOWER_32BITS_MASK 0xffffffff

/**
 * @def LP_CORE_TIMER_ACCESS_HIGHER_32BITS_MASK
 * @brief 31-0: 64-bit core timer read/write, MS 32 bits
 * When reading returns the status of the core timer-read-hold register.
 * That register is loaded when the user does a read of the LS-32 timer bits.
 * There is little sense in reading this register without first doing a read
 * from the LS-32 bit register.
 * When writing the value is written to the timer, as well as the value
 * previously written to the LS-32 write-holding bit register.
 * There is little sense in writing this register without first doing a write
 * to the LS-32 bit register.
 */
#define LP_CORE_TIMER_ACCESS_HIGHER_32BITS_MASK 0xffffffff


/* Local Interrupt Routing Register */
/* 31-3: reserved */
/**
 * @def LP_LOCAL_INTERRUPT_ROUTING_CORE_IRQ(cpu)
 * @brief 2-0: Interrupt Routing.
 */
/**
 * @def LP_LOCAL_INTERRUPT_ROUTING_CORE_IRQ(cpu)
 * @brief Local peripheral local interrupt routing core IRQ.
 * 000 = Local timer interrupt goes to Core 0 IRQ
 * 001 = Local timer interrupt goes to Core 1 IRQ
 * 010 = Local timer interrupt goes to Core 2 IRQ
 * 011 = Local timer interrupt goes to Core 3 IRQ
 */
#define LP_LOCAL_INTERRUPT_ROUTING_CORE_IRQ(cpu) (cpu)

/**
 * @def LP_LOCAL_INTERRUPT_ROUTING_CORE_FIQ(cpu)
 * @brief Local peripheral local interrupt routing core FIQ.
 * 100 = Local timer interrupt goes to Core 0 FIQ
 * 101 = Local timer interrupt goes to Core 1 FIQ
 * 110 = Local timer interrupt goes to Core 2 FIQ
 * 111 = Local timer interrupt goes to Core 3 FIQ
 */
#define LP_LOCAL_INTERRUPT_ROUTING_CORE_FIQ(cpu) ((cpu) + 0x4)


/* AXI Outstanding Counters */
/* 31-26: 0 */
/**
 * @def LP_AXI_OUTSTANDING_COUNTERS_WRITES_MASK
 * @brief 16-15: Outstanding writes counter.
 */
#define LP_AXI_OUTSTANDING_COUNTERS_WRITES_MASK (0x3 << 15)

/* 15-10: 0 */

/**
 * @def LP_AXI_OUTSTANDING_COUNTERS_READS_MASK
 * @brief 9-0: Outstanding reads counter.
 */
#define LP_AXI_OUTSTANDING_COUNTERS_READS_MASK 0x3ff

/* AXI Outstanding Interrupt */
/* 31-21: 0 */

/**
 * @def LP_AXI_OUTSTANDING_IRQ_AXI_OUTSTANDING_INTERRUPT_ENABLE
 * @brief 20: AXI-outstanding interrupt enable.
 */
#define LP_AXI_OUTSTANDING_IRQ_AXI_OUTSTANDING_INTERRUPT_ENABLE (0x1 << 20)

/**
 * @def LP_AXI_OUTSTANDING_IRQ_AXI_OUTSTANDING_TIMEOUT_MS24BITS_MASK
 * @brief 19-0: AXI-outstanding time-out MS24 bits.
 */
#define LP_AXI_OUTSTANDING_IRQ_AXI_OUTSTANDING_TIMEOUT_MS24BITS_MASK 0xfffff

/* Local Timer Control and Status Register */
/**
 * @def LP_LOCAL_TIMER_CTRL_STATUS_INTERRUPT_FLAG
 * @brief 31: Interrupt flag (Read-Only).
 */
#define LP_LOCAL_TIMER_CTRL_STATUS_INTERRUPT_FLAG (0x1 << 31)

/* 30: reserved */

/**
 * @def LP_LOCAL_TIMER_CTRL_STATUS_INTERRUPT_ENABLE
 * @brief 29: Interrupt Enable (1= enabled).
 */
#define LP_LOCAL_TIMER_CTRL_STATUS_INTERRUPT_ENABLE (0x1 << 29)

/**
 * @def LP_LOCAL_TIMER_CTRL_STATUS_TIMER_ENABLE
 * @brief 28: Timer Enable (1= enabled).
 */
#define LP_LOCAL_TIMER_CTRL_STATUS_TIMER_ENABLE (0x1 << 28)

/**
 * @def LP_LOCAL_TIMER_CTRL_STATUS_RELOAD_VALUE_MASK
 * @brief 27-0: Re-load Value.
 */
#define LP_LOCAL_TIMER_CTRL_STATUS_RELOAD_VALUE_MASK 0xfffffff


/* Timer Local Timer IRQ Clear and Reload (write-only) */

/**
 * @def LP_LOCAL_TIMER_WRITE_FLAGS_INTERRUPT_FLAG_CLEAR
 * @brief 31: Interrupt flag clear when written as 1 (write-only).
 */
#define LP_LOCAL_TIMER_WRITE_FLAGS_INTERRUPT_FLAG_CLEAR (0x1 << 31)

/**
 * @def LP_LOCAL_TIMER_WRITE_FLAGS_LOCAL_TIMER_RELOADED
 * @brief 30: Local timer-reloaded when written as 1 (write only).
 */
#define LP_LOCAL_TIMER_WRITE_FLAGS_LOCAL_TIMER_RELOADED (0x1 << 30)
/* 29-0: reserved */


/* Core Timers Interrupt Control Registers */
/* 31-8: reserved */
/**
 * @def LP_CORE_TIMERS_INTERRUPT_CTRL_CNTVIRQ_FIQ_CTRL
 * @brief 7: nCNTVIRQ FIQ control.
 * If set, this bit overrides the IRQ bit (3).
 * 0 = FIQ disabled
 * 1 = FIQ Enabled
 */
#define LP_CORE_TIMERS_INTERRUPT_CTRL_CNTVIRQ_FIQ_CTRL (0x1 << 7)

/**
 * @def LP_CORE_TIMERS_INTERRUPT_CTRL_CNTHPIRQ_FIQ_CTRL
 * @brief 6: nCNTHPIRQ FIQ control.
 * If set, this bit overrides the IRQ bit (2).
 * 0 = FIQ disabled
 * 1 = FIQ Enabled
 */
#define LP_CORE_TIMERS_INTERRUPT_CTRL_CNTHPIRQ_FIQ_CTRL (0x1 << 6)

/**
 * @def LP_CORE_TIMERS_INTERRUPT_CTRL_CNTPNSIRQ_FIQ_CTRL
 * @brief 5: nCNTPNSIRQ FIQ control.
 * If set, this bit overrides the IRQ bit (1).
 * 0 = FIQ disabled
 * 1 = FIQ Enabled
 */
#define LP_CORE_TIMERS_INTERRUPT_CTRL_CNTPNSIRQ_FIQ_CTRL (0x1 << 5)

/**
 * @def LP_CORE_TIMERS_INTERRUPT_CTRL_CNTPSIRQ_FIQ_CTRL
 * @brief 4: nCNTPSIRQ FIQ control.
 * If set, this bit overrides the IRQ bit (0).
 * 0 = FIQ disabled
 * 1 = FIQ Enabled
 */
#define LP_CORE_TIMERS_INTERRUPT_CTRL_CNTPSIRQ_FIQ_CTRL (0x1 << 4)

/**
 * @def LP_CORE_TIMERS_INTERRUPT_CTRL_CNTVIRQ_IRQ_CTRL
 * @brief 3: nCNTVIRQ IRQ control.
 * This bit is only valid if bit 7 is clear otherwise it is ignored.
 * 0 = IRQ disabled
 * 1 = IRQ Enabled
 */
#define LP_CORE_TIMERS_INTERRUPT_CTRL_CNTVIRQ_IRQ_CTRL (0x1 << 3)

/**
 * @def LP_CORE_TIMERS_INTERRUPT_CTRL_CNTHPIRQ_IRQ_CTRL
 * @brief 2: nCNTHPIRQ IRQ control.
 * This bit is only valid if bit 6 is clear otherwise it is ignored.
 * 0 = IRQ disabled
 * 1 = IRQ Enabled
 */
#define LP_CORE_TIMERS_INTERRUPT_CTRL_CNTHPIRQ_IRQ_CTRL (0x1 << 2)

/**
 * @def LP_CORE_TIMERS_INTERRUPT_CTRL_CNTPNSIRQ_IRQ_CTRL
 * @brief 1: nCNTPNSIRQ IRQ control.
 * This bit is only valid if bit 5 is clear otherwise it is ignored.
 * 0 = IRQ disabled
 * 1 = IRQ Enabled
 */
#define LP_CORE_TIMERS_INTERRUPT_CTRL_CNTPNSIRQ_IRQ_CTRL (0x1 << 1)

/**
 * @def LP_CORE_TIMERS_INTERRUPT_CTRL_CNTPSIRQ_IRQ_CTRL
 * @brief 0: nCNTPSIRQ IRQ control.
 * This bit is only valid if bit 4 is clear otherwise it is ignored.
 * 0 = IRQ disabled
 * 1 = IRQ Enabled
 */
#define LP_CORE_TIMERS_INTERRUPT_CTRL_CNTPSIRQ_IRQ_CTRL (0x1 << 0)


/* Core Mailboxes Interrupt Control Registers */
/* 31-8: reserved */
/* 7: Mailbox-3 FIQ control.
 * If set, this bit overrides the IRQ bit (3).
 * 0 = FIQ disabled
 * 1 = FIQ Enabled
 */
/* 6: Mailbox-2 FIQ control.
 * If set, this bit overrides the IRQ bit (2).
 * 0 = FIQ disabled
 * 1 = FIQ Enabled
 */
/* 5: Mailbox-1 FIQ control.
 * If set, this bit overrides the IRQ bit (1).
 * 0 = FIQ disabled
 * 1 = FIQ Enabled
 */
/* 4: Mailbox-0 FIQ control.
 * If set, this bit overrides the IRQ bit (0).
 * 0 = FIQ disabled
 * 1 = FIQ Enabled
 */
/* 3: Mailbox-3 IRQ control.
 * This bit is only valid if bit 7 is clear otherwise it is ignored.
 * 0 = IRQ disabled
 * 1 = IRQ Enabled
 */
/* 2: Mailbox-2 IRQ control.
 * This bit is only valid if bit 6 is clear otherwise it is ignored.
 * 0 = IRQ disabled
 * 1 = IRQ Enabled
 */
/* 1: Mailbox-1 IRQ control.
 * This bit is only valid if bit 4 is clear otherwise it is ignored.
 * 0 = IRQ disabled
 * 1 = IRQ Enabled
 */
/* 0: Mailbox-0 IRQ control.
 * This bit is only valid if bit 4 is clear otherwise it is ignored.
 * 0 = IRQ disabled
 * 1 = IRQ Enabled
 */
/**
 * @def LP_CORE_MAILBOXES_IRQ_ENABLE(mb)
 * @brief Local peripheral core mailboxes IRQ enable.
 *
 * @param mb Mailbox.
 */
#define LP_CORE_MAILBOXES_IRQ_ENABLE(mb) (0x1 << (mb))

/**
 * @def LP_CORE_MAILBOXES_FIQ_ENABLE(mb)
 * @brief Local peripheral core mailboxes FIQ enable.
 */
#define LP_CORE_MAILBOXES_FIQ_ENABLE(mb) (0x1 << ((mb) + 4))


/* Core Interrupt Source Registers */
/* 31-28: reserved */
/**
 * @def LP_CORE_IRQ_SRC_PERIPHERAL(x)
 * @brief 27-12: Peripheral 1...15 Interrupt (Currently not used).
 */
#define LP_CORE_IRQ_SRC_PERIPHERAL(x) (0x1 << (x))

/**
 * @def LP_CORE_IRQ_SRC_LOCAL_TIMER_INTERRUPT
 * @brief 11: Local Timer Interrupt.
 */
#define LP_CORE_IRQ_SRC_LOCAL_TIMER_INTERRUPT (0x1 << 11)

/**
 * @def LP_CORE_IRQ_SRC_AXI_OUTSTANDING_INTERRUPT
 * @brief 10: AXI-Outstanding Interrupt <For core 0 only!> all others are 0.
 */
#define LP_CORE_IRQ_SRC_AXI_OUTSTANDING_INTERRUPT (0x1 << 10)

/**
 * @def LP_CORE_IRQ_SRC_PMU_INTERRUPT
 * @brief 9: PMU Interrupt.
 */
#define LP_CORE_IRQ_SRC_PMU_INTERRUPT (0x1 << 9)

/**
 * @def LP_CORE_IRQ_SRC_GPU_INTERRUPT
 * @brief 8: GPU Interrupt can be high in one core only.
 */
#define LP_CORE_IRQ_SRC_GPU_INTERRUPT (0x1 << 8)

/**
 * @def LP_CORE_IRQ_SRC_MAILBOX_INTERRUPT(mb)
 * @brief 7-4: Mailboxes 3-0 Interrupts.
 *
 * @param mb Mailbox.
 */
#define LP_CORE_IRQ_SRC_MAILBOX_INTERRUPT(mb) (0x1 << ((mb) + 4))

/**
 * @def LP_CORE_IRQ_SRC_CNTVIRQ_INTERRUPT
 * @brief 3: CNTVIRQ Interrupt.
 */
#define LP_CORE_IRQ_SRC_CNTVIRQ_INTERRUPT (0x1 << 3)

/**
 * @def LP_CORE_IRQ_SRC_CNTHPIRQ_INTERRUPT
 * @brief 2: CNTHPIRQ Interrupt.
 */
#define LP_CORE_IRQ_SRC_CNTHPIRQ_INTERRUPT (0x1 << 2)

/**
 * @def LP_CORE_IRQ_SRC_CNTPNSIRQ_INTERRUPT
 * @brief 1: CNTPNSIRQ Interrupt.
 */
#define LP_CORE_IRQ_SRC_CNTPNSIRQ_INTERRUPT (0x1 << 1)

/**
 * @def LP_CORE_IRQ_SRC_CNTPSIRQ_INTERRUPT
 * @brief 0: CNTPSIRQ Interrupt (Physical Timer -1).
 */
#define LP_CORE_IRQ_SRC_CNTPSIRQ_INTERRUPT (0x1 << 0)

/* Core Fast Interrupt Source Registers */
/* 31-28: reserved */
/**
 * @def LP_CORE_FIQ_SRC_PERIPHERAL(x)
 * @brief 27-12: Peripheral 1...15 Interrupt (Currently not used).
 *
 * @param x Peripheral interrupt.
 */
#define LP_CORE_FIQ_SRC_PERIPHERAL(x) (0x1 << (x))

/**
 * @def LP_CORE_FIQ_SRC_LOCAL_TIMER_INTERRUPT
 * @brief 11: Local Timer Interrupt.
 */
#define LP_CORE_FIQ_SRC_LOCAL_TIMER_INTERRUPT (0x1 << 11)

/* 10: reserved */

/**
 * @def LP_CORE_FIQ_SRC_PMU_INTERRUPT
 * @brief 9: PMU Fast Interrupt.
 */
#define LP_CORE_FIQ_SRC_PMU_INTERRUPT (0x1 << 9)

/**
 * @def LP_CORE_FIQ_SRC_GPU_INTERRUPT
 * @brief 8: GPU Fast Interrupt can be high in one core only.
 */
#define LP_CORE_FIQ_SRC_GPU_INTERRUPT (0x1 << 8)

/**
 * @def LP_CORE_FIQ_SRC_MAILBOX_INTERRUPT(mb)
 * @brief 7-4: Mailboxes 3-0 Interrupts.
 *
 * @param mb Mailbox.
 */
#define LP_CORE_FIQ_SRC_MAILBOX_INTERRUPT(mb) (0x1 << ((mb) + 4))

/**
 * @def LP_CORE_FIQ_SRC_CNTVIRQ_INTERRUPT
 * @brief 3: CNTVIRQ Fast Interrupt.
 */
#define LP_CORE_FIQ_SRC_CNTVIRQ_INTERRUPT (0x1 << 3)

/**
 * @def LP_CORE_FIQ_SRC_CNTHPIRQ_INTERRUPT
 * @brief 2: CNTHPIRQ Fast Interrupt.
 */
#define LP_CORE_FIQ_SRC_CNTHPIRQ_INTERRUPT (0x1 << 2)

/**
 * @def LP_CORE_FIQ_SRC_CNTPNSIRQ_INTERRUPT
 * @brief 1: CNTPNSIRQ Fast Interrupt.
 */
#define LP_CORE_FIQ_SRC_CNTPNSIRQ_INTERRUPT (0x1 << 1)

/**
 * @def LP_CORE_FIQ_SRC_CNTPSIRQ_INTERRUPT
 * @brief 0: CNTPSIRQ Fast Interrupt.
 */
#define LP_CORE_FIQ_SRC_CNTPSIRQ_INTERRUPT (0x1 << 0)


/* Core [cpu] Mailbox [mb] Write-Set Registers */

/**
 * @def LP_MAILBOX0_INTERRUPT_DEBUG
 * @brief Local peripheral mailbox 0 interrupt debug.
 */
#define LP_MAILBOX0_INTERRUPT_DEBUG (0x1 << 0)

/**
 * @def LP_MAILBOX0_INTERRUPT_SCHED
 * @brief Local peripheral mailbox 0 interrupt scheduling.
 */
#define LP_MAILBOX0_INTERRUPT_SCHED (0x1 << 1)

/**
 * @def LP_MAILBOX0_INTERRUPT_CALLBACK0
 * @brief Local peripheral mailbox 0 interrupt callback 0.
 */
#define LP_MAILBOX0_INTERRUPT_CALLBACK0 (0x1 << 2)

/**
 * @def LP_MAILBOX0_INTERRUPT_CALLBACK1
 * @brief Local peripheral mailbox 0 interrupt callback 1.
 */
#define LP_MAILBOX0_INTERRUPT_CALLBACK1 (0x1 << 3)

/**
 * @def LP_MAILBOX0_INTERRUPT_CALLBACK2
 * @brief Local peripheral mailbox 0 interrupt callback 2.
 */
#define LP_MAILBOX0_INTERRUPT_CALLBACK2 (0x1 << 4)

/**
 * @def LP_MAILBOX0_INTERRUPT_CALLBACK3
 * @brief Local peripheral mailbox 0 interrupt callback 3.
 */
#define LP_MAILBOX0_INTERRUPT_CALLBACK3 (0x1 << 5)

/**
 * @def LP_MAILBOX0_INTERRUPT_CALLBACK4
 * @brief Local peripheral mailbox 0 interrupt callback 4.
 */
#define LP_MAILBOX0_INTERRUPT_CALLBACK4 (0x1 << 6)

/**
 * @def LP_MAILBOX0_INTERRUPT_CALLBACK5
 * @brief Local peripheral mailbox 0 interrupt callback 5.
 */
#define LP_MAILBOX0_INTERRUPT_CALLBACK5 (0x1 << 7)

/**
 * @def LP_MAILBOX0_INTERRUPT_CALLBACK6
 * @brief Local peripheral mailbox 0 interrupt callback 6.
 */
#define LP_MAILBOX0_INTERRUPT_CALLBACK6 (0x1 << 8)

/**
 * @def LP_MAILBOX0_INTERRUPT_CALLBACK7
 * @brief Local peripheral mailbox 0 interrupt callback 7.
 */
#define LP_MAILBOX0_INTERRUPT_CALLBACK7 (0x1 << 9)


#ifndef __ASSEMBLY__

/**
 * @fn static inline void enable_mailbox_interrupt(void)
 * @brief enable mailbox interrupt.
 */
static inline void enable_mailbox_interrupt(void)
{
  int i;

  for (i = 0; i < 4; i++) {
    mmio_out32(LP_CORE_MAILBOXES_INTERRUPT_CTRL(i), 0xf);
  }
}

/**
 * @fn static inline void disable_mailbox_interrupt(void)
 * @brief disable mailbox interrupt.
 */
static inline void disable_mailbox_interrupt(void)
{
  int i;

  for (i = 0; i < 4; i++) {
    mmio_out32(LP_CORE_MAILBOXES_INTERRUPT_CTRL(i), 0x0);
  }
}


/**
 * @fn void register_mailbox_callback_handler(callback_t func)
 * @brief register mailbox callback handler.
 *
 * @param func Callback function.
 */
void register_mailbox_callback_handler(callback_t func);


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AARCH64_RASPI3_LOCAL_PERIPHERAL_H__ */

