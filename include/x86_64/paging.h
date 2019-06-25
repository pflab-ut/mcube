/**
 * @file include/x86_64/paging.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_PAGING_H__
#define __MCUBE_X86_64_PAGING_H__

/*
 * x86_64 paging and kernel address-space control
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#ifndef __ASSEMBLY__


/*
 * Kernel-space mappings
 *
 * Macros returning physical addresses intentionally return
 * an unsigned integer instead of a pointer; we do not want
 * to have invalid pointers dangling around.
 */

/*
 * Mappings for kernel text, data, and bss (-2GB)
 *
 * This is the virtual base for %rip, %rsp, and kernel global
 * symbols. Check the logic behind this at head.S comments.
 *
 * @c KTEXT_OFFSET is equivalent to Linux's __START_KERNEL_MAP
 */

/**
 * @def KTEXT_PAGE_OFFSET
 * @brief Kernel text page offset.
 */
#define KTEXT_PAGE_OFFSET 0xffffffff80000000ULL

/**
 * @def KTEXT_PHYS_OFFSET
 * @brief Kernel text physical address offset.
 */
#define KTEXT_PHYS_OFFSET 0x0

/**
 * @def KTEXT_PAGE_END
 * @brief Kernel text page end.
 */
#define KTEXT_PAGE_END 0xffffffffa0000000ULL

/**
 * @def KTEXT_AREA_SIZE
 * @brief Kernel text area size.
 */
#define KTEXT_AREA_SIZE (KTEXT_PAGE_END - KTEXT_PAGE_OFFSET)

/**
 * @def KTEXT_PHYS_END
 * @brief Kernel text physical address end.
 */
#define KTEXT_PHYS_END (KTEXT_PHYS_OFFSET + KTEXT_AREA_SIZE)

/**
 * @def KTEXT_VIRTUAL(phys_address)
 * @brief Kernel text physical to virtual address.
 *
 * @param phys_address Physical address.
 */
#define KTEXT_VIRTUAL(phys_address)                         \
  ({                                                        \
    assert((uintptr_t)(phys_address) >= KTEXT_PHYS_OFFSET); \
    assert((uintptr_t)(phys_address) < KTEXT_PHYS_END);     \
                                                            \
    (void *)((char *)(phys_address) + KTEXT_PAGE_OFFSET);   \
  })

/**
 * @def KTEXT_PHYS(virt_address)
 * @brief Kernel text virtual to physical address.
 *
 * @param virt_address Virtual address.
 */
#define KTEXT_PHYS(virt_address)                              \
  ({                                                          \
    assert((uintptr_t)(virt_address) >= KTEXT_PAGE_OFFSET);   \
    assert((uintptr_t)(virt_address) < KTEXT_PAGE_END);       \
                                                              \
    (uintptr_t)((char *)(virt_address) - KTEXT_PAGE_OFFSET);  \
  })

/*
 * Kernel-space mappings for all system physical memory
 *
 * This is the standard kernel virtual base for everything
 * except kernel text and data areas (%rip, %rsp, symbols).
 * Check the logic behind this at the head.S comments.
 *
 * @c KERN_PAGE_END_MAX, @c KERN_PHYS_END_MAX: those are only
 * rached if the system has max supported phys memory, 64TB!
 */

/**
 * @def KERN_PAGE_OFFSET
 * @brief Kernel page offset.
 */
#define KERN_PAGE_OFFSET 0xffff800000000000ULL

/**
 * @def KERN_PHYS_OFFSET
 * @brief Kernel physical address offset.
 */
#define KERN_PHYS_OFFSET 0x0ULL

/**
 * @def KERN_PAGE_END_MAX
 * @brief Kernel page end maximum.
 */
#define KERN_PAGE_END_MAX 0xffffc00000000000ULL

/**
 * @def KERN_AREA_MAX_SIZE
 * @brief Kernel area maximum size.
 */
#define KERN_AREA_MAX_SIZE (KERN_PAGE_END_MAX - KERN_PAGE_OFFSET)

/**
 * @def KERN_PHYS_END_MAX
 * @brief Kernel physical end maximum.
 */
#define KERN_PHYS_END_MAX (KERN_PHYS_OFFSET + KERN_AREA_MAX_SIZE)

/**
 * @def VIRTUAL(phys_address)
 * @brief Physical address to virtual address.
 */
#define VIRTUAL(phys_address)                               \
  ({                                                        \
    assert((uintptr_t)(phys_address) >= KERN_PHYS_OFFSET);  \
    assert((uintptr_t)(phys_address) < KERN_PHYS_END_MAX);  \
                                                            \
    (void *)((char *)(phys_address) + KERN_PAGE_OFFSET);    \
  })

/**
 * @def PHYS(virt_address)
 * @brief Virtual address to physical address.
 */
#define PHYS(virt_address)                                  \
  ({                                                        \
    assert((uintptr_t)(virt_address) >= KERN_PAGE_OFFSET);  \
    assert((uintptr_t)(virt_address) < KERN_PAGE_END_MAX);  \
                                                            \
    (uintptr_t)((char *)(virt_address) - KERN_PAGE_OFFSET); \
  })

/*
 * Page Map Level 4
 *
 * A PML4 Table can map the entire virtual 48-bit x86_64
 * address space. Each table entry maps a 512-GB region.
 */

/**
 * @def PML4_ENTRY_SHIFT
 * @brief PML4 entry shift.
 */
#define PML4_ENTRY_SHIFT (21 + 9 + 9)                       /* 39 */

/**
 * @def PML4_ENTRY_MAPPING_SIZE
 * @brief PML4 entry mapping size.
 */
#define PML4_ENTRY_MAPPING_SIZE (0x1ULL << PML4_ENTRY_SHIFT)     /* 512-GByte */

/**
 * @def PML4_MAPPING_SIZE
 * @brief PML4 mapping size.
 */
#define PML4_MAPPING_SIZE (0x1ULL << (PML4_ENTRY_SHIFT + 9)) /* 256-TByte */

/**
 * @def PML4_ENTRIES
 * @brief PML4 entries.
 */
#define PML4_ENTRIES 512                                /* 4KB / 8 */

/**
 * @def pml4_index(virt_addr)
 * Extract the 9-bit PML4 index/offset from given virtual address.
 *
 * @param virt_addr Virtual address.
 */
#define pml4_index(virt_addr)                               \
  (((uintptr_t)(virt_addr) >> PML4_ENTRY_SHIFT) & 0x1ffULL)

/*
 * Page Map Level 3 - the Page Directory Pointer
 *
 * A PML3 Table can map a 512-GByte virtual space by
 * virtue of its entries, which can map 1-GByte each.
 */

/**
 * @def PML3_ENTRY_SHIFT
 * @brief PML3 entry shift.
 */
#define PML3_ENTRY_SHIFT (21 + 9)                           /* 30 */

/**
 * @def PML3_ENTRY_MAPPING_SIZE
 * @brief PML3 entry mapping size.
 */
#define PML3_ENTRY_MAPPING_SIZE (0x1ULL << PML3_ENTRY_SHIFT)       /* 1-GByte */

/**
 * @def PML3_MAPPING_SIZE
 * @brief PML3 mapping size.
 */
#define PML3_MAPPING_SIZE (0x1ULL << (PML3_ENTRY_SHIFT + 9)) /* 512-GByte */

/**
 * @def PML3_ENTRIES
 * @brief PML3 entries.
 */
#define PML3_ENTRIES 512                                /* 4K / 8 */

/**
 * @def pml3_index(virt_addr)
 * @brief extract the 9-bit PML3 index/offset from given virtual address.
 *
 * @param virt_addr Virtual address.
 */
#define pml3_index(virt_addr)                               \
  (((uintptr_t)(virt_addr) >> PML3_ENTRY_SHIFT) & 0x1ffULL)

/*
 * Page Map Level 2 - the Page Directory
 *
 * A PML2 Table can map a 1-GByte virtual space by
 * virtue of its entries, which can map 2-MB each.
 */

/**
 * @def PML2_ENTRY_SHIFT
 * @brief PML2 entry shift.
 */
#define PML2_ENTRY_SHIFT (21)

/**
 * @def PML2_ENTRY_MAPPING_SIZE
 * @brief PML2 entry mapping size.
 */
#define PML2_ENTRY_MAPPING_SIZE (0x1ULL << PML2_ENTRY_SHIFT)       /* 2-MByte */

/**
 * @def PML2_MAPPING_SIZE
 * @brief PML2 mapping size.
 */
#define PML2_MAPPING_SIZE (0x1ULL << (PML2_ENTRY_SHIFT + 9)  /* 1-GByte */

/**
 * @def PML2_ENTRIES
 * @brief PML2 entries.
 */
#define PML2_ENTRIES 512                                /* 4K / 8 */

/**
 * @def pml2_index(virt_addr)
 * @brief extract the 9-bit PML2 index/offset from given virtual address.
 *
 * @param virt_addr Virtual address.
 */
#define pml2_index(virt_addr)                               \
  (((uintptr_t)(virt_addr) >> PML2_ENTRY_SHIFT) & 0x1ffULL)


/**
 * @def page_aligned(addr)
 * @brief Is page aligned?
 *
 * @param addr Address.
 */
#define page_aligned(addr) (IS_ALIGNED((uintptr_t)(addr), PAGE_SIZE))

/*
 * 2-MByte pages
 */

/**
 * @def PAGE_SHIFT_2MB
 * @brief Page shift 2MB.
 */
#define PAGE_SHIFT_2MB 21

/**
 * @def PAGE_SIZE_2MB
 * @brief Page size 2MB.
 */
#define PAGE_SIZE_2MB (1 << PAGE_SHIFT_2MB)

/*
 * x86_64 Page Table entries
 */

/**
 * @struct pml4e
 * @brief Page-map level 4 entry.
 *
 * Format is common for 2-MB and 4-KB pages.
 */
struct pml4e {
  /**
   * Flags.
   */
  uint64_t
  /** Present PML4 entry. */
  present: 1,
           /** 0: write-disable this 512-GB region. */
           read_write: 1,
           /** 0: no access for CPL=3 code. */
           user_supervisor: 1,
           /** Page-level write-through. */
           pwt: 1,
           /** Page-level cache disable. */
           pcd: 1,
           /**
            * Has this entry been used for virtual address translation?
            * Whenever the cpu uses this paging-entry as a part of virtual
            * address translation, it sets this flag. Since the cpu uses
            * the TLB afterwards, this flag is set only the first time the
            * table or physical page is read or written to. The bit is
            * 'sticky': it never get cleared by the CPU afterwards.
            */
           accessed: 1,
           /** Play it safe and don't use this. */
           __ignored: 1,
           /** AMD and Intel conflict: only one bit is reserved
            * (must be zero) for Intel cpus; they're actually 2 bits for AMDs. */
           __reserved0: 2,
           /** Use those as we wish. */
           avail0: 3,
           /** Page Directory Pointer base >> 12. */
           pml3_base: 40,
           /** AMD was really generous .. */
           avail1: 11,
           /** No-Execute for this 512-GB region. */
           nx: 1;
} __packed /** packed. */;

/**
 * @fn static inline void *pml3_base(struct pml4e *pml4e)
 * @brief PML3 base.
 *
 * @param pml4e PML4 entry.
 */
static inline void *pml3_base(struct pml4e *pml4e)
{
  return VIRTUAL((uintptr_t) pml4e->pml3_base << PAGE_SHIFT);
}

/**
 * @struct pml3e
 * @brief Page Directory Pointer entry.
 *
 * Format is common for 2-MB and 4-KB pages.
 */
struct pml3e {
  /**
   * Flags.
   */
  uint64_t
  /** Present PDP entry. */
  present: 1,
           /** 0: write-disable this 1-GB region. */
           read_write: 1,
           /** 0: no access for CPL=3 code. */
           user_supervisor: 1,
           /** Page-level write-through. */
           pwt: 1,
           /** Page-level cache disable. */
           pcd: 1,
           /** Accessed bit (see pml4e comment). */
           accessed: 1,
           /** Ignored; don't use. */
           __ignored: 1,
           /** Must-be-Zero (see pml4e comment). */
           __reserved0: 2,
           /** Available for use. */
           avail0: 3,
           /** Page Directory base >> 12. */
           pml2_base: 40,
           /** Available area. */
           avail1: 11,
           /** No-Execute for this 1-GB region. */
           nx: 1;
} __packed /** packed. */;

/**
 * @fn static inline void *pml2_base(struct pml3e *pml3e)
 * @brief PML2 base.
 *
 * @param pml3e PML3 entry.
 */
static inline void *pml2_base(struct pml3e *pml3e)
{
  return VIRTUAL((uintptr_t)pml3e->pml2_base << PAGE_SHIFT);
}

/**
 * @struct pml2e
 * @brief Page Directory entry, 2-MB pages.
 *
 * NOTE!! set the page size bit to 1.
 */
struct pml2e {
  /**
   * Flags.
   */
  unsigned
  /** Present referenced 2-MB page. */
  present: 1,
           /** 0: write-disable this 2-MB page. */
           read_write: 1,
           /** 0: no access for CPL=3 code. */
           user_supervisor: 1,
           /** Page-level write-through. */
           pwt: 1,
           /** Page-level cache disable. */
           pcd: 1,
           /** Accessed bit (see pml4e comment). */
           accessed: 1,
           /** The dirty flag is only available in the lowest-level table.
            * If set, it indicates the physical page this entry points
            * has been written. This bit is never cleared by the CPU. */
           dirty: 1,
           /** Page Size bit; must be set to one. When this bit is set,
            * in a third (1-GB page) or second level (2-MB page) page
            * table, the entry is the lowest level of the page translation
            * hierarchy. */
           __reserved1: 1,
           /** Global page bit; only available in the lowest-level page
            * translation hieararchy. If set, referenced page TLB entry
            * is not invalidated when CR3 is loaded. */
           global: 1,
           /** Use those as we wish. */
           avail0: 3,
           /** Page-Attribute Table bit. */
           pat: 1,
           /** Must be zero. */
           __reserved0: 8,
           /** Page base >> 21. */
           page_base: 31,
           /** Available for use. */
           avail1: 11,
           /** No-Execute for this 2-MB page. */
           nx: 1;
} __packed /** packed. */;

/**
 * @fn static inline void *page_base(struct pml2e *pml2e)
 * @brief Page base.
 *
 * @param pml2e PML2 entry.
 */
static inline void *page_base(struct pml2e *pml2e)
{
  return VIRTUAL((uintptr_t) pml2e->page_base << PAGE_SHIFT_2MB);
}

/*
 * %CR3
 *
 * "Flags affected by `Move to/from Control Registers':
 * The OF, SF, ZF, AF, PF, and CF flags are undefined."
 *  --Intel, vol. 2A
 */

/**
 * @fn static inline void load_cr3(uint64_t cr3)
 * @brief load CR3.
 *
 * @param cr3 CR3.
 */
static inline void load_cr3(uint64_t cr3)
{
  asm volatile("mov %0, %%cr3"
               :
               :"r"(cr3)
               :"cc", "memory");
}

/**
 * @fn static inline uint64_t get_cr3(void)
 * @brief get CR3.
 *
 * @return CR3.
 */
static inline uint64_t get_cr3(void)
{
  uint64_t cr3;

  asm volatile("mov %%cr3, %0"
               :"=r"(cr3)
               :
               :"cc", "memory");

  return cr3;
}


#else /* __ASSEMBLY__ */

/**
 * @def KTEXT_PAGE_OFFSET
 * @brief Kernel text page offset.
 */
#define KTEXT_PAGE_OFFSET 0xffffffff80000000

/**
 * @def KTEXT_VIRTUAL(address)
 * @brief Kernel text virtual address.
 *
 * @param address Address.
 */
#define KTEXT_VIRTUAL(address) ((address) + KTEXT_PAGE_OFFSET)

/**
 * @def KTEXT_PHYS(address)
 * @brief Kernel text physical address.
 *
 * @param address Address.
 */
#define KTEXT_PHYS(address) ((address) - KTEXT_PAGE_OFFSET)

/**
 * @def KERN_PAGE_OFFSET
 * @brief Kernel page offset.
 */
#define KERN_PAGE_OFFSET 0xffff800000000000

/**
 * @def VIRTUAL(address)
 * @brief Virtual address.
 *
 * @param address Address.
 */
#define VIRTUAL(address) ((address) + KERN_PAGE_OFFSET)

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_PAGING_H__ */
