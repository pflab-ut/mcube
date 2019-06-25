/**
 * @file include/x86_64/mptables.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_MPTABLES_H__
#define __MCUBE_X86_64_MPTABLES_H__

/*
 * Intel MultiProcessor Specification v1.4 tables
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * NOTE! MP strings are coded in ASCII, but are not NULL-terminated.
 */

#ifndef __ASSEMBLY__


/*
 * Intel MP Floating Pointer structure
 *
 * ASCII signature serves as a search key for locating the
 * structure. Note the little-endian byte ordering.
 */

/**
 * @def MPF_SIGNATURE
 * @brief MP floating signature.
 */
#define MPF_SIGNATURE ('_'<<(3 * 8) | 'P'<<(2 * 8) | 'M'<<(1 * 8) | '_')

/**
 * @struct mpf_struct
 * @brief MP floating information.
 */
struct mpf_struct {
  /**
   * "_MP_".
   */
  uint32_t signature;

  /**
   * MP configuration table pointer.
   */
  uint32_t conf_physaddr;

  /**
   * Structure length in 16-byte units.
   */
  uint8_t length;

  /**
   * version1.1=0x01, version1.4=0x04.
   */
  uint8_t version;

  /**
   * Sum of structure's bytes.
   */
  uint8_t checksum;

  /**
   * Default configuration used?
   */
  uint8_t feature1;

  /**
   * IMCR.
   */
  uint8_t
  /** Reserved. */
  feature2_reserved: 7,
                     /** Obsolete PIC mode implemented? */
                     imcr: 1;

  /**
   * Reserved.
   */
  uint8_t feature3;

  /**
   * Reserved.
   */
  uint8_t feature4;

  /**
   * Reserved.
   */
  uint8_t feature5;
} __packed /** packed. */;

/*
 * MP Configuration table header
 */

/**
 * @def MPC_SIGNATURE
 * @brief MP configuration signature.
 */
#define MPC_SIGNATURE ('P'<<(3 * 8) | 'M'<<(2 * 8) | 'C'<<(1 * 8) | 'P')

/**
 * @struct mpc_table
 * @brief MP configuration table.
 */
struct mpc_table {
  /**
   * "PCMP".
   */
  uint32_t signature;

  /**
   * Base Table length + header.
   */
  uint16_t length;

  /**
   * version1.1=0x01, version1.4=0x04.
   */
  uint8_t version;

  /**
   * mpf checksum.
   */
  uint8_t checksum;

  /**
   * OEM ID.
   */
  char oem[8];

  /**
   * Product ID/family.
   */
  char product[12];

  /**
   * OEM table physical pointer, or 0.
   */
  uint32_t oem_physaddr;

  /**
   * OEM table size in bytes, or 0.
   */
  uint16_t oem_size;

  /**
   * Num of table's entries.
   */
  uint16_t entries;

  /**
   * Obsolete LAPIC base - use MSRs.
   */
  uint32_t lapic_base;

  /**
   * Extended entries length, or 0.
   */
  uint16_t ext_length;

  /**
   * Extended entries checksum, or 0.
   */
  uint8_t ext_checksum;

  /**
   * Reserved.
   */
  uint8_t reserved;
} __packed /** packed. */;

/**
 * @struct mpc_cpu
 * @brief MP configuration table entries.
 */
struct mpc_cpu {
  /**
   * Entry type (processor).
   */
  uint8_t entry;

  /**
   * This processor's lapic ID.
   */
  uint8_t lapic_id;

  /**
   * This proecessor's lapic version.
   */
  uint8_t lapic_ver;

  /**
   * Flags.
   */
  uint8_t
  /** Set if this processor is usable. */
  enabled: 1,
           /** Set for the bootstrap processor. */
           bsc: 1,
           /** Reserved. */
           flags_reserved: 6;

  /**
   * Signature (stepping, model, family.
   */
  uint32_t signature;

  /**
   * Flags as returned by CPUID.
   */
  uint32_t flags;

  /**
   * Reserved.
   */
  uint64_t reserved;
} __packed /** packed. */;

/**
 * @struct mpc_bus
 * @brief MP configuration bus.
 */
struct mpc_bus {
  /**
   * Entry type (bus).
   */
  uint8_t entry;

  /**
   * Bus ID.
   */
  uint8_t id;

  /**
   * Bus Type string.
   */
  char type[6];
} __packed /** packed. */;

/**
 * @struct mpc_ioapic
 * @brief MP configuration I/O APIC.
 */
struct mpc_ioapic {
  /**
   * Entry type (I/O APIC).
   */
  uint8_t entry;

  /**
   * The ID of this I/O APIC.
   */
  uint8_t id;

  /**
   * I/O APIC's version register.
   */
  uint8_t version;

  /**
   * Flags.
   */
  uint8_t
  /** If zero, this I/O APIC is unusable. */
  enabled: 1,
           /** Reserved. */
           flags_reserved: 7;

  /**
   * This I/O APIC base address.
   */
  uint32_t base;
} __packed /** packed. */;

/**
 * @struct mpc_irq
 * @brief MP configuration IRQ.
 */
struct mpc_irq {
  /**
   * Entry type (I/O interrupt entry).
   */
  uint8_t entry;

  /**
   * Interrupt type.
   */
  uint8_t type;

  /**
   * Flags.
   */
  uint16_t
  /** Polarity of APIC I/O input signal. */
  polarity: 2,
            /** Trigger mode. */
            trigger: 2,
            /* Reserved. */
            reserved: 12;

  /**
   * Interrupt source bus.
   */
  uint8_t src_busid;

  /**
   * Source bus IRQ.
   */
  uint8_t src_busirq;

  /**
   * Destination I/O APIC ID.
   */
  uint8_t dst_ioapicid;

  /**
   * Destination I/O APIC INTINn pin.
   */
  uint8_t dst_ioapicpin;
} __packed /** packed. */;

/**
 * @struct mpc_linterrupt
 * @brief MP configuration local interrupt.
 */
struct mpc_linterrupt {
  /**
   * Entry type (local interrupt entry).
   */
  uint8_t entry;

  /**
   * Interrupt type.
   */
  uint8_t type;

  /**
   * Flags.
   */
  uint16_t
  /** Polarity of APIC I/O input signal. */
  polarity: 2,
            /** Trigger mode. */
            trigger: 2,
            /** Reserved. */
            reserved: 12;

  /**
   * Interrupt source bus.
   */
  uint8_t src_busid;

  /**
   * Source bus IRQ.
   */
  uint8_t src_busirq;

  /**
   * Destination local APIC ID.
   */
  uint8_t dst_lapic;

  /**
   * Destination local APIC LINTINn pin.
   */
  uint8_t dst_lapicpin;
} __packed /** packed. */;

/**
 * @def MPC_ENTRY_MAX_LEN
 * @brief MPC entry maximum length.
 */
#define MPC_ENTRY_MAX_LEN (sizeof(struct mpc_cpu))

/**
 * @fn static inline void mptables_check(void)
 * @brief Compile-time MP tables sizes sanity checks.
 */
static inline void mptables_check(void)
{
  compiler_assert(sizeof(struct mpf_struct) == 4 * 4);
  compiler_assert(sizeof(struct mpc_table) == 11 * 4);
  compiler_assert(sizeof(struct mpc_cpu) == 5 * 4);
  compiler_assert(sizeof(struct mpc_bus) == 2 * 4);
  compiler_assert(sizeof(struct mpc_ioapic) == 2 * 4);
  compiler_assert(sizeof(struct mpc_irq) == 2 * 4);
  compiler_assert(sizeof(struct mpc_linterrupt) == 2 * 4);

  compiler_assert(MPC_ENTRY_MAX_LEN >= sizeof(struct mpc_cpu));
  compiler_assert(MPC_ENTRY_MAX_LEN >= sizeof(struct mpc_bus));
  compiler_assert(MPC_ENTRY_MAX_LEN >= sizeof(struct mpc_ioapic));
  compiler_assert(MPC_ENTRY_MAX_LEN >= sizeof(struct mpc_irq));
  compiler_assert(MPC_ENTRY_MAX_LEN >= sizeof(struct mpc_linterrupt));
}

/**
 * @enum mp_entry
 * @brief Variable entries types
 */
enum mp_entry {
  MP_PROCESSOR = 0,
  MP_BUS,
  MP_IOAPIC,
  MP_IOINTERRUPT,
  MP_LINTERRUPT,
};

/**
 * @enum mp_irqtype
 * @brief MP IRQ type.
 */
enum mp_irqtype {
  MP_INT = 0,      /* IOAPIC provided vector */
  MP_NMI,
  MP_SMI,
  MP_ExtINT,      /* 8259A provided vector */
};

/*
 * Parsed MP tables data exported to the rest of
 * of the system
 */

/**
 * @var mp_isa_busid
 * @brief MP ISA bus ID.
 */
extern int mp_isa_busid;

/**
 * @var nr_mpcirqs
 * @brief Number of MPC IRQs.
 */
extern int nr_mpcirqs;

/**
 * @def MAX_IRQS
 * @brief Maximum IRQs.
 */
#define MAX_IRQS  (0xff - 0x1f)

/**
 * @fn mp_irqs[MAX_IRQS]
 * @brief Array of MP IRQs.
 */
extern struct mpc_irq mp_irqs[MAX_IRQS];

/**
 * @fn void mptables_init(void)
 * @brief initialize MP tables.
 */
void mptables_init(void);

/**
 * @fn __pure_const int mptables_get_nr_cpus(void)
 * @brief get number of CPUs in MP tables.
 *
 * @return Number of CPUs in MP tables.
 */
__pure_const int mptables_get_nr_cpus(void);

/**
 * @fn void mpc_dump(__unused struct mpc_table *mpc)
 * @brief Dump tables in case of critical errors.
 *
 * @param mpc MP configuration.
 */
void mpc_dump(__unused struct mpc_table *mpc);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_MPTABLES_H__ */
