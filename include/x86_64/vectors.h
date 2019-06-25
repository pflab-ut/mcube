/**
 * @file include/x86_64/vectors.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_VECTORS_H__
#define __MCUBE_X86_64_VECTORS_H__

/*
 * IRQ vectors assignment to bootstrap CPU
 *
 * Copyright (C) 2010-2012 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * We manually assign vector numbers to different IRQ sources till we
 * have a dynamic IRQ model.
 *
 * NOTE! The 32 vectors <= 0x1f are reserved for architecture-defined
 * exceptions and interrupts; don't use them.
 *
 * NOTE! Local & IO APICs only support vectors in range [0x10 -> 0xff].
 * "When an interrupt vector in  the range 0 to 15 is sent or received
 * through the local APIC, the APIC indicates an illegal vector in its
 * Error Status Register. Although vectors [0x10 -> 0x1f] are reserved,
 * the local APIC does not treat them as illegeal."  --Intel
 *
 * Several interrupts might be pending on a CPU instruction boundary;
 * APIC prioritize IRQs based on their IOAPIC-setup vector numbers:
 *
 *  IRQ priority = vector number / 0x10 (highest 4 bits)
 *
 * where the [0x0 -> 0xff] possible vector values lead to:
 *
 *  0x100 / 0x10 = 0x10        (16) IRQ priority classes
 *
 * with `1' being the lowest priority, and `15' being the highest.
 */

// Priority 0xf - Highest priority
/**
 * @def TICKS_IRQ_VECTOR
 * @brief Ticks IRQ vector.
 */
#define TICKS_IRQ_VECTOR 0xf0

/**
 * @def HALT_CPU_IPI_VECTOR
 * @brief Halt CPU IPI vector.
 */
#define HALT_CPU_IPI_VECTOR 0xf1

/**
 * @def APIC_SPURIOUS_VECTOR
 * @brief APIC spurious vector.
 */
#define APIC_SPURIOUS_VECTOR 0xff  // Intel-defined default

// Priority 0x4 - APIC vectors
/**
 * @def APIC_TIMER_VECTOR
 * @brief APIC timer vector.
 */
#define APIC_TIMER_VECTOR 0x40

/**
 * @def APIC_THERMAL_VECTOR
 * @brief APIC thermal vector.
 */
#define APIC_THERMAL_VECTOR 0x41

/**
 * @def APIC_PERFC_VECTOR
 * @brief APIC perfc vector.
 */
#define APIC_PERFC_VECTOR 0x42

/**
 * @def APIC_LINT0_VECTOR
 * @brief APIC local interrupt 0 vector.
 */
#define APIC_LINT0_VECTOR 0x43

/**
 * @def APIC_LINT1_VECTOR
 * @brief APIC local interrupt 1 vector.
 */
#define APIC_LINT1_VECTOR 0x44

// Priority 0x3 - External interrupts
/**
 * @def KEYBOARD_IRQ_VECTOR
 * @brief Keyboard IRQ vector.
 */
#define KEYBOARD_IRQ_VECTOR 0x30

/**
 * @def PIT_TESTS_VECTOR
 * @brief PIT tests vector.
 */
#define PIT_TESTS_VECTOR 0x31

/**
 * @def APIC_TESTS_VECTOR
 * @brief APIC tests vector.
 */
#define APIC_TESTS_VECTOR 0x32

// Priority 0x2 - Lowest possible priority (PIC spurious IRQs)
/**
 * @def PIC_IRQ0_VECTOR
 * @brief PIC IRQ0 vector.
 */
#define PIC_IRQ0_VECTOR 0x20

/**
 * @def PIC_IRQ7_VECTOR
 * @brief PIC IRQ7 vector.
 */
#define PIC_IRQ7_VECTOR (PIC_IRQ0_VECTOR + 7)

/**
 * @def PIC_IRQ8_VECTOR
 * @brief PIC IRQ8 vector.
 */
#define PIC_IRQ8_VECTOR 0x28

/**
 * @def PIC_IRQ15_VECTOR
 * @brief PIC IRQ15 vector.
 */
#define PIC_IRQ15_VECTOR (PIC_IRQ8_VECTOR + 7)

// priority 0x1 - (System reserved)

#endif /* __MCUBE_X86_64_VECTORS_H__ */
