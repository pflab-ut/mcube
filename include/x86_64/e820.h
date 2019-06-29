/**
 * @file include/x86_64/e820.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_E820_H__
#define __MCUBE_X86_64_E820_H__

/*
 * BIOS 0xE820 - Query System Address Map service.
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * In real-mode (e820.S), we query the bios for system memory map and
 * store the result in below form, to be read by higher level kernel code:
 *
 *             Success                       Failure
 *
 *     -----------------------       -----------------------
 *    |       Checksum        |     |       Checksum        |
 *     -----------------------       -----------------------
 *    |     Err Code (0)      |     |    Err Code ( > 0)    |  Error code
 *     -----------------------       -----------------------
 *    |       E820_END        |     |       E820_END        |   Entries-end flag
 *     -----------------------       -----------------------
 *    |        ......         |     |        ......         |
 *    |      E820 Entry       |     |      E820 Entry       |  Entry N
 *     -----------------------       -----------------------
 *    |    E820 Entry Size    |     |    E820 Entry Size    |  Entry N size
 *     -----------------------       -----------------------
 *              ....                          ....    Var # of entries
 *     -----------------------       -----------------------
 *    |        ......         |     |        ......         |
 *    |      E820 Entry       |     |      E820 Entry       |   Entry 1
 *     -----------------------       -----------------------
 *    |    E820 Entry Size    |     |    E820 Entry Size    |  Entry 1 size
 *     -----------------------       -----------------------
 *    | 'C' | 'U' | 'T' | 'E' |     | 'C' | 'U' | 'T' | 'E' |  Start signature
 *     -----------------------       -----------------------
 *                            ^                             ^
 *        E820_BASE ----------|         E820_BASE ----------|
 *
 * Each entry is returned by a single INT15 BIOS query, resulting in N+1
 * queries, where the last query is marked by the BIOS returning %ebx=0,
 * and no e820 entry written; we directly put the end mark afterwards.
 *
 * If any of the N+1 queries resulted an error, we stop quering the BIOS
 * any further, directly put the end mark, and set the suitable err code.
 *
 * Check the ACPI spec v4.0, Chapter 14, "System Address Map Interfaces",
 * and below structure definitions for the e820h entries format.
 *
 * From now on, we'll refer to above composite struct as 'E820H struct'.
 */

/*
 * Shared defs between C and real-mode code.
 */

/**
 * @def E820_PHYS_BASE
 * @brief E820H struct base address.
 * Should in the first 64K [0, 0xffff] range to ease real-mode access.
 */
#define E820_PHYS_BASE 0x1000

/**
 * @def E820_PHYS_MAX
 * @brief The struct shouldn't exceed a 4K page (arbitary).
 */
#define E820_PHYS_MAX (E820_PHYS_BASE + 0x1000)

/**
 * @def E820_INIT_SIG
 * @brief Struct start signature.
 */
#define E820_INIT_SIG ('C' << (3 * 8) | 'U' << (2 * 8) | 'T' << (1 * 8) | 'E')

/**
 * @def E820_VALID_SIG
 * @brief E820H-struct-is-validated signature.
 */
#define E820_VALID_SIG ('V' << (3 * 8) | 'A' << (2 * 8) | 'L' << (1 * 8) | 'D')

/**
 * @def E820_BIOS_SIG
 * @brief ACPI-defined 'E820h supported' BIOS signature.
 */
#define E820_BIOS_SIG ('S' << (3 * 8) | 'M' << (2 * 8) | 'A' << (1 * 8) | 'P')

/**
 * @def E820_END
 * @brief E820 list end mark.
 */
#define E820_END 0xffffffff

/* Error codes by e820.S. */

/**
 * @def E820_SUCCESS
 * @brief No errors; e820 entries validated.
 */
#define E820_SUCCESS 0x0

/**
 * @def E820_NOT_SUPP
 * @brief BIOS doesn't support E820H srvice.
 */
#define E820_NOT_SUPP 0x1

/**
 * @def E820_BUF_FULL
 * @brief Returned data passed size limit.
 */
#define E820_BUF_FULL 0x2

/**
 * @def E820_ERROR
 * @brief General e820 error (carry set).
 */
#define E820_ERROR 0x3

/**
 * @def E820_BIOS_BUG
 * @brief Buggy BIOS - violating ACPI.
 */
#define E820_BIOS_BUG 0x4

/**
 * @def E820_HUGE_ENTRY
 * @brief Entry size > limit.
 */
#define E820_HUGE_ENTRY 0x5

#ifndef __ASSEMBLY__


/*
 * C code shouldn't be concerned with phys addresses.
 */

/**
 * @def E820_BASE
 * @brief E820 base address.
 */
#define E820_BASE VIRTUAL(E820_PHYS_BASE)

/**
 * @def E820_MAX
 * @brief E820 maximum address.
 */
#define E820_MAX VIRTUAL(E820_PHYS_MAX)

/**
 * @struct e820_range
 * @brief ACPI address range descriptor.
 */
struct e820_range {
  /**
   * Range base address.
   */
  uint64_t base;

  /**
   * Range length.
   */
  uint64_t len;

  /**
   * ACPI-defined range type.
   */
  uint32_t type;
} __packed /** packed. */;

/*
 * ACPI memory range types
 */
enum {
  E820_AVAIL  = 0x1,    /* Available for use by the OS */
  E820_RESERVED  = 0x2,    /* Do not use */
  E820_ACPI_TBL  = 0x3,    /* ACPI Reclaim Memory */
  E820_ACPI_NVS  = 0x4,    /* ACPI NVS Memory */
  E820_ERRORMEM  = 0x5,    /* BIOS detected errors at this range */
  E820_DISABLED  = 0x6,    /* Not BIOS chipset-enabled memory */
};

/**
 * @def e820_for_each
 * @brief iterate over E820h-struct ranges.
 *
 * Take care while modifying this code, we're really
 * bending C and GNU extensions rules to achieve it.
 *
 * Prerequisite: E820h-struct previously validated.
 *
 * @param range       the iterator, 'struct e820_range *'
 */
#define e820_for_each(range)                            \
  for (uint32_t *entry = (uint32_t *) E820_BASE + 1,    \
         entry_len = *entry++,                          \
         __unused *_____b = (uint32_t *)                \
         ({range  = (struct e820_range *) entry;});     \
                                                        \
       *(entry - 1) != E820_END;                          \
                                                          \
       entry = (uint32_t *)((char *) entry + entry_len),  \
         entry_len = *entry++,                            \
         range = (struct e820_range *)entry)


/**
 * @struct e820_setup
 * @brief e820 module public interfaces.
 */
struct e820_setup {
  /**
   * True if struct is initialized.
   */
  int valid;

  /**
   * # of e820-available pages.
   */
  uint64_t avail_pages;

  /**
   * # of bios e820 ranges.
   */
  uint64_t avail_ranges;

  /**
   * max addressable/avail phys addr + 1.
   */
  uint64_t phys_addr_end;
};

/**
 * @fn struct e820_setup *e820_get_memory_setup(void)
 * @brief The page allocator need to know its own length
 * (which depends on available physical memory) before filling its entries.
 * Thus, we provide it this method.
 *
 * @return Memory setup.
 */
struct e820_setup *e820_get_memory_setup(void);

/**
 * @fn uint64_t e820_get_phys_addr_end(void)
 * @brief get end of e820 physical address.
 *
 * @return End of e820 physical address.
 */
uint64_t e820_get_phys_addr_end(void);

/**
 * @fn int e820_sanitize_range(struct e820_range *range, uint64_t kmem_end)
 * @brief modify given e820-available range to meet our standards:
 * - we work with memory in units of pages: page-align
 *   given range if possible, or bailout.
 * - treat ranges inside our kernel mem area as reserved.
 *
 * @param range E820-available range.
 * @param kmem_end: kernel memory area end
 * @return Zero if success, and nonzero if failure.
 */
int e820_sanitize_range(struct e820_range *range, uint64_t kmem_end);

/**
 * @fn void e820_init(void)
 * @brief initialize e820.
 */
void e820_init(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_E820_H__ */
