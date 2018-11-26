/**
 * @file include/arm/sys_regs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_SYS_REGS_H__
#define __MCUBE_ARM_SYS_REGS_H__

/* CNTV_CTL_EL0, Counter-timer Virtual Timer Control register
 * Page 2188 of AArch64-Reference-Manual.
 */
/* 31-3: reserved */
/* 2: ISTATUS
 * The status of the timer interrupt. This bit is read-only. Permitted values are:
 * 0 : Interrupt not asserted.
 * 1 : Interrupt asserted.
 * A register write that sets IMASK to 1 latches this bit to reflect the status
 * of the interrupt immediately before that write.
 * Reset value is architecturally UNKNOWN.
 */
#define CNTV_CTL_EL0_ISTATUS_NOT_ASSERTED (0x0 << 2)
#define CNTV_CTL_EL0_ISTATUS_ASSERTED (0x1 << 2)
/* 1: IMASK
 * Timer interrupt mask bit. Permitted values are:
 * 0 : Timer interrupt is not masked.
 * 1 : Timer interrupt is masked.
 * Reset value is architecturally UNKNOWN.
 */
#define CNTV_CTL_EL0_IMASK_NOT_MASKED (0x0 << 1)
#define CNTV_CTL_EL0_IMASK_MASKED (0x1 << 1)
/* 0: ENABLE
 * Enables the timer. Permitted values are:
 * 0 : Timer disabled.
 * 1 : Timer enabled.
 * Disabling the timer masks the timer interrupt, but the timer value
 * continues to count down. Resets to 0.
 */
#define CNTV_CTL_EL0_DISABLE (0x0 << 0)
#define CNTV_CTL_EL0_ENABLE (0x1 << 0)


/* ESR_ELx, Exception Syndrome Registers (EL1-3).
 * Page 1899 of AArch64-Reference-Manual.
 */
/* 31-26: Exception Class.
 * Indicates the reason for the exception that this register holds information about.
 */
#define ESR_ELx_EC_SHIFT 26

#define ESR_ELx_EC_UNKNOWN_REASON 0x1
#define ESR_ELx_EC_WFI_OR_WFE_INSTRUCTION_EXCEPTION 0x2
#define ESR_ELx_EC_MCR_OR_MRC_ACCESS_TO_CP15 0x3
#define ESR_ELx_EC_MCRR_OR_MRRC_ACCESS_TO_CP15 0x4
#define ESR_ELx_EC_MCR_OR_MRC_ACCESS_TO_CP14 0x5
#define ESR_ELx_EC_LDC_OR_STC_ACCESS_TO_CP14 0x6
#define ESR_ELx_EC_ACCESS_TO_SIMD_OR_FP_REG 0x7
#define ESR_ELx_EC_MCR_MRC_ACCESS_TO_CP10 0x8
/* 0x9-0xb: reserved */
#define ESR_ELx_EC_MRRC_ACCESS_TO_CP14 0xc
/* 0xd: reserved */
#define ESR_ELx_EC_ILLEGAL_EXECUTION_STATE 0xe
#define ESR_ELx_EC_SVC_INSTRUCTION_EXCEPTION32 0x11
#define ESR_ELx_EC_HVC_INSTRUCTION_EXCEPTION32 0x12
#define ESR_ELx_EC_SMC_INSTRUCTION_EXCEPTION32 0x13
/* 0x14: reserved */
#define ESR_ELx_EC_SVC_INSTRUCTION_EXCEPTION64 0x15
#define ESR_ELx_EC_HVC_INSTRUCTION_EXCEPTION64 0x16
#define ESR_ELx_EC_SMC_INSTRUCTION_EXCEPTION64 0x17
#define ESR_ELx_EC_MSR_MRS_SYSTEM_EXCEPTION64 0x18
/* 0x19-0x1f: reserved */
#define ESR_ELx_EC_INSTRUCTION_ABORT_FROM_LOWER_EXCEPTION_LEVEL 0x20
#define ESR_ELx_EC_INSTRUCTION_ABORT_TAKEN_WITHOUT_A_CHANGE_IN_EXCEPTION_LEVEL 0x21
#define ESR_ELx_EC_MISALIGNED_PC_EXCEPTION 0x22
/* 0x23: reserved */
#define ESR_ELx_EC_DATA_ABORT_FROM_A_LOWER_EXCEPTION_LEVEL 0x24
#define ESR_ELx_EC_DATA_ABORT_TAKEN_WITHOUT_A_CHANGE_IN_EXCEPTION_LEVEL 0x25
#define ESR_ELx_EC_STACK_POINTER_ALIGNMENT_EXCEPTION 0x26
/* 0x27: reserved */
#define ESR_ELx_EC_FLOATING_POINT_EXCEPTION32 0x28
/* 0x29-0x2b: reserved */
#define ESR_ELx_EC_FLOATING_POINT_EXCEPTION64 0x2c
/* 0x2d-0x2e: reserved */
#define ESR_ELx_EC_SERROR_INTERRUPT 0x2f
#define ESR_ELx_EC_BREAKPOINT_EXCEPTION_FROM_A_LOWER_EXCEPTION_LEVEL 0x30
#define ESR_ELx_EC_BREAKPOINT_EXCEPTION_WITHOUT_A_CHANGE_IN_EXCEPTION_LEVEL 0x31
#define ESR_ELx_EC_SOFTWARE_STEP_EXCEPTION_FROM_A_LOWER_EXCEPTION_LEVEL 0x32
#define ESR_ELx_EC_SOFTWARE_STEP_EXCEPTION_TAKEN_WITHOUT_A_CHANGE_IN_EXCEPTION_LEVEL 0x33
#define ESR_ELx_EC_WATCHPOINT_EXCEPTION_FROM_A_LOWER_EXCEPTION_LEVEL 0x34
#define ESR_ELx_EC_WATCHPOINT_EXCEPTION_WITHOUT_A_CHANGE_IN_EXCEPTION_LEVEL 0x35
/* 0x36-0x37: reserved */
#define ESR_ELx_EC_BKPT_INSTRUCTION_EXCEPTION 0x38
/* 0x39: reserved */
#define ESR_ELx_EC_VECTOR_CATCH_EXCEPTION_FROM_AARCH32_STATE 0x3a
/* 0x3b: reserved */
#define ESR_ELx_EC_BRK_INSTRUCTION_EXCEPTION 0x3c
/* 0x3d-0x3f: reserved */


/* 25: Instruction Length for synchronous exceptions. Possible values of this bit are:
 * 0 16-bit instruction trapped.
 * 1 32-bit instruction trapped. This value also applies to the following exceptions:
 ** An SError interrupt.
 ** An Instruction Abort exception.
 ** A Misaligned PC exception.
 ** A Misaligned Stack Pointer exception.
 ** A Data Abort for which the value of the ISV bit is 0.
 ** An Illegal Execution State exception.
 ** Any debug exception except for Software Breakpoint Instruction exceptions.
 ** For Software Breakpoint Instruction exceptions, this bit has its standard meaning:
 ** 0 : 16-bit T32 BKPT instruction.
 ** 1 : 32-bit A32 BKPT instruction or A64 BRK instruction.
 ** An exception reported using EC value 0b000000.
 */
#define ESR_ELx_IL_16BIT_INSTRUCTION_TRAPPED (0x0 << 25)
#define ESR_ELx_IL_32BIT_INSTRUCTION_TRAPPED (0x1 << 25)
/* 24-0: Instruction Specific Syndrome.
 * Architecturally, this field can be defined independently for each defined 
 * Exception class. However, in practice, some ISS encodings are used for more than
 * one Exception class. See the description of the EC field for more information
 * about the ISS formats.
 */
#define ESR_ELx_ISS_MASK 0x1ffffff
/* 24-10: reserved. */
/* 9: EA, External abort type.
 * This bit can provide an IMPLEMENTATION DEFINED classification of synchronous external aborts.
 * For any abort other than a synchronous external abort this bit returns a value of 0.
 */
#define ESR_ELx_ISS_EA (0x1 << 9)
/* 8: reserved. */
#define ESR_ELx_ISS_S1PTW_FAULT_ON_THE_STAGE2 (0x1 << 7)
#define ESR_ELx_ISS_S1PTW_FAULT_NOT_ON_THE_STAGE2 (0x0 << 7)
/* 6: reserved */
/* 5-0: IFSC, Instruction fault status code. Indicates the fault that caused the exception */
#define ESR_ELx_ISS_IFSC_MASK 0x3f
/* 0b0000LL: Address Size fault. The LL bits indicate the level at which the fault occurred.
 * 0b0001LL: Translation fault. The LL bits indicate the level at which the fault occurred.
 * 0b0010LL: Access Flag fault. The LL bits indicate the level at which the fault occurred.
 * 0b0011LL: Permission fault. The LL bits indicate the level at which the fault occurred.
 * 0b010000: Synchronous External abort.
 * 0b011000: Synchronous Parity error on a memory access.
 * 0b0101LL: Synchronous External abort on a translation table walk.
 *           The LL bits indicate the level at which the fault occurred.
 * 0b0111LL: Synchronous Parity error on a memory access on a translation table walk.
 *           The LL bits indicate the level at which the fault occurred.
 * 0b100001: Alignment fault.
 * 0b110000: TLB Conflict fault.
 */


/* SPSR_ELx, Saved Program Status Registers (EL1-3)
 * Page 1417 of AArch64-Reference-Manual.
 */
/* 31-28: Condition flags
 * Shows the values of the condition flags immediately before the exception was taken:
 * N, bit[31] Negative condition flag.
 * Z, bit[30] Zero condition flag.
 * C, bit[29] Carry condition flag.
 * V, bit[28] Overflow condition flag.
 */
#define SPSR_ELx_NEGATIVE_CONDITION_FLAG (0x1 << 31)
#define SPSR_ELx_ZERO_CONDITION_FLAG (0x1 << 30)
#define SPSR_ELx_CARRY_CONDITION_FLAG (0x1 << 29)
#define SPSR_ELx_OVERFLOW_CONDITION_FLAG (0x1 << 28)
/* 27-22: reserved */
/* 21: Software Step bit */
#define SPSR_ELx_SOFTWARE_STEP_BIT (0x1 << 21)
/* 20: Illegal Execution State bit */
#define SPSR_ELx_ILLEGAL_EXCEPTION_STATE_BIT (0x1 << 20)
/* 19-10: reserved */
/* 9: Debug exception mask bit */
#define SPSR_ELx_DEBUG_EXCEPTION_MASK_BIT (0x1 << 9)
/* 8-6: Interrupt mask bits
 * Shows the values of the interrupt mask bits immediately before the exception was taken:
 * A, bit[8] SError interrupt mask bit.
 * I, bit[7] IRQ mask bit.
 * F, bit[6] FIQ mask bit.
 */
#define SPSR_ELx_SERROR_INTERRUPT_MASK_BIT (0x1 << 8)
#define SPSR_ELx_IRQ_MASK_BIT (0x1 << 7)
#define SPSR_ELx_FIQ_MASK_BIT (0x1 << 6)
/* 5: reserved */
/* 4-0: Mode field
 * For exceptions taken from AArch64 state:
 * M[4] The value of this is 0. M[4] encodes the value of PSTATE.nRW.
 * M[3:0] Encodes the Exception level and the stack pointer register selection.
 * 0b1101 EL3h
 * 0b1100 EL3t
 * 0b1001 EL2h
 * 0b1000 EL2t
 * 0b0101 EL1h
 * 0b0100 EL1t
 * 0b0000 EL0t
 */
#define SPSR_ELx_MODE_FIELD_MASK 0x1f
#define SPSR_ELx_MODE_FIELD_EL0t 0x0
/* 0x1-0x3: reserved */
#define SPSR_ELx_MODE_FIELD_EL1t 0x4
#define SPSR_ELx_MODE_FIELD_EL1h 0x5
#define SPSR_ELx_MODE_FIELD_EL2t 0x8
#define SPSR_ELx_MODE_FIELD_EL2h 0x9
#define SPSR_ELx_MODE_FIELD_EL3t 0xa
#define SPSR_ELx_MODE_FIELD_EL3h 0xb



/* PMUSERENR_EL0, Performance Monitors User Enable Register */
/* 31-4: reserved */
/* 3: Event counter read enable.
 * The possible values of this bit are:
 * 0 : EL0 read access to PMXEVCNTR_EL0 / PMEVCNTR<n>_EL0 and read/write access
 * to PMSELR_EL0 disabled if PMUSERENR_EL0.EN is also 0.
 * 1 : EL0 read access to PMXEVCNTR_EL0 / PMEVCNTR<n>_EL0 and read/write access
 * to PMSELR_EL0 enabled.
 * On Warm reset, the field reset value is architecturally UNKNOWN.
 */
#define PMUSERENR_ER (0x1 << 3)
/* 2: Cycle counter read enable.
 * The possible values of this bit are:
 * 0 : EL0 read access to PMCCNTR_EL0 disabled if PMUSERENR_EL0.EN is also 0.
 * 1 : EL0 read access to PMCCNTR_EL0 enabled.
 * On Warm reset, the field reset value is architecturally UNKNOWN.
 */
#define PMUSERENR_CR (0x1 << 2)
/* 1: Software Increment write enable.
 * The possible values of this bit are:
 * 0 : EL0 write access to PMSWINC_EL0 disabled if PMUSERENR_EL0.EN is also 0.
 * 1 : EL0 write access to PMSWINC_EL0 enabled.
 * On Warm reset, the field reset value is architecturally UNKNOWN.
 */
#define PMUSERENR_SW (0x1 << 1)
/* 0: EL0 access enable bit. The possible values of this bit are:
 * 0 : EL0 access to the Performance Monitors disabled.
 * 1 : EL0 access to the Performance Monitors enabled. Can access all PMU registers at EL0,
 * except for writes to PMUSERENR_EL0 and reads/writes of PMINTENSET_EL1 and PMINTENCLR_EL1.
 * On Warm reset, the field reset value is architecturally UNKNOWN.
 */
#define PMUSERENR_EN (0x1 << 0)


/* PMCNTENSET, Performance Monitors Count Enable Set register */
/* 31: PMCCNTR enable bit.
 * Enables the cycle counter register. Possible values are:
 * 0 : When read, means the cycle counter is disabled. When written, has no effect.
 * 1 : When read, means the cycle counter is enabled. When written, enables the cycle counter.
 * On Warm reset, the field reset value is architecturally UNKNOWN.
 */
#define PMCNTENSET_C (0x1 << 31)
/* 30-0: P<x>, bit [x] for x = 0 to (N - 1)
 * Event counter enable bit for PMEVCNTR<x>.
 * * When EL2 is implemented, in Non-secure EL1 and EL0, N is the value in HDCR.HPMN.
 * Otherwise, N is the value in PMCR.N.
 * Bits [30:N] are RAZ/WI.
 * Possible values of each bit are:
 * 0 : When read, means that PMEVCNTR<x> is disabled. When written, has no effect.
 * 1 : When read, means that PMEVCNTR<x> event counter is enabled. When written,
 * enables PMEVCNTR<x>.
 */
#define PMCNTENSET_P(x) (0x1 << (x))


/* PMCR, Performance Monitors Control Register */
/* 31-24: Implementer code.
 * This field is RO with an IMPLEMENTATION DEFINED value.
 * The implementer codes are allocated by ARM. Values have the same interpretation
 * as bits [31:24] of the MIDR.
 */
#define PMCR_IMP_MASK (0xff << 24)
/* 23-16: Identification code.
 * This field is RO with an IMPLEMENTATION DEFINED value.
 * Each implementer must maintain a list of identification codes that is specific
 * to the implementer. A specific implementation is identified by the combination
 * of the implementer code and the identification code.
 */
#define PMCR_IDCODE_MASK (0xff << 16)
/* 15-11: Number of event counters.
 * This field is RO with an IMPLEMENTATION DEFINED value that indicates the number
 * of counters implemented.
 * The value of this field is the number of counters implemented, from 0b00000
 * for no counters to 0b11111 for 31 counters.
 * An implementation can implement only the Cycle Count Register, PMCCNTR.
 * This is indicated by a value of 0b00000 for the N field.
 */
#define PMCR_N_MASK (0xf << 11)
/* 10-7: reserved */
/* 6: Long cycle counter enable.
 * Determines which PMCCNTR bit generates an overflow recorded by PMOVSR[31].
 * 0 : Cycle counter overflow on increment that changes PMCCNTR[31] from 1 to 0.
 * 1 : Cycle counter overflow on increment that changes PMCCNTR[63] from 1 to 0.
 * ARM deprecates use of PMCR.LC = 0.
 * On Warm reset, the field reset value is architecturally UNKNOWN.
 */
#define PMCR_LC (0x1 << 6)
/* 5: Disable cycle counter when event counting is prohibited.
 * The possible values of this bit are:
 * 0 PMCCNTR, if enabled, counts when event counting is prohibited.
 * 1 PMCCNTR does not count when event counting is prohibited.
 * Event counting is prohibited when ProfilingProhibited(IsSecure(),PSTATE.EL) == TRUE.
 * This bit is RW.
 * On Warm reset, the field resets to 0.
 */
#define PMCR_DP (0x1 << 5)
/* 4: Enable export of events in an IMPLEMENTATION DEFINED event stream.
 * The possible values of this bit are:
 * 0 : Do not export events.
 * 1 : Export events where not prohibited.
 * This bit is used to permit events to be exported to another debug device,
 * such as an OPTIONAL trace extension, over an event bus.
 * If the implementation does not include such an event bus, this bit is RAZ/WI.
 * This bit does not affect the generation of Performance Monitors overflow
 * interrupt requests or signaling to a cross-trigger interface (CTI) that can be
 * implemented as signals exported from the processor.
 * If the implementation does not include an exported event stream, this bit is RAZ/WI.
 * Otherwise this bit is RW.
 * On Warm reset, the field resets to 0.
 */
#define PMCR_X (0x1 << 4)
/* 3: Clock divider.
 * The possible values of this bit are:
 * 0 : When enabled, PMCCNTR counts every clock cycle.
 * 1 : When enabled, PMCCNTR counts once every 64 clock cycles.
 * This bit is RW.
 * If PMCR.LC == 1, this bit is ignored and the cycle counter counts every clock cycle.
 * ARM deprecates use of PMCR.D = 1.
 * On Warm reset, the field resets to 0.
 */
#define PMCR_D (0x1 << 3)
/* 2: Cycle counter reset.
 * This bit is WO. The effects of writing to this bit are:
 * 0 : No action.
 * 1 : Reset PMCCNTR to zero.
 * This bit is always RAZ.
 * Resetting PMCCNTR does not clear the PMCCNTR overflow bit to 0.
 * On Warm reset, the field reset value is architecturally UNKNOWN.
 */
/* 1: Event counter reset.
 * This bit is WO.
 * The effects of writing to this bit are:
 * 0 : No action.
 * 1 : Reset all event counters accessible in the current EL, not including PMCCNTR, to zero.
 * This bit is always RAZ.
 * In Non-secure EL0 and EL1, if EL2 is implemented, a write of 1 to this bit does not reset event
 * counters that HDCR.HPMN reserves for EL2 use.
 * In EL2 and EL3, a write of 1 to this bit resets all the event counters.
 * Resetting the event counters does not clear any overflow bits to 0.
 * On Warm reset, the field reset value is architecturally UNKNOWN.
 */
#define PMCR_P (0x1 << 1)
/* 0: Enable. The possible values of this bit are:
 * 0 : All counters, including PMCCNTR, are disabled.
 * 1 : All counters are enabled by PMCNTENSET.
 * This bit is RW.
 * In Non-secure EL0 and EL1, if EL2 is implemented, this bit does not affect
 * the operation of event counters that HDCR.HPMN reserves for EL2 use.
 * On Warm reset, the field resets to 0.
 */
#define PMCR_E (0x1 << 0)

#endif /* __MCUBE_ARM_SYS_REGS_H__ */
