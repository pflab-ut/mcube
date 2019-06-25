/**
 * @file include/x86_64/idt.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_IDT_H__
#define __MCUBE_X86_64_IDT_H__

/*
 * IDT table descriptor definitions and accessor methods
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 */

/**
 * @def IDT_GATES
 * @brief IDT gates.
 */
#define IDT_GATES (0xff + 1)

/**
 * @def EXCEPTION_GATES
 * @brief Exception gates.
 */
#define EXCEPTION_GATES (0x1f + 1)

/**
 * @def GATE_INTERRUPT
 * @brief Gate interrupt.
 */
#define GATE_INTERRUPT 0xe

/**
 * @def GATE_TRAP
 * @brief Gate trap.
 */
#define GATE_TRAP 0xf

#ifndef __ASSEMBLY__

/**
 * @struct idt_gate
 * @brief IDT gate information.
 */
struct idt_gate {
  /**
   * Offset low.
   */
  uint16_t offset_low;

  /**
   * Selector.
   */
  uint16_t selector;

  uint16_t
  /** IST etc. */
  ist: 3,
       /** Reserved. */
       reserved0: 5,
       /** Type. */
       type: 4,
       /** Reserved. */
       reserved0_1: 1,
       /** DPL. */
       dpl: 2,
       /** P. */
       p: 1;

  /**
   * Offset middle.
   */
  uint16_t offset_middle;

  /**
   * Offset high.
   */
  uint32_t offset_high;

  /**
   * Reserved.
   */
  uint32_t reserved0_2;
} __packed /** packed. */;

/**
 * @struct idt_descriptor
 * @brief IDT descriptor information.
 */
struct idt_descriptor {
  /**
   * Limit.
   */
  uint16_t limit;

  /**
   * Base.
   */
  uint64_t base;
} __packed /** packed. */;

/*
 * Symbols from idt.S
 *
 * Note that 'extern <type> *SYMBOL;' won't work since it'd mean we
 * don't point to meaningful data yet, which isn't the case.
 *
 * We use 'SYMBOL[]' since in a declaration, [] just leaves it open
 * to the number of base type objects which are present, not *where*
 * they are.
 *
 * SYMBOL[n] just adds more static-time safety; SYMBOL[n][size] let
 * the compiler automatically calculate an entry index for us.
 *
 * @IDT_STUB_SIZE: exception stub _code_ size.
 */

/**
 * @var idtdesc
 * @brief IDT descriptor.
 */
extern const struct idt_descriptor idtdesc;

/**
 * @var idt[IDT_GATES]
 * @brief Array of IDT.
 */
extern struct idt_gate idt[IDT_GATES];

/**
 * @def IDT_STUB_SIZE
 * @brief IDT stub size.
 */
#define IDT_STUB_SIZE 12

/**
 * @var idt_exception_stubs[EXCEPTION_GATES][IDT_STUB_SIZE]
 * @brief IDT exception stubs.
 */
extern const char idt_exception_stubs[EXCEPTION_GATES][IDT_STUB_SIZE];

/**
 * @fn void default_irq_handler(void)
 * @brief default IRQ handler.
 */
void default_irq_handler(void);

/**
 * @fn static inline void pack_idt_gate(struct idt_gate *gate, uint8_t type,
 *                                      void *addr)
 * @brief pack IDT gate.
 *
 * @param gate Gate.
 * @param type Type.
 * @param addr Address.
 */
static inline void pack_idt_gate(struct idt_gate *gate, uint8_t type,
                                 void *addr)
{
  gate->offset_low = (uintptr_t) addr & 0xffff;
  gate->selector = KERNEL_CS;
  gate->ist = 0;
  gate->reserved0 = 0;
  gate->type = type;
  gate->reserved0_1 = 0;
  gate->dpl = 0;
  gate->p = 1;
  gate->offset_middle = ((uintptr_t) addr >> 16) & 0xffff;
  gate->offset_high = (uintptr_t) addr >> 32;
  gate->reserved0_2 = 0;
}

/**
 * @fn static inline void write_idt_gate(struct idt_gate *gate, struct idt_gate *idt,
 *                                       unsigned offset)
 * @brief write IDT gate.
 *
 * @param gate Gate.
 * @param idt IDT.
 * @param offset Offset.
 */
static inline void write_idt_gate(struct idt_gate *gate, struct idt_gate *idt,
                                  unsigned offset)
{
  assert(offset < IDT_GATES);
  idt[offset] = *gate;
}

/*
 * The only difference between an interrupt gate and a trap gate
 * is the way the processor handles the IF flag in the EFLAGS.
 *
 * Trap gates leaves the IF flag set while servicing the interrupt,
 * which means handlers can get interrupted indefinitely, and our
 * stack can get overflowed in a matter of milliseconds.
 *
 * Interrupt gates on the other hand clear the IF flag upon entry.
 * A subsequent IRET instruction restores the IF flag to its value
 * in the saved contents.
 */

/**
 * @fn static inline void set_idt_gate(unsigned int n, void *addr)
 * @brief set IDT gate.
 *
 * @param n Number.
 * @param addr Address.
 */
static inline void set_idt_gate(unsigned int n, void *addr)
{
  struct idt_gate gate;
  pack_idt_gate(&gate, GATE_INTERRUPT, addr);
  write_idt_gate(&gate, idt, n);
}

/**
 * @fn static inline void load_idt(const struct idt_descriptor *idt_desc)
 * @brief load IDT.
 *
 * @param idt_desc IDT descriptor.
 */
static inline void load_idt(const struct idt_descriptor *idt_desc)
{
  asm volatile("lidt %0"
               :
               :"m"(*idt_desc));
}

/**
 * @fn static inline struct idt_descriptor get_idt(void)
 * @brief get IDT.
 *
 * @return IDT.
 */
static inline struct idt_descriptor get_idt(void)
{
  struct idt_descriptor idt_desc;

  asm volatile("sidt %0"
               :"=m"(idt_desc)
               :);

  return idt_desc;
}



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_IDT_H__ */

