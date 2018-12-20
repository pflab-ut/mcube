/**
 * @file include/x86/acpi.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_ACPI_H__
#define	__MCUBE_X86_ACPI_H__

/**
 * @brief Advanced configuration and power interface (ACPI) tables.
 *
 * Copyright 2016 Brett Vickers.
 * Use of this source code is governed by a BSD-style license
 * that can be found in the MonkOS LICENSE file.
 */

#ifndef __ASSEMBLY__

/**
 * @struct acpi_hdr
 * @brief  Header attached to the front of all ACPI tables.
 */
struct acpi_hdr {
  union {
    char     bytes[4];     ///< Contains 4-letter table identifier
    uint32_t dword;
  }        signature;
  uint32_t length;           ///< Length of this table including header
  uint8_t  revision;         ///< Revision number, should be 1
  uint8_t  checksum;         ///< Covers entire table
  char     oemid[6];         ///< Supplied by the OEM
  char     oemtableid[8];    ///< Supplied by the OEM
  uint32_t oemrevision;      ///< Supplied by the OEM
  char     creatorid[4];     ///< Vendor id
  uint32_t creator_revision; ///< Revision of this utility
} PACKSTRUCT;

/**
 * @struct acpi_fadt
 * @brief  Fixed ACPI Description Table (FADT)
 */
struct acpi_fadt {
  struct acpi_hdr hdr;

  uint32_t firmware_ctl;  ///< Pointer to FACS firmware control block
  uint32_t ptr_dsdt;      ///< Pointer to DSDT block
  uint8_t  reserved1;     ///< Not used
  uint8_t  pm_profile;    ///< Preferred power management profile
  uint16_t sci_interrupt; ///< SCI interrupt vector
  uint32_t smi_cmdport;   ///< SMI command port
  uint8_t  acpi_enable;   ///< SMI command to disable SMI ownership
  uint8_t  acpi_disable;  ///< SMI command to re-enable SMI ownership
  uint8_t  s4bios_req;    ///< SMI command to enter S4BIOS state
  uint8_t  pstate_ctl;    ///< SMI command to assume perf state ctl
  uint32_t pm1a_evtblock; ///< Port of PM1a event register block
  uint32_t pm1b_evtblock; ///< Port of PM1b event register block
  uint32_t pm1a_ctlblock; ///< Port of PM1a ctl register block
  uint32_t pm1b_ctlblock; ///< Port of PM1b ctl register block
  uint32_t pm2_ctlblock;  ///< Port of PM2 ctl register block
  uint32_t pmt_ctlblock;  ///< Port of PM timer ctl register block
  uint32_t gpe0_block;    ///< Port of general-purpose event 0 reg block
  uint32_t gpe1_block;    ///< Port of general-purpose event 0 reg block
  uint8_t  pm1_evt_len;   ///< Bytes decoded by pm1*_evtblock
  uint8_t  pm1_ctl_len;   ///< Bytes decoded by pm1*_ctlblock
  uint8_t  pm2_ctl_len;   ///< Bytes decoded by pm2_ctlblock
  uint8_t  pmt_ctl_len;   ///< Bytes decoded by pmt_ctlblock
  uint8_t  gpe0_len;      ///< Bytes decoded by gpe0_block
  uint8_t  gpe1_len;      ///< Bytes decoded by gpe1_block
  uint8_t  gpe1_base;     ///< Offset where gpe1 events start
  uint8_t  cstate_ctl;    ///< SMI command for C state notifications
  uint16_t latency_c2;    ///< Worst-case us latency to enter C2 state
  uint16_t latency_c3;    ///< Worst-case us latency to enter C3 state
  uint16_t flush_size;    ///< Cache reads to flush dirty cache
  uint16_t flush_stride;  ///< Cache width (flush stride)
  uint8_t  duty_offset;   ///< Index of P_CNT reg duty cycle setting
  uint8_t  duty_width;    ///< Width of P_CNT reg duty cycle setting
  uint8_t  alarm_day;     ///< RTC RAM index day-of-month alarm: day
  uint8_t  alarm_month;   ///< RTC RAM index day-of-month alarm: month
  uint8_t  century;       ///< RTC RAM index of century
  uint16_t boot_arch;     ///< Boot architecture flags (ACPI 2.0+)
  uint8_t  reserved2;     ///< Not used
  uint32_t flags;         ///< Fixed feature flags
} PACKSTRUCT;

/**
 * @struct acpi_mcfg
 * @brief  PCI express Mapped Configuration (MCFG) table.
 */
struct acpi_mcfg {
  struct acpi_hdr hdr;

  uint64_t reserved;
} PACKSTRUCT;

/**
 * @struct acpi_mcfg_addr
 * @brief  MCFG entry, one or more of which appears at the tail of the acpi_mcfg struct.
 */
struct acpi_mcfg_addr {
  uint64_t base;          ///< Base address of configuration mechanism
  uint16_t seg_group;     ///< PCI segment group number
  uint8_t  bus_start;     ///< Start PCI bus number
  uint8_t  bus_end;       ///< End PCI bus number
  uint32_t reserved;
} PACKSTRUCT;

/**
 * @struct acpi_madt
 * @brief  Multiple APIC description table (MADT).
 */
struct acpi_madt {
  struct acpi_hdr hdr;

  uint32_t ptr_local_apic;   ///< Local APIC address
  uint32_t flags;            ///< APIC flags
} PACKSTRUCT;

/**
 * @enum  acpi_madt_type
 * @brief MADT entry types.
 */
enum acpi_madt_type {
    ACPI_MADT_LOCAL_APIC   = 0,  ///< Processor Local APIC
    ACPI_MADT_IO_APIC      = 1,  ///< I/O APIC
    ACPI_MADT_ISO          = 2,  ///< Interrupt Source Override
    ACPI_MADT_NMIS         = 3,  ///< NMI Source
    ACPI_MADT_LOCAL_NMI    = 4,  ///< Local APIC NMI
    ACPI_MADT_LOCAL_ADDR   = 5,  ///< Local APIC Address Override
    ACPI_MADT_IO_SAPIC     = 6,  ///< I/O SAPIC
    ACPI_MADT_LOCAL_SAPIC  = 7,  ///< Local SAPIC
    ACPI_MADT_PLATFORM_IS  = 8,  ///< Platform Interrupt Sources
    ACPI_MADT_LOCAL_X2APIC = 9,  ///< Processor Local x2APIC
    ACPI_MADT_X2APIC_NMI   = 10, ///< Local x2APIC NMI
    ACPI_MADT_GIC          = 11, ///< GIC
    ACPI_MADT_GICD         = 12, ///< GICD
};

/**
 * @struct acpi_madt_hdr
 * @brief  MADT entry header.
 */
struct acpi_madt_hdr {
  uint8_t type;           ///< acpi_madt_type
  uint8_t length;         ///< Length of IC structure including header
} PACKSTRUCT;

/**
 * @struct acpi_madt_local_apic
 * @brief  MADT local APIC entry.
 */
struct acpi_madt_local_apic {
  struct acpi_madt_hdr hdr; // type = 0

  uint8_t  procid;          ///< Processor ID
  uint8_t  apicid;          ///< Local APIC ID
  uint32_t flags;           ///< Local APIC flags (bit 0 = usable)
} PACKSTRUCT;

/**
 * @struct acpi_madt_io_apic
 * @brief  MADT I/O APIC entry.
 */
struct acpi_madt_io_apic {
  struct acpi_madt_hdr hdr; // type = 1

  uint8_t  apicid;          ///< I/O APIC ID
  uint8_t  reserved;
  uint32_t ptr_io_apic;     ///< I/O APIC address
  uint32_t interrupt_base;  ///< Interrupt # where interrupts start
} PACKSTRUCT;

/**
 * @struct acpi_madt_iso
 * @brief  MADT Interrupt Source Override (ISO) entry.
 */
struct acpi_madt_iso {
  struct acpi_madt_hdr hdr; // type = 2

  uint8_t  bus;             ///< Must be 0, meaning ISA
  uint8_t  source;          ///< Bus-relative source (IRQ)
  uint32_t interrupt;       ///< Interrupt this soruce will signal
  uint16_t flags;           ///< MPS INTI flags
} PACKSTRUCT;

/**
 * @fn       init_acpi
 * @brief    Find and parse all available ACPI tables.
 */
void init_acpi(void);

/**
 * @fn       acpi_version
 * @brief    Return the ACPI version number.
 * @returns  The ACPI version number (1 through 5).
 */
int acpi_version(void);

/**
 * @fn       acpi_fadt
 * @brief    Return a pointer to the ACPI fixed ACPI description table.
 * @returns  A pointer to the non-extended FADT structure.
 */
const struct acpi_fadt *acpi_fadt(void);

/**
 * @fn       acpi_madt
 * @brief    Return a pointer to the ACPI multiple APIC description table(MADT).
 * @returns  A pointer to the MADT structure.
 */
const struct acpi_madt *acpi_madt(void);

/**
 * @fn         acpi_next_local_apic
 * @brief      Return a pointer to the next Local APIC structure entry in the MADT table.
 * @param[in]  prev Pointer to the local APIC returned by a previous call to this function.
 *             Pass NULL for the first call.
 * @returns    A pointer to a the next local APIC structure, or NULL if none remain.
 */
const struct acpi_madt_local_apic *acpi_next_local_apic(const struct acpi_madt_local_apic *prev);

/**
 * @fn        acpi_next_io_apic
 * @brief     Return a pointer to the next I/O APIC structure entry in the MADT table.
 * @param[in] prev Pointer to the I/O APIC returned by a previous call to this function.
 *            Pass NULL for the first call.
 * @returns    A pointer to a the next I/O APIC structure, or NULL if none remain.
 */
const struct acpi_madt_io_apic *acpi_next_io_apic(const struct acpi_madt_io_apic *prev);

/**
 * @fn        acpi_next_iso
 * @brief     Return a pointer to the next Interrupt Source Override (ISO) structure entry in the MADT table.
 * @param[in] prev Pointer to the ISO returned by a previous call to this function.
 *            Pass NULL for the first call.
 * @returns   A pointer to a the next ISO structure, or NULL if none remain.
 */
const struct acpi_madt_iso *acpi_next_iso(const struct acpi_madt_iso *prev);

/**
 * @fn        acpi_next_mcfg_addr
 * @brief     Return a pointer to the next Mapped Configuration (MCFG)
 *            address entry, used for PCIe.
 * @param[in] prev Pointer to the MCFG entry returned by a previous call to this function.
 *            Pass NULL for the first call.
 * @returns   A pointer to a the next MCFG entry, or NULL if none remain.
 */
const struct acpi_mcfg_addr *acpi_next_mcfg_addr(const struct acpi_mcfg_addr *prev);



#define SIGNATURE_RSDP      0x2052545020445352ll // "RSD PTR "
#define SIGNATURE_MADT      0x43495041           // "APIC"
#define SIGNATURE_BOOT      0x544f4f42           // "BOOT"
#define SIGNATURE_FADT      0x50434146           // "FACP"
#define SIGNATURE_HPET      0x54455048           // "HPET"
#define SIGNATURE_MCFG      0x4746434d           // "MCFG"
#define SIGNATURE_SRAT      0x54415253           // "SRAT"
#define SIGNATURE_SSDT      0x54445353           // "SSDT"
#define SIGNATURE_WAET      0x54454157           // "WAET"

// Page alignment macros
#define PAGE_ALIGN_DOWN(a)  ((a) & ~(PAGE_SIZE - 1))
#define PAGE_ALIGN_UP(a)    (((a) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

/**
 * @struct btable
 * @brief A structure used to track the state of the temporary page table generated by the boot loader.
 * The ACPI code updates it to access the memory stored in ACPI tables.
 */
struct btable {
  page_t *root;           ///< The top-level (PML4) page table
  page_t *next_page;      ///< The next page to use when allocating
  page_t *term_page;      ///< Just beyond the last available page
};

typedef struct btable btable_t;

/**
 * @struct acpi_rsdp
 * @brief ACPI Root System Description Pointer.
 */
struct acpi_rsdp {
  char signature[8];  ///< Contains "RSD PTR "
  uint8_t checksum;      ///< Covers up to (and including) ptr_rsdt
  char oemid[6];      ///< Supplied by the OEM
  uint8_t revision;      ///< 0=1.0, 1=2.0, 2=3.0
  uint32_t ptr_rsdt;      ///< 32-bit pointer to RSDT table

  // The following fields do not exist in ACPI1.0
  uint32_t length;        ///< RSDT table length, including header
  uint64_t ptr_xsdt;      ///< 64-bit pointer to XSDT table
  uint8_t checksum_ex;   ///< Covers entire rsdp structure
  uint8_t reserved[3];
} PACKSTRUCT;

/**
 * @struct acpi_rsdt
 * @brief ACPI Root System Description Table.
 */
struct acpi_rsdt {
  struct acpi_hdr hdr;
  uint32_t ptr_table[1]; ///< Pointers to other ACPI tables
} PACKSTRUCT;

/**
 * @struct acpi_xsdt
 * @brief ACPI eXtended System Descriptor Table (XSDT).
 */
struct acpi_xsdt {
  struct acpi_hdr hdr;
  uint64_t ptr_table[1]; ///< Pointers to other ACPI tables
} PACKSTRUCT;


/**
 * @struct acpi
 * @brief ACPI.
 */
struct acpi {
  int version; // ACPI version (1, 2 or 3)
  const struct acpi_rsdp *rsdp;
  const struct acpi_rsdt *rsdt;
  const struct acpi_xsdt *xsdt;
  const struct acpi_fadt *fadt;
  const struct acpi_madt *madt;
  const struct acpi_mcfg *mcfg;
};


/**
 * @struct address_structure
 * @brief Address structure information.
 */
struct address_structure {
  uint8_t address_space_id;    // 0 - system memory, 1 - system I/O
  uint8_t register_bit_width;
  uint8_t register_bit_offset;
  uint8_t reserved;
  uint64_t address;
} __attribute__((packed));


/**
 * @struct description_table_header
 * @brief Description Table Header information.
 */
struct description_table_header {
  char signature[4];    // 'HPET' in case of HPET table
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  char oemid[6];
  uint64_t oem_tableid;
  uint32_t oem_revision;
  uint32_t creator_id;
  uint32_t creator_revision;
} __attribute__((packed));

/**
 * @struct hpet
 * @brief HPET information.
 */
struct hpet {
  struct description_table_header header;
  uint8_t hardware_rev_id;
  uint8_t comparator_count:5;
  uint8_t counter_size:1;
  uint8_t reserved:1;
  uint8_t legacy_replacement:1;
  uint16_t pci_vendor_id;
  struct address_structure address;
  uint8_t hpet_number;
  uint16_t minimum_tick;
  uint8_t page_protection;
} __attribute__((packed));



#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_X86_PAGING_H__ */
