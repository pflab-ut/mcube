/**
 * @file include/x86/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_IRQ_H__
#define __MCUBE_X86_IRQ_H__

#ifndef __ASSEMBLY__


#define PIC0_ICW1 0x0020
#define PIC0_OCW2 0x0020
#define PIC0_OCW3 0x0020
#define PIC0_IRR  0x0020
#define PIC0_ISR  0x0020

#define PIC0_IMR  0x0021
#define PIC0_OCW1 0x0021
#define PIC0_ICW2 0x0021
#define PIC0_ICW3 0x0021
#define PIC0_ICW4 0x0021

#define PIC1_ICW1 0x00a0
#define PIC1_OCW2 0x00a0
#define PIC1_OCW3 0x00a0
#define PIC1_IRR  0x00a0
#define PIC1_ISR  0x00a0

#define PIC1_IMR  0x00a1
#define PIC1_ICW2 0x00a1
#define PIC1_ICW3 0x00a1
#define PIC1_ICW4 0x00a1
#define PIC1_OCW1 0x00a1

/* Initialization Command Words (ICW)1 */
/* 7-5: A7-A5 of interrupt vector address (MCS-80/85 mode only) */
#define ICW1_INT_VECTOR_ADDRESS(x) ((x) << 5)
/* 4: Initialization bit.
	 Set 1 if PIC is to be initialized. */
#define ICW1_INIT_BIT (0x1 << 4)
/* 3: Level Trigger Mode.
	 If LTIM = 1, then the 8259A will operate in the level interrupt mode. Edge detect logic
	 on the interrupt inputs will be disabled. */
#define ICW1_EDGE_TRIGGER_MODE (0x0 << 3)
#define ICW1_LEVEL_TRIGGER_MODE (0x1 << 3)
/* 2: CALL address interval.
	 ADI = 1 then interval e 4; ADI e 0 then interval e 8.
	 call address interval. 1 = Interval of 4, 0 = Interval of 8 */
#define ICW1_ADI8 (0x0 << 2)
#define ICW1_ADI4 (0x1 << 2)
/* 1: Single. Means that this is the only 8259A in
	 the system. If SNGL e 1 no ICW3 will be issued. */
#define ICW1_CASCADE (0x0 << 1)
#define ICW1_SINGLE (0x1 << 1)
/* 0: Initialization Command 4.
	 If this bit is set, ICW4 has to be read.
	 If ICW4 is not needed, set IC4 = 0. */
#define ICW1_NOT_NEED_ICW4 (0x0 << 0)
#define ICW1_NEED_ICW4 (0x1 << 0)

#define ICW1_EDGE_TRIGGER (ICW1_INIT_BIT | ICW1_EDGE_TRIGGER_MODE | ICW1_ADI8 | ICW1_CASCADE | ICW1_NEED_ICW4)

/* Initialization Command Words (ICW)2 */
/* 7-0: A15-A8 of interrupt vector address (MCS-80/85 mode) */
#define ICW2_INT_VECTOR_ADDRESS(x) ((x) << 0)
/* 7-3: T7-T3 of interrupt vector address (8086/8088 mode) */
#define ICW2_INT_VECTOR_ADDRESS_T(x) ((x) << 0)

/* Initialization Command Words (ICW)3 */
/* This word is read only when there is more than one 8259A in the system and cascading is used, in which 
	 case ICW1_SINGLE = 0. It will load the 8-bit slave register.
	 a. In the master mode (either when SP e 1, or in buffered mode when M/S e 1 in ICW4) a "1" is
	 set for each slave in the system.
	 The master then will release byte 1 of the call sequence (for MCS-80/85 system)
	 and will enable the corresponding slave to release bytes 2 and 3 (for 8086 only byte 2)
	 through the cascade lines.
	 b. In the slave mode (either when SP = 0, or if BUF = 1 and M/S = 0 in ICW4) bits 2-0 identify the
	 slave. The slave compares its cascade input with these bits and, if they are equal, bytes 2 and 3 of
	 the call sequence (or just byte 2 for 8086) are released by it on the Data Bus.
*/
/* 7-0: master device */
/* 1 = IR Input has a slave, 0 = IR input does not have a slave */
#define ICW3_SET_MASTER_DEVICE(x) (0x1 << (x))
/* 2-0: slave device. In this case, 7-3 always 0 */
#define ICW3_SET_SLAVE_DEVICE(x) (0x1 << (x))


/* Initialization Command Words (ICW)4 */
/* 7-5: always 0 */
/* 4: Special Fully Nested Mode (SFNM).
	 If SFNM = 1 the special fully nested mode is programmed. */
#define ICW4_NOT_SFNM (0x0 << 4)
#define ICW4_SFNM (0x1 << 4)
/* 3-2: Buffer Mode (BUF).
	 If BUF = 1 the buffered mode is programmed.
	 In buffered mode SP/EN becomes an enable output and the master/slave determination is by M/S. */
#define ICW4_BUF_SLAVE (0x2 << 2)
#define ICW4_BUF_MASTER (0x3 << 2)
/* 1: Auto End Of Interrupt (AEOI).
	 If AEOI = 1 the automatic end of interrupt mode is programmed. */
#define ICW4_NORMAL_AOI (0x0 << 1)
#define ICW4_AUTO_EOI (0x1 << 1)
/* 0 Microprocessor Mode:
	 uPM = 0 sets the 8259A for MCS-80, 85 system operation,
	 uPM = 1 sets the 8259A for 8086 system */
#define ICW4_MODE_MCS (0x0 << 0)
#define ICW4_MODE_8086 (0x1 << 0)

#define ICW4_NON_BUFFER (ICW4_NOT_SFNM | ICW4_NORMAL_AOI | ICW4_MODE_8086)


/* Operation Command Words (OCW)1 */
#define OCW1_ENABLE_ALL_IRQS 0x00
#define OCW1_DISABLE_ALL_IRQS 0xff


/* Operation Command Words (OCW)2 */
/* 7-5: End Of Interrupt */
/* rotate in automatic EOI mode */
#define OCW2_ROT_AUTO_EOI_CLEAR (0x0 << 5)
/* non specific EOI command */
#define OCW2_NON_SPEC_EOI (0x1 << 5)
/* 0x2: no operation */
/* specific EOI command */
#define OCW2_SPEC_EOI (0x3 << 5)
/* rotate in automatic EOI mode (set) */
#define OCW2_ROT_AUTO_EOI_SET (0x4 << 5)
/* rotate on non specific EOI command */
#define OCW2_ROT_NON_SPEC_EOI (0x5 << 5)
/* set priority command */
#define OCW2_SET_PRIO (0x6 << 5)
/* rotate on specific EOI command */
#define OCW2_ROT_SPEC_EOI (0x7 << 5)
/* 4-3: always 0 */
/* 2-0: These bits determine the interrupt level acted upon when the SL bit (6 bit) is active */
#define OCW2_INT_LEVEL_MASK 0x7


/* Operation Command Words (OCW)3 */
/* 7: always 0 */
/* 6: Enable Special Mask Mode (ESMM).
	 When this bit is set to 1 it enables the SMM bit to set or reset the
	 Special Mask Mode. When ESMM e 0 the SMM bit becomes a "don't c care". */
#define OCW3_ESMM (0x1 << 6)
/* 5: Special Mask Mode (SMM).
	 If ESMM = 1 and SMM = 1 the 8259A will enter Special Mask Mode.
	 If ESMM = 1 and SMM = 0 the 8259A will revert to normal mask mode.
	 When ESMM = 0, SMM has no effect. */
#define OCW3_SMM (0x1 << 5)
/* 4: always 0 */
/* 3: always 1 */
/* 2: 1 = poll command, 0 = no poll command */
#define OCW3_POLL (0x1 << 2)
/* 1-0: set interrupt request/service register */
/* 0x1-0x0: no action */
/* 0x2: read interrupt request regiser on next read pulse */
#define OCW3_INT_REG_COMMAND (0x2 << 0)
/* 0x3: read interrupt service register on next read pulse */
#define OCW3_ISR (0x3 << 0)


#define NR_IRQS 256

#define NR_MASTER_IRQS 8
#define NR_SLAVE_IRQS 8

/* Master 8259 */
/* IRQ0-7 */
#define PIT_IRQ 0
#define KEY_IRQ 1
#define SLAVE_IRQ 2
#define COM2_4_IRQ 3
#define COM1_3_IRQ 4
#define HDC_IRQ 5
#define FDC_IRQ 6
#define LPT1_IRQ 7

/* Slave 8259 */
/* IRQ8-15 */
#define RTC_IRQ 8
#define OIA_IRQ1 9
#define OIA_IRQ2 10
#define OIA_IRQ3 11
#define MOUSE_IRQ 12
#define NDP_IRQ 13
#define HDC1_IRQ 14
#define HDC2_IRQ 15

#define RTC_SLAVE_IRQ (RTC_IRQ - NR_MASTER_IRQS)
#define OIA_SLAVE_IRQ1 (OIA_IRQ1 - NR_MASTER_IRQS)
#define OIA_SLAVE_IRQ2 (OIA_IRQ2 - NR_MASTER_IRQS)
#define OIA_SLAVE_IRQ3 (OIA_IRQ3 - NR_MASTER_IRQS)
#define MOUSE_SLAVE_IRQ (MOUSE_IRQ - NR_MASTER_IRQS)
#define NDP_SLAVE_IRQ (NDP_IRQ - NR_MASTER_IRQS)
#define HDC1_SLAVE_IRQ (HDC1_IRQ - NR_MASTER_IRQS)
#define HDC2_SLAVE_IRQ (HDC2_IRQ - NR_MASTER_IRQS)

#define IRQ_OFFSET 0x20


#define TRAP_IRQ_OFFSET 0x30
#define SCHED_IRQ_OFFSET TRAP_IRQ_OFFSET

#define SYSCALL_IRQ 0x80

/* magic number */
#define LAPIC_TIMER_IRQ 0x3e
//#define LAPIC_TIMER_IRQ 0x20
//#define LAPIC_TIMER_IRQ 0x21


extern uint32_t common_interrupt;
extern uint32_t system_call;

extern struct irq_descriptor irq_desc[NR_IRQS];

static inline uint8_t unmask_irq(uint8_t irq)
{
  return ~(0x1 << irq) & 0xff;
}

static inline uint8_t mask_irq(uint8_t irq)
{
  return 0x1 << irq;
}

static inline uint8_t clear_irq(uint8_t irq)
{
	return OCW2_SPEC_EOI | irq;
}


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_IRQ_H__ */
