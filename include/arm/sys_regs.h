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


#endif /* __MCUBE_ARM_SYS_REGS_H__ */
