/**
 * @file include/x86_64/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_IRQ_H__
#define __MCUBE_X86_64_IRQ_H__

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

/**
 * @var idt_descriptor_size
 * @brief IDT descriptor size.
 */
extern unsigned long idt_descriptor_size;


// IDT memory range
/**
 * @def MEM_IDT
 * @brief Memory IDT.
 */
#define MEM_IDT 0x00001000

/**
 * @def MEM_IDT_SIZE
 * @brief Memory IDT size.
 */
#define MEM_IDT_SIZE (IDT_DESCRIPTOR_SIZE * 256)

// Kernel ISR table
/**
 * @def MEM_ISR_TABLE
 * @brief Memory ISR table.
 */
#define MEM_ISR_TABLE 0x00002000

/**
 * @def MEM_ISR_TABLE_SIZE
 * @brief Memory ISR table size.
 */
#define MEM_ISR_TABLE_SIZE (8 * 256) // Pointer per interrupt

// ISR thunks
/**
 * @def MEM_ISR_THUNKS
 * @brief Memory ISR thunks.
 */
#define MEM_ISR_THUNKS 0x00002800

/**
 * @def MEM_ISR_THUNKS_SIZE
 * @brief Memory ISR thunks size.
 */
#define MEM_ISR_THUNKS_SIZE (17 * 256) // 17 bytes of code per interrupt

// Segment selectors

/**
 * @def SEGMENT_KERNEL_DATA
 * @brief Segment kernel data.
 */
#define SEGMENT_KERNEL_DATA 0x08

/**
 * @def SEGMENT_KERNEL_CODE
 * @brief Segment kernel code.
 */
#define SEGMENT_KERNEL_CODE 0x10

/**
 * @def SEGMENT_USER_DATA
 * @brief Segment user data.
 */
#define SEGMENT_USER_DATA 0x18

/**
 * @def SEGMENT_USER_CODE
 * @brief Segment user code.
 */
#define SEGMENT_USER_CODE 0x20

/**
 * @def SEGMENT_TSS
 * @brief Segment TSS.
 */
#define SEGMENT_TSS 0x28

// CPU exception constants
/**
 * @def EXCEPTION_DIVBYZERO
 * @brief Exception divided by zero.
 */
#define EXCEPTION_DIVBYZERO 0x00

/**
 * @def EXCEPTION_DEBUG
 * @brief Exception debug.
 */
#define EXCEPTION_DEBUG 0x01

/**
 * @def EXCEPTION_NMI
 * @brief Exception Non-Maskable Interrupt.
 */
#define EXCEPTION_NMI 0x02

/**
 * @def EXCEPTION_BREAKPOINT
 * @brief Exception breakpoint.
 */
#define EXCEPTION_BREAKPOINT 0x03

/**
 * @def EXCEPTION_OVERFLOW
 * @brief Exception overflow.
 */
#define EXCEPTION_OVERFLOW 0x04

/**
 * @def EXCEPTION_BOUNDS
 * @brief Exception bounds.
 */
#define EXCEPTION_BOUNDS 0x05

/**
 * @def EXCEPTION_INVALID_OPCODE
 * @brief Exception invalid opcode.
 */
#define EXCEPTION_INVALID_OPCODE 0x06

/**
 * @def EXCEPTION_NO_DEVICE
 * @brief Exception no device.
 */
#define EXCEPTION_NO_DEVICE 0x07

/**
 * @def EXCEPTION_DOUBLE_FAULT
 * @brief Exception double fault.
 */
#define EXCEPTION_DOUBLE_FAULT 0x08

/**
 * @def EXCEPTION_COPROCESSOR
 * @brief Exception coprocessor.
 */
#define EXCEPTION_COPROCESSOR 0x09

/**
 * @def EXCEPTION_INVALID_TSS
 * @brief Exception invalid TSS.
 */
#define EXCEPTION_INVALID_TSS 0x0a

/**
 * @def EXCEPTION_SEGMENT_NOT_PRESENT
 * @brief Exception segment not present.
 */
#define EXCEPTION_SEGMENT_NOT_PRESENT 0x0b

/**
 * @def EXCEPTION_STACK_FAULT
 * @brief Exception stack fault.
 */
#define EXCEPTION_STACK_FAULT 0x0c

/**
 * @def EXCEPTION_GENERAL_PROTECTION
 * @brief Exception general protection.
 */
#define EXCEPTION_GENERAL_PROTECTION 0x0d

/* 0x0f: reserved */
/**
 * @def EXCEPTION_PAGE_FAULT
 * @brief Exception page fault.
 */
#define EXCEPTION_PAGE_FAULT 0x0e

/**
 * @def EXCEPTION_FPU
 * @brief Exception FPU.
 */
#define EXCEPTION_FPU 0x10

/**
 * @def EXCEPTION_ALIGNMENT
 * @brief Exception alignment.
 */
#define EXCEPTION_ALIGNMENT 0x11

/**
 * @def EXCEPTION_MACHINE_CHECK
 * @brief Exception machine check.
 */
#define EXCEPTION_MACHINE_CHECK 0x12

/**
 * @def EXCEPTION_SIMD
 * @brief Exception SIMD.
 */
#define EXCEPTION_SIMD 0x13

/**
 * @def EXCEPTION_VIRTUALIZATION
 * @brief Exception virtualization.
 */
#define EXCEPTION_VIRTUALIZATION 0x14

/* 0x15-1d: reserved */
/**
 * @def EXCEPTION_SECURITY
 * @brief Exception security.
 */
#define EXCEPTION_SECURITY 0x1e
/* 0x1f: reserved */


// PIC port constants
/**
 * @def PIC_PORT_CMD_MASTER
 * @brief Command port for master PIC.
 */
#define PIC_PORT_CMD_MASTER 0x20

/**
 * @def PIC_PORT_CMD_SLAVE
 * @brief Command port for slave PIC.
 */
#define PIC_PORT_CMD_SLAVE 0xa0

/**
 * @def PIC_PORT_DATA_MASTER
 * @brief Data port for master PIC.
 */
#define PIC_PORT_DATA_MASTER 0x21

/**
 * @def PIC_PORT_DATA_SLAVE
 * @brief Data port for slave PIC.
 */
#define PIC_PORT_DATA_SLAVE 0xa1


/**
 * @def PIC0_ICW1
 * @brief PIC 0 Initialization Command Words (ICW) 1.
 */
#define PIC0_ICW1 0x0020

/**
 * @def PIC0_OCW2
 * @brief PIC 0 Operation Command Words (OCW) 2.
 */
#define PIC0_OCW2 0x0020

/**
 * @def PIC0_OCW3
 * @brief PIC 0 Operation Command Words (OCW) 3.
 */
#define PIC0_OCW3 0x0020

/**
 * @def PIC0_IRR
 * @brief PIC 0 Interrput Request Register (IRR).
 */
#define PIC0_IRR 0x0020

/**
 * @def PIC0_ISR
 * @brief PIC 0 In-Service Register (ISR).
 */
#define PIC0_ISR 0x0020

/**
 * @def PIC0_IMR
 * @brief PIC 0 Interrupt Mask Register (IMR).
 */
#define PIC0_IMR 0x0021

/**
 * @def PIC0_OCW1
 * @brief PIC 0 Operation Command Words (OCW) 1.
 */
#define PIC0_OCW1 0x0021

/**
 * @def PIC0_ICW2
 * @brief PIC 0 Initialization Command Words (ICW) 2.
 */
#define PIC0_ICW2 0x0021

/**
 * @def PIC0_ICW3
 * @brief PIC 0 Initialization Command Words (ICW) 3.
 */
#define PIC0_ICW3 0x0021

/**
 * @def PIC0_ICW4
 * @brief PIC 0 Initialization Command Words (ICW) 4.
 */
#define PIC0_ICW4 0x0021

/**
 * @def PIC1_ICW1
 * @brief PIC 1 Initialization Command Words (ICW) 1.
 */
#define PIC1_ICW1 0x00a0

/**
 * @def PIC1_OCW2
 * @brief PIC 1 Operation Command Words (OCW) 2.
 */
#define PIC1_OCW2 0x00a0

/**
 * @def PIC1_OCW3
 * @brief PIC 1 Operation Command Words (OCW) 3.
 */
#define PIC1_OCW3 0x00a0

/**
 * @def PIC1_IRR
 * @brief PIC 1 Interrput Request Register (IRR).
 */
#define PIC1_IRR 0x00a0

/**
 * @def PIC0_ISR
 * @brief PIC 0 In-Service Register (ISR).
 */
#define PIC1_ISR 0x00a0

/**
 * @def PIC1_IMR
 * @brief PIC 1 Interrupt Mask Register (IMR).
 */
#define PIC1_IMR 0x00a1

/**
 * @def PIC1_ICW2
 * @brief PIC 1 Initialization Command Words (ICW) 2.
 */
#define PIC1_ICW2 0x00a1

/**
 * @def PIC1_ICW3
 * @brief PIC 1 Initialization Command Words (ICW) 3.
 */
#define PIC1_ICW3 0x00a1

/**
 * @def PIC1_ICW4
 * @brief PIC 1 Initialization Command Words (ICW) 4.
 */
#define PIC1_ICW4 0x00a1

/**
 * @def PIC1_OCW1
 * @brief PIC 1 Operation Command Words (OCW) 1.
 */
#define PIC1_OCW1 0x00a1

/* Initialization Command Words (ICW) 1 */
/**
 * @def ICW1_INT_VECTOR_ADDRESS(x)
 * @brief 7-5: A7-A5 of interrupt vector address (MCS-80/85 mode only).
 *
 * @param x Value.
 */
#define ICW1_INT_VECTOR_ADDRESS(x) ((x) << 5)

/* 4: Initialization bit. */
/**
 * @def ICW1_INIT_BIT
 * @brief 4: Initialization bit.
 * Set 1 if PIC is to be initialized.
 */
#define ICW1_INIT_BIT (0x1 << 4)

/* 3: Level Trigger Mode. */
/**
 * @def ICW1_EDGE_TRIGGER_MODE
 * @brief 3: Edge Trigger Mode.
 * If LTIM = 1, then the 8259A will operate in the level interrupt mode.
 * Edge detect logic on the interrupt inputs will be disabled.
 */
#define ICW1_EDGE_TRIGGER_MODE (0x0 << 3)

/**
 * @def ICW1_LEVEL_TRIGGER_MODE
 * @brief 3: Level Trigger Mode.
 * If LTIM = 1, then the 8259A will operate in the level interrupt mode.
 * Edge detect logic on the interrupt inputs will be disabled.
 */
#define ICW1_LEVEL_TRIGGER_MODE (0x1 << 3)

/* 2: CALL address interval. */
/**
 * @def ICW1_ADI8
 * @brief 2: CALL address interval.
 * ADI = 1 then interval e 4; ADI e 0 then interval e 8.
 * call address interval. 1 = Interval of 4, 0 = Interval of 8.
 */
#define ICW1_ADI8 (0x0 << 2)

/**
 * @def ICW1_ADI4
 * @brief ADI = 1 then interval e 4; ADI e 0 then interval e 8.
 * call address interval. 1 = Interval of 4, 0 = Interval of 8.
 */
#define ICW1_ADI4 (0x1 << 2)

/* 1: Single. Means that this is the only 8259A in the system. */
/**
 * @def ICW1_CASCADE
 * @brief 0: Cascade.
 */
#define ICW1_CASCADE (0x0 << 1)

/**
 * @def ICW1_SINGLE
 * @brief If SNGL e 1 no ICW3 will be issued.
 */
#define ICW1_SINGLE (0x1 << 1)

/* 0: Initialization Command 4. */
/**
 * @def ICW1_NOT_NEED_ICW4
 * @brief ICW1 not need ICW4.
 * If ICW4 is not needed, set IC4 = 0.
 */
#define ICW1_NOT_NEED_ICW4 (0x0 << 0)

/**
 * @def ICW1_NEED_ICW4
 * @brief ICW1 need ICW4.
 * If this bit is set, ICW4 has to be read.
 */
#define ICW1_NEED_ICW4 (0x1 << 0)

/**
 * @def ICW1_EDGE_TRIGGER
 * @brief ICW1 edge trigger.
 */
#define ICW1_EDGE_TRIGGER (ICW1_INIT_BIT | ICW1_EDGE_TRIGGER_MODE | ICW1_ADI8 \
                           | ICW1_CASCADE | ICW1_NEED_ICW4)

/* Initialization Command Words (ICW)2 */
/**
 * @def ICW2_INT_VECTOR_ADDRESS(x)
 * @brief 7-0: A15-A8 of interrupt vector address (MCS-80/85 mode).
 *
 * @param x Value.
 */
#define ICW2_INT_VECTOR_ADDRESS(x) ((x) << 0)

/**
 * @def ICW2_INT_VECTOR_ADDRESS_T(x)
 * @brief 7-3: T7-T3 of interrupt vector address (8086/8088 mode).
 *
 * @param x Value.
 */
#define ICW2_INT_VECTOR_ADDRESS_T(x) ((x) << 0)

/* Initialization Command Words (ICW) 3 */
/* This word is read only when there is more than one 8259A in the system
 * and cascading is used, in which case ICW1_SINGLE = 0.
 * It will load the 8-bit slave register.
 *
 * a. In the master mode (either when SP e 1, or in buffered mode when M/S e 1
 * in ICW4) a "1" is set for each slave in the system.
 * The master then will release byte 1 of the call sequence (for MCS-80/85 system)
 * and will enable the corresponding slave to release bytes 2 and 3 (for 8086 only byte 2)
 * through the cascade lines.
 *
 * b. In the slave mode (either when SP = 0, or if BUF = 1 and M/S = 0 in ICW4)
 * bits 2-0 identify the slave.
 * The slave compares its cascade input with these bits and, if they are equal,
 * bytes 2 and 3 of the call sequence (or just byte 2 for 8086) are released
 * by it on the Data Bus.
 */
/**
 * @def ICW3_SET_MASTER_DEVICE(x)
 * @brief 7-0: master device.
 * 1 = IR Input has a slave, 0 = IR input does not have a slave.
 *
 * @param x Value.
 */
#define ICW3_SET_MASTER_DEVICE(x) (0x1 << (x))

/**
 * @def ICW3_SET_SLAVE_DEVICE(x)
 * @brief 2-0: slave device. In this case, 7-3 always 0.
 *
 * @param x Value.
 */
#define ICW3_SET_SLAVE_DEVICE(x) (0x1 << (x))


/* Initialization Command Words (ICW) 4. */
/* 7-5: always 0 */

/* 4: Special Fully Nested Mode (SFNM). */
/**
 * @def ICW4_NOT_SFNM
 * @brief ICW4 not Special Fully Nested Mode (SFNM).
 */
#define ICW4_NOT_SFNM (0x0 << 4)

/**
 * @def ICW4_SFNM
 * @brief ICW4 Special Fully Nested Mode (SFNM).
 * If SFNM = 1 the special fully nested mode is programmed.
 */
#define ICW4_SFNM (0x1 << 4)

/*
 * 3-2: Buffer Mode (BUF).
 * If BUF = 1 the buffered mode is programmed.
 * In buffered mode SP/EN becomes an enable output and the master/slave determination is by M/S.
 */
/**
 * @def ICW4_BUF_SLAVE
 * @brief ICW4 buffer slave.
 */
#define ICW4_BUF_SLAVE (0x2 << 2)

/**
 * @def ICW4_BUF_MASTER
 * @brief ICW4 buffer master.
 */
#define ICW4_BUF_MASTER (0x3 << 2)

/* 1: Auto End Of Interrupt (AEOI) */
/**
 * @def ICW4_NORMAL_AOI
 * @brief ICW4 Normal Auto of Interrupt (AOI).
 */
#define ICW4_NORMAL_AOI (0x0 << 1)

/**
 * @def ICW4_AUTO_EOI
 * @brief ICW 4 End Of Interrupt (EOI).
 * If AEOI = 1 the automatic end of interrupt mode is programmed.
 */
#define ICW4_AUTO_EOI (0x1 << 1)

/* 0 Microprocessor Mode: */
/**
 * @def ICW4_MODE_MCS
 * @brief uPM = 0 sets the 8259A for MCS-80, 85 system operation,
 */
#define ICW4_MODE_MCS (0x0 << 0)

/**
 * @def ICW4_MODE_8086
 * @brief uPM = 1 sets the 8259A for 8086 system.
 */
#define ICW4_MODE_8086 (0x1 << 0)

/**
 * @def ICW4_NON_BUFFER
 * @brief ICW4 non buffer.
 */
#define ICW4_NON_BUFFER (ICW4_NOT_SFNM | ICW4_NORMAL_AOI | ICW4_MODE_8086)


/* Operation Command Words (OCW) 1 */

/**
 * @def OCW1_ENABLE_ALL_IRQS
 * @brief enable OCW1 all IRQs.
 */
#define OCW1_ENABLE_ALL_IRQS 0x00

/**
 * @def OCW1_DISABLE_ALL_IRQS
 * @brief disable OCW1 all IRQs.
 */
#define OCW1_DISABLE_ALL_IRQS 0xff


/* Operation Command Words (OCW) 2 */
/* 7-5: End Of Interrupt */
/**
 * @def OCW2_ROT_AUTO_EOI_CLEAR
 * @brief rotate in automatic EOI mode.
 */
#define OCW2_ROT_AUTO_EOI_CLEAR (0x0 << 5)

/**
 * @def OCW2_NON_SPEC_EOI
 * @brief non specific EOI command.
 */
#define OCW2_NON_SPEC_EOI (0x1 << 5)

/* 0x2: no operation */

/**
 * @def OCW2_SPEC_EOI
 * @brief specific EOI command.
 */
#define OCW2_SPEC_EOI (0x3 << 5)

/**
 * @def OCW2_ROT_AUTO_EOI_SET
 * @brief rotate in automatic EOI mode (set).
 */
#define OCW2_ROT_AUTO_EOI_SET (0x4 << 5)

/**
 * @def OCW2_ROT_NON_SPEC_EOI
 * @brief rotate on non specific EOI command.
 */
#define OCW2_ROT_NON_SPEC_EOI (0x5 << 5)

/**
 * @def OCW2_SET_PRIO
 * @brief set priority command.
 */
#define OCW2_SET_PRIO (0x6 << 5)

/**
 * @def OCW2_ROT_SPEC_EOI
 * @brief rotate on specific EOI command.
 */
#define OCW2_ROT_SPEC_EOI (0x7 << 5)

/* 4-3: always 0 */

/**
 * @def OCW2_INT_LEVEL_MASK
 * @brief 2-0: These bits determine the interrupt level acted upon
 * when the SL bit (6 bit) is active.
 */
#define OCW2_INT_LEVEL_MASK 0x7

/* Operation Command Words (OCW) 3 */
/* 7: always 0 */

/**
 * @def OCW3_ESMM
 * @brief 6: Enable Special Mask Mode (ESMM).
 * When this bit is set to 1 it enables the SMM bit to set or reset the
 * Special Mask Mode. When ESMM e 0 the SMM bit becomes a "don't c care".
 */
#define OCW3_ESMM (0x1 << 6)

/**
 * @def OCW3_SMM
 * @brief 5: Special Mask Mode (SMM).
 * If ESMM = 1 and SMM = 1 the 8259A will enter Special Mask Mode.
 * If ESMM = 1 and SMM = 0 the 8259A will revert to normal mask mode.
 * When ESMM = 0, SMM has no effect.
 */
#define OCW3_SMM (0x1 << 5)

/* 4: always 0 */
/* 3: always 1 */

/**
 * @def OCW3_POLL
 * @brief 2: 1 = poll command, 0 = no poll command.
 */
#define OCW3_POLL (0x1 << 2)

/* 1-0: set interrupt request/service register */
/* 0x1-0x0: no action */

/**
 * @def OCW3_INT_REG_COMMAND
 * @brief 0x2: read interrupt request regiser on next read pulse.
 */
#define OCW3_INT_REG_COMMAND (0x2 << 0)

/**
 * @def OCW3_ISR
 * @brief 0x3: read interrupt service register on next read pulse.
 */
#define OCW3_ISR (0x3 << 0)

/**
 * @def NR_IRQS
 * @brief Number of IRQs.
 */
#define NR_IRQS 256

/**
 * @def NR_MASTER_IRQS
 * @brief Number of master IRQs.
 */
#define NR_MASTER_IRQS 8

/**
 * @def NR_SLAVE_IRQS
 * @brief Number of slave IRQs.
 */
#define NR_SLAVE_IRQS 8

/* Master 8259 */
/* IRQ0-7 */

/**
 * @def PIT_IRQ
 * @brief PIT (System timer) IRQ.
 */
#define PIT_IRQ 0

/**
 * @def KEYBOARD_IRQ
 * @brief Keyboard IRQ.
 */
#define KEYBOARD_IRQ 1

/**
 * @def SLAVE_IRQ
 * @brief Cascaded signals from IRQs 8-15 (any devices configured to use IRQ 2
 * will actually be using IRQ 9).
 */
#define SLAVE_IRQ 2

/**
 * @def COM2_4_IRQ
 * @brief Serial port controller for serial port 2
 * (shared with serial port 4, if present) IRQ.
 */
#define COM2_4_IRQ 3

/**
 * @def COM1_3_IRQ
 * @brief Serial port controller for serial port 1
 * (shared with serial port 3, if present) IRQ.
 */
#define COM1_3_IRQ 4

/**
 * @def HDC_IRQ
 * @brief Parallel port 2 and 3 or sound card.
 */
#define HDC_IRQ 5

/**
 * @def FDC_IRQ
 * @brief Floppy Disk Controller (FDC) IRQ.
 */
#define FDC_IRQ 6

/**
 * @def LPT1_IRQ
 * @brief Parallel port 1.
 * It is used for printers or for any parallel port if a printer is not present.
 * It can also be potentially be shared with a secondary sound card
 * with careful management of the port.
 */
#define LPT1_IRQ 7

/* Slave 8259 */
/* IRQ8-15 */

/**
 * @def RTC_IRQ
 * @brief Real-Time Clock (RTC) IRQ.
 */
#define RTC_IRQ 8

/**
 * @def OIA_IRQ1
 * @brief Advanced Configuration and Power Interface (ACPI) system control interrupt
 * on Intel chipsets.
 * Other chipset manufacturers might use another interrupt for this purpose,
 * or make it available for the use of peripherals (any devices configured to use IRQ 2
 * will actually be using IRQ 9).
 */
#define OIA_IRQ1 9

/**
 * @def OIA_IRQ2
 * @brief The Interrupt is left open for the use of peripherals (open interrupt/available,
 * SCSI or NIC).
 */
#define OIA_IRQ2 10

/**
 * @def OIA_IRQ3
 * @brief The Interrupt is left open for the use of peripherals (open interrupt/available,
 * SCSI or NIC).
 */
#define OIA_IRQ3 11

/**
 * @def MOUSE_IRQ
 * @brief Mouse on PS/2 connector.
 */
#define MOUSE_IRQ 12

/**
 * @def NDP_IRQ
 * @brief CPU co-processor or integrated floating point unit or inter-processor interrupt
 * (use depends on OS).
 */
#define NDP_IRQ 13

/**
 * @def HDC1_IRQ
 * @brief Primary ATA channel (ATA interface usually serves hard disk drives
 * and CD drives).
 */
#define HDC1_IRQ 14

/**
 * @def HDC2_IRQ
 * @brief Secondary ATA channel.
 */
#define HDC2_IRQ 15

/**
 * @def RTC_SLAVE_IRQ
 * @brief RTC slave IRQ.
 */
#define RTC_SLAVE_IRQ (RTC_IRQ - NR_MASTER_IRQS)

/**
 * @def OIA_SLAVE_IRQ1
 * @brief OIA slave IRQ 1.
 */
#define OIA_SLAVE_IRQ1 (OIA_IRQ1 - NR_MASTER_IRQS)

/**
 * @def OIA_SLAVE_IRQ2
 * @brief OIA slave IRQ 2.
 */
#define OIA_SLAVE_IRQ2 (OIA_IRQ2 - NR_MASTER_IRQS)

/**
 * @def OIA_SLAVE_IRQ3
 * @brief OIA slave IRQ 3.
 */
#define OIA_SLAVE_IRQ3 (OIA_IRQ3 - NR_MASTER_IRQS)

/**
 * @def MOUSE_SLAVE_IRQ
 * @brief Mouse slave IRQ.
 */
#define MOUSE_SLAVE_IRQ (MOUSE_IRQ - NR_MASTER_IRQS)

/**
 * @def NDP_SLAVE_IRQ
 * @brief NDP slave IRQ.
 */
#define NDP_SLAVE_IRQ (NDP_IRQ - NR_MASTER_IRQS)

/**
 * @def HDC1_SLAVE_IRQ
 * @brief HDC1 slave IRQ.
 */
#define HDC1_SLAVE_IRQ (HDC1_IRQ - NR_MASTER_IRQS)

/**
 * @def HDC2_SLAVE_IRQ
 * @brief HDC2 slave IRQ.
 */
#define HDC2_SLAVE_IRQ (HDC2_IRQ - NR_MASTER_IRQS)


/* 0x20-0xff: maskable interrupts */

/**
 * @def TRAP_IRQ_OFFSET
 * @brief Trap IRQ offset.
 */
#define TRAP_IRQ_OFFSET 0x20

// Interrupt vector numbers: hardware IRQ traps

/**
 * @def TRAP_PIT_IRQ
 * @brief Trap PIT IRQ.
 */
#define TRAP_PIT_IRQ (PIT_IRQ + TRAP_IRQ_OFFSET)

/**
 * @def TRAP_KEYBOARD_IRQ
 * @brief Trap keyboard IRQ.
 */
#define TRAP_KEYBOARD_IRQ (KEYBOARD_IRQ + TRAP_IRQ_OFFSET)


/**
 * @def SYSCALL_IRQ
 * @brief Syscall IRQ.
 */
#define SYSCALL_IRQ 0x40

/**
 * @def LAPIC_TIMER_IRQ
 * @brief LAPIC timer IRQ.
 */
#define LAPIC_TIMER_IRQ 0x41

/**
 * @def TRAP_INTERRUPT_FATAL_IRQ
 * @brief Trap interrupt fatal IRQ.
 */
#define TRAP_INTERRUPT_FATAL_IRQ 0xff

/**
 * @var common_interrupt
 * @brief Common interrupt.
 */
extern uint32_t common_interrupt;

/**
 * @var system_call
 * @brief System call.
 */
extern uint32_t system_call;

/**
 * @fn static inline void enable_local_irq(void)
 * @brief enable local IRQ.
 */
static inline void enable_local_irq(void)
{
  sti();
}

/**
 * @fn static inline void disable_local_irq(void)
 * @brief disable local IRQ.
 */
static inline void disable_local_irq(void)
{
  cli();
}

/**
 * @fn static inline bool is_irq_enabled(unsigned long flags)
 * @brief Is IRQ enabled?
 *
 * @param flags Flags.
 * @return True if IRQ enabled.
 */
static inline bool is_irq_enabled(unsigned long flags)
{
  return flags & CPU_EFLAGS_INTERRUPT;
}

/*
 * Disable interrupts, but restore the original %rflags
 * interrupt enable flag (IF) state afterwards.
 */

/**
 * @fn static inline void save_local_irq(union rflags *flags)
 * @brief save local IRQ.
 *
 * @param flags Flags.
 */
static inline void save_local_irq(union rflags *flags)
{
  *flags = get_rflags();

  if (flags->irqs_enabled) {
    disable_local_irq();
  }
}

/**
 * @fn static inline void restore_local_irq(union rflags *flags)
 * @brief restore local IRQ.
 *
 * @param flags Flags.
 */
static inline void restore_local_irq(union rflags *flags)
{
  if (flags->irqs_enabled) {
    set_rflags(*flags);
  }
}

/**
 * @fn static inline void invalid_opcode(void)
 * @brief Invalid opcode.
 */
static inline void invalid_opcode(void)
{
  asm volatile("int %0" :: "i"(EXCEPTION_INVALID_OPCODE));
}

/**
 * @fn static inline void fatal(void)
 * @brief Fatal.
 */
static inline void fatal(void)
{
  asm volatile("int %0" :: "i"(TRAP_INTERRUPT_FATAL_IRQ));
}


// PIC commands
/**
 * @def PIC_CMD_EOI
 * @brief PIC End of interrupt.
 */
#define PIC_CMD_EOI 0x20

/**
 * @fn void PIC_handler(void)
 * @brief PIC interrupt handler.
 */
void PIC_handler(void);

/**
 * @fn void pic_init(void)
 * @brief initialize PIC.
 */
void pic_init(void);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_IRQ_H__ */
