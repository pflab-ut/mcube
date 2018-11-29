/**
 * @file include/x86/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_IRQ_H__
#define __MCUBE_X86_IRQ_H__

#ifndef __ASSEMBLY__

/*
 * Interrupt memory layout
 *
 *   00001000 - 00001fff     4,096 bytes     Interrupt descriptor table (IDT)
 *   00002000 - 000027ff     2,048 bytes     Kernel-defined ISR table
 *   00002800 - 00002fff     2,048 bytes     ISR thunk table
 *
 * The IDT contains 256 interrupt descriptors, each of which points at one of
 * the internet service routine (ISR) thunks. The thunks prepare a jump to a
 * general-purpose ISR dispatcher, which calls the appropriate ISR from the
 *  kernel-defined ISR table.
 */

extern unsigned long idt_descriptor_size;


// IDT memory range
#define MEM_IDT 0x00001000
#define MEM_IDT_SIZE (IDT_DESCRIPTOR_SIZE * 256)

// Kernel ISR table
#define MEM_ISR_TABLE 0x00002000
#define MEM_ISR_TABLE_SIZE (8 * 256) // Pointer per interrupt

// ISR thunks
#define MEM_ISR_THUNKS 0x00002800
#define MEM_ISR_THUNKS_SIZE (17 * 256) // 17 bytes of code per interrupt

// Segment selectors
#define SEGMENT_KERNEL_DATA 0x08
#define SEGMENT_KERNEL_CODE 0x10
#define SEGMENT_USER_DATA 0x18
#define SEGMENT_USER_CODE 0x20
#define SEGMENT_TSS 0x28

// CPU exception constants used by this code.
#define EXCEPTION_NMI 0x02
#define EXCEPTION_DF 0x08
#define EXCEPTION_TS 0x0a
#define EXCEPTION_NP 0x0b
#define EXCEPTION_SS 0x0c
#define EXCEPTION_GP 0x0d
#define EXCEPTION_PF 0x0e
#define EXCEPTION_MC 0x12


// PIC port constants
#define PIC_PORT_CMD_MASTER   0x20   ///< Command port for master PIC
#define PIC_PORT_CMD_SLAVE    0xa0   ///< Command port for slave PIC
#define PIC_PORT_DATA_MASTER  0x21   ///< Data port for master PIC
#define PIC_PORT_DATA_SLAVE   0xa1   ///< Data port for slave PIC



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
#define KEYBOARD_IRQ 1
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


/* 0x20-0xff: maskable interrupts */
#define TRAP_IRQ_OFFSET 0x20

// Interrupt vector numbers: hardware IRQ traps
#define TRAP_PIT_IRQ          (PIT_IRQ + TRAP_IRQ_OFFSET)
#define TRAP_KEYBOARD_IRQ     (KEYBOARD_IRQ + TRAP_IRQ_OFFSET)


#define SYSCALL_IRQ 0x40
#define LAPIC_TIMER_IRQ 0x41


#define TRAP_INTERRUPT_FATAL_IRQ 0xff


extern uint32_t common_interrupt;
extern uint32_t system_call;


static inline void enable_local_irq(void)
{
	sti();
}

static inline void disable_local_irq(void)
{
	cli();
}

static inline int is_irq_enabled(unsigned long flags)
{
  return flags & CPU_EFLAGS_INTERRUPT;
}


static inline void save_local_irq(unsigned long *flags)
{
	asm volatile("# raw_save_flags\n\t"
							 "pushf ; pop %0"
							 : "=rm" (*flags)
							 :
							 : "memory");
  if (is_irq_enabled(*flags)) {
    disable_local_irq();
  }
}


static inline void restore_local_irq(unsigned long *flags)
{
	asm volatile("push %0 ; popf"
							 :
							 :"g" (*flags)
							 :"memory", "cc");
  if (is_irq_enabled(*flags)) {
    enable_local_irq();
  }
}

static inline void invalid_opcode(void)
{
  asm volatile("int %0" :: "i"(EXCEPTION_INVALID_OPCODE));
}


static inline void fatal(void)
{
  asm volatile("int %0" :: "i"(TRAP_INTERRUPT_FATAL_IRQ));
}


// PIC commands
#define PIC_CMD_EOI           0x20   ///< End of interrupt

//----------------------------------------------------------------------------
//  @struct interrupt_context
/// @brief      A record describing the CPU state at the time of the
///             interrupt.
//----------------------------------------------------------------------------
struct interrupt_context {
    registers_t regs;            ///< all general-purpose registers.
    uint64_t    error;           ///< exception error identifier.
    uint64_t    interrupt;       ///< interrupt vector number.
    uint64_t    retaddr;         ///< interrupt return address.
    uint64_t    cs;              ///< code segment.
    uint64_t    rflags;          ///< flags register.
    uint64_t    rsp;             ///< stack pointer.
    uint64_t    ss;              ///< stack segment.
};

typedef struct interrupt_context interrupt_context_t;

void do_switch_thread_arch(interrupt_context_t *context);

//----------------------------------------------------------------------------
//  @fn         init_irq_asm
/// @brief      Initialize all interrupt tables.
/// @details    Initialize a table of interrupt service routine thunks, one
///             for each of the 256 possible interrupts. Then set up the
///             interrupt descriptor table (IDT) to point to each of the
///             thunks.
///
///             Interrupts should not be enabled until this function has
///             been called.
//----------------------------------------------------------------------------
void init_irq_asm(void);

//----------------------------------------------------------------------------
//  @typedef    isr_handler
/// @brief      Interrupt service routine called when an interrupt occurs.
/// @param[in]  context     The CPU state at the time of the interrupt.
//----------------------------------------------------------------------------
typedef void (*isr_handler)(interrupt_context_t *context);

//----------------------------------------------------------------------------
//  @fn         set_isr
/// @brief      Set an interrupt service routine for the given interrupt
///             number.
/// @details    Interrupts should be disabled while setting these handlers.
///             To disable an ISR, set its handler to null.
/// @param[in]  interrupt   Interrupt number (0-255).
/// @param[in]  handler     Interrupt service routine handler function.
//----------------------------------------------------------------------------
void set_isr(int interrupt, isr_handler handler);

//----------------------------------------------------------------------------
//  @fn         enable_irq
/// @brief      Tell the PIC to enable a hardware interrupt.
/// @param[in]  irq     IRQ number to enable (0-15).
//----------------------------------------------------------------------------
void enable_irq(uint8_t irq);

//----------------------------------------------------------------------------
//  @fn         disable_irq
/// @brief      Tell the PIC to disable a hardware interrupt.
/// @param[in]  irq     IRQ number to enable (0-15).
//----------------------------------------------------------------------------
void disable_irq(uint8_t irq);


static inline uint8_t clear_irq(uint8_t irq)
{
	return OCW2_SPEC_EOI | irq;
}


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_IRQ_H__ */
