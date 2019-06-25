/**
 * @file include/x86_64/ioapic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_IOAPIC_H__
#define __MCUBE_X86_64_IOAPIC_H__

/*
 * I/O APIC definitions
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 */


#ifndef __ASSEMBLY__

/**
 * @struct ioapic_desc
 * @brief I/O APIC descriptors.
 *
 * System-wide I/O APIC descriptors for each I/O APIC reported
 * by the BIOS as usable. At lease one ioapic should be enabled.
 */
struct ioapic_desc {
  /**
   * Chip APIC ID.
   */
  uint8_t id;

  /**
   * Chip version: 0x11, 0x20, ..
   */
  uint8_t version;

  /**
   * This IOAPIC physical base address.
   */
  uint32_t base;

  /**
   * IRQs = (0 - max_irq) inclusive.
   */
  uint8_t max_irq;
};

/**
 * @var nr_ioapics
 * @brief (MP, ACPI) usable I/O APICs count.
 */
extern int nr_ioapics;

/**
 * @def IOAPICS_MAX
 * @brief Arbitary.
 */
#define IOAPICS_MAX 8

/* FIXME: locks around the global resource array once SMP
 * primitive locking is done */
/**
 * @var ioapic_descs[IOAPICS_MAX]
 * @brief I/O APIC descriptors.
 */
extern struct ioapic_desc ioapic_descs[IOAPICS_MAX];

/*
 * I/O APIC registers and register accessors
 */

/**
 * @def IOAPIC_ID
 * @brief I/O APIC ID.
 */
#define IOAPIC_ID 0x00

/**
 * @union ioapic_id
 * @brief I/O APIC ID.
 */
union ioapic_id {
  /**
   * Value.
   */
  uint32_t value;
  struct {
    /**
     * ID.
     */
    uint32_t reserved0: 24, id: 8;
  } __packed /** packed. */;
};

/**
 * @def IOAPIC_VER
 * @brief I/O APIC version.
 */
#define IOAPIC_VER 0x01

/**
 * @union ioapic_ver
 * @brief I/O APIC Version.
 */
union ioapic_ver {
  /**
   * Value.
   */
  uint32_t value;
  struct {
    /**
     * Version etc.
     */
    uint32_t version: 8, reserved0: 8,
             max_irq: 8, reserved1: 8;
  } __packed /** packed. */;
};

/**
 * @def IOAPIC_ARB
 * @brief I/O APIC arbitration.
 */
#define IOAPIC_ARB 0x02

/**
 * @union ioapic_arb
 * @brief I/O APIC arbitration.
 */
union ioapic_arb {
  /**
   * Value.
   */
  uint32_t value;
  struct {
    /**
     * Arbitration.
     */
    uint32_t reserved0: 24, arbitration: 4,
             reserved1: 4;
  } __packed /** packed. */;
};

/**
 * @def IOAPIC_MMIO_SPACE
 * @brief IOAPIC memory mapped registers address space size.
 */
#define IOAPIC_MMIO_SPACE 0x20

/**
 * @fn static inline uintptr_t ioapic_base(int apic)
 * @brief get the I/O APIC virtual base from the IOAPICs repository.
 * The data was found either by parsing the MP-tables or ACPI.
 *
 * @param apic APIC address.
 * @return I/O APIC virtual base.
 */
static inline uintptr_t ioapic_base(int apic)
{
  void *vbase;

  assert(apic < nr_ioapics);
  vbase = vm_kmap(ioapic_descs[apic].base, IOAPIC_MMIO_SPACE);

  return (uintptr_t) vbase;
}

/**
 * @fn static inline uint32_t ioapic_read(int apic, uint8_t reg)
 * @brief read I/O APIC.
 *
 * @param apic APIC address.
 * @param reg Register.
 * @return I/O APIC data.
 */
static inline uint32_t ioapic_read(int apic, uint8_t reg)
{
  uint32_t *ioregsel = (uint32_t *)ioapic_base(apic);
  uint32_t *iowin = (uint32_t *)(ioapic_base(apic) + 0x10);

  mmio_out32(ioregsel, reg);
  return mmio_in32(iowin);
}

/**
 * @fn static inline void ioapic_write(int apic, uint8_t reg, uint32_t value)
 * @brief write I/O APIC.
 *
 * @param apic APIC address.
 * @param reg Register.
 * @param value Value.
 */
static inline void ioapic_write(int apic, uint8_t reg, uint32_t value)
{
  uint32_t *ioregsel = (uint32_t *) ioapic_base(apic);
  uint32_t *iowin = (uint32_t *)(ioapic_base(apic) + 0x10);

  mmio_out32(ioregsel, reg);
  mmio_out32(iowin, value);
}

/**
 * @def IOAPIC_REDTBL0
 * @brief I/O APIC redirection table.
 */
#define IOAPIC_REDTBL0 0x10


/**
 * @union ioapic_irqentry
 * @brief I/O APIC irq entry.
 *
 * Don't use a single uint64_t element here. All APIC registers are
 * accessed using 32 bit loads and stores. Registers that are
 * described as 64 bits wide are accessed as multiple independent
 * 32 bit registers -- Intel 82093AA datasheet.
 */
union ioapic_irqentry {
  struct {
    /**
     * Vector etc.
     */
    uint32_t vector: 8, delivery_mode: 3, dst_mode: 1,
             delivery_status: 1, polarity: 1, remote_irr: 1,
             trigger: 1, mask: 1, reserved0: 15;

    /**
     * Destination.
     */
    uint32_t reserved1: 24, dst: 8;
  } __packed /** packed. */;
  struct {
    /**
     * Value low.
     */
    uint32_t value_low;

    /**
     * Value high.
     */
    uint32_t value_high;
  } __packed /** packed. */;

  /**
   * Value.
   */
  uint64_t value;
};

/**
 * @enum ioapic_delmod
 * @brief Delivery mode (R/W).
 */
enum ioapic_delmod {
  IOAPIC_DELMOD_FIXED = 0x0,
  IOAPIC_DELMOD_LOWPR = 0x1,
  IOAPIC_DELMOD_SMI   = 0x2,
  IOAPIC_DELMOD_NMI   = 0x4,
  IOAPIC_DELMOD_INIT  = 0x5,
  IOAPIC_DELMOD_EXTINT = 0x7,
};

/**
 * @enum ioapic_dstmod
 * @brief Destination mode (R/W).
 */
enum ioapic_dstmod {
  IOAPIC_DSTMOD_PHYSICAL = 0x0,
  IOAPIC_DSTMOD_LOGICAL  = 0x1,
};

/**
 * @enum ioapic_polarity
 * @brief Interrupt Input Pin Polarity (R/W).
 */
enum ioapic_polarity {
  IOAPIC_POLARITY_HIGH = 0x0,
  IOAPIC_POLARITY_LOW  = 0x1,
};

/**
 * @enum ioapic_trigger
 * @brief Trigger Mode (R/W).
 */
enum ioapic_trigger {
  IOAPIC_TRIGGER_EDGE  = 0x0,
  IOAPIC_TRIGGER_LEVEL = 0x1,
};

/**
 * @enum ioapic_mask
 * @brief Interrupt Mask (R/W).
 */
enum ioapic_mask {
  IOAPIC_UNMASK = 0x0,
  IOAPIC_MASK   = 0x1,
};

/**
 * @enum ioapic_dst
 * @brief Message Destination Address (APIC logical destination mode).
 */
enum ioapic_dst {
  /* Each local APIC performs a logical AND of chosen
   * address and its logical APIC ID. If a 'true'
   * condition was detected, the IRQ is accepted. */
  IOAPIC_DST_BROADCAST = 0xff,
};

/**
 * @fn static inline union ioapic_irqentry ioapic_read_irqentry(int apic, uint8_t irq)
 * @brief read I/O APIC IRQ entry.
 *
 * @param apic APIC ID.
 * @param irq IRQ.
 * @return I/O APIC IRQ entry.
 */
static inline union ioapic_irqentry ioapic_read_irqentry(int apic, uint8_t irq)
{
  union ioapic_irqentry entry = { .value = 0 };
  entry.value_low = ioapic_read(apic, IOAPIC_REDTBL0 + 2 * irq);
  entry.value_high = ioapic_read(apic, IOAPIC_REDTBL0 + 2 * irq + 1);
  return entry;
}

/**
 * @fn static inline void ioapic_write_irqentry(int apic, uint8_t irq,
 *                                              union ioapic_irqentry entry)
 * @brief write I/O APIC IRQ entry.
 *
 * NOTE! Write the upper half _before_ writing the lower half.
 * The low word contains the mask bit, and we want to be sure
 * of the irq entry integrity if the irq is going to be enabled.
 */
static inline void ioapic_write_irqentry(int apic, uint8_t irq,
                                         union ioapic_irqentry entry)
{
  ioapic_write(apic, IOAPIC_REDTBL0 + 2 * irq + 1, entry.value_high);
  ioapic_write(apic, IOAPIC_REDTBL0 + 2 * irq, entry.value_low);
}


/**
 * @fn static inline void ioapic_mask_irq(int apic, uint8_t irq)
 * @brief mask I/O APIC IRQ.
 *
 * @param apic APIC ID.
 * @param irq IRQ.
 */
static inline void ioapic_mask_irq(int apic, uint8_t irq)
{
  union ioapic_irqentry entry = { .value = 0 };
  entry.value_low = ioapic_read(apic, IOAPIC_REDTBL0 + 2 * irq);
  entry.mask = IOAPIC_MASK;
  ioapic_write(apic, IOAPIC_REDTBL0 + 2 * irq, entry.value_low);
}

/**
 * @struct ioapic_pin
 * @brief I/O APIC pin.
 *
 * Represents where an interrupt source is connected to the
 * I/O APICs system
 */
struct ioapic_pin {
  /**
   * which ioapic?
   */
  int apic;

  /**
   * which pin in this I/O APIC.
   */
  int pin;
};

/**
 * @fn void ioapic_setup_isairq(uint8_t irq, uint8_t vector, enum irq_dst dst)
 * @brief setup the I/O APIC routing entry representing given
 * ISA @a irq, pointing it to @a dst CPUs IDT @a vector.
 *
 * NOTE! You need to install a @a vector handler before
 * setting up its respective I/O APIC entry.
 *
 * @param irq IRQ.
 * @param vector Vector.
 * @param dst Destination.
 */
void ioapic_setup_isairq(uint8_t irq, uint8_t vector, enum irq_dst dst);

/**
 * @fn void ioapic_init(void)
 * @brief initialize I/O APIC.
 */
void ioapic_init(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_IOAPIC_H__ */
