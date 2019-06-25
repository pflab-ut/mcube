/**
 * @file include/x86_64/mm.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_MM_H__
#define __MCUBE_X86_64_MM_H__

/*
 * Memory Management (MM) bare bones
 *
 * Copyright (C) 2009-2010 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * MM terminology:
 *
 * o Page Frame: a physical 4-KBytes page, naturally 0x1000-aligned.
 *
 * o Available Page: a page frame marked as available (not reserved) for
 *   system use by the ACPI e820h service.
 *
 * o Page Frame Descriptor: one for each available page frame in the sy-
 *   stem, collecting important details for that page, like whether it's
 *   allocated (by the page allocator), or free.
 *
 * o Page Frame Descriptor Table (pfdtable): A table (array) collecting
 *   all the system's page frame descriptors, and thus statically repre-
 *   senting all system's available RAM.
 *
 * o Page Allocator: an MM module which allocates and reclaims (manages)
 *   memory pages for the rest of the system. Naturally, it manages
 *   these 4K-pages through their descriptors state.
 *
 * o Zone: A special area of physical memory; e.g., we can request a page
 *   from the page allocator, but only from the first physical GB 'zone'.
 *
 * o Bucket Allocator: our 'malloc' service, managing dynamic variable-
 *   sized memory regions (the heap) upon request. First, it allocates
 *   memory - in pages - from the page allocator, and then it slices and
 *   dices them as it sees fit.
 *
 * o Slab (object-caching) Allocator: we don't have one; the bucket all-
 *   ocator is more than enough for our purposes so far.
 */

#ifndef __ASSEMBLY__

/**
 * @enum zone_id
 * @brief Zone ID.
 *
 * Page allocator Zones
 *
 * We divide the physical memory space to 'zones' according to
 * need. The main rational for adding zones support to the page
 * allocator so far is ZONE_1GB.
 *
 * Please order the zones IDs, beginning from 0 upwards, with
 * no gaps, according to their prioririty: the smaller the ID,
 * the higher the zone's priority. Each ID except NULL is used
 * as an index in the zones descriptors table.
 *
 * If the number of zones (including NULL) exceeded four, don't
 * forget to extend the 'struct page' zone_id field from 2 to 3
 * bits: it's essential to save space in that struct.
 */
enum zone_id {
  /* Allocate pages from the first GByte
   *
   * At early boot, where permanent kernel page tables
   * are not yet ready, PAGE_OFFSET-based addresses are
   * only set-up for the first physical GB by head.S
   *
   * At that state, the subset of virtual adddresses
   * representing phys regions > 1-GB are unmapped. If
   * the system has > 1-GB RAM, the page allocator will
   * then return some unmapped virt addresses, leading
   * to #PFs at early boot - Zones_1GB raison d'etre.
   *
   * IMP: Before and while building kernel's permanent
   * page tables at vm_init(), _only_ use this zone! */
  ZONE_1GB = 0,

  /* Allocate pages from any zone
   *
   * Beside acting as a flag, this zone also represents
   * all pages in the system with no special semantics
   * to us. Thus, always make it the least-priority one.
   *
   * If we requested a page from this zone, the page
   * allocator will get free pages from the least prio-
   * rity zone first, reserving precious memory areas
   * as far as possible */
  ZONE_ANY = 1,

  /* Undefined; the `NULL' zone! */
  ZONE_UNASSIGNED = 2,

  /* Number of zones */
  NR_ZONES = 3,
};

/**
 * @struct page
 * @brief Page Frame Descriptor.
 *
 * It's essential to save space in this struct since there's
 * one for each available page frame in the system.
 *
 * Additionally, a smaller pfd means a _much_ smaller pfdtable,
 * lessening this table's probability of hitting a reserved memory area
 * like the legacy ISA and PCI holes.
 */
struct page {
  /**
   * Page information.
   */
  uint64_t
/** Phys addr = pfn << PAGE_SHIFT */ pfn:
  (64 - PAGE_SHIFT),
  /** Not allocated? */
  free: 1,
  /** Used by the bucket-allocator? */
  in_bucket: 1,
  /** The zone we're assigned to. */
  zone_id: 2;

  union {
    /**
     * If in pfdfree_head, next free page.
     */
    struct page *next;

    /**
     * If allocated for the bucket allocator,
     * bucket index in the kmembuckets table.
     */
    uint8_t bucket_idx;
  };
};


/**
 * @var pfdtable
 * @brief Page frame descriptor table.
 */
extern struct page *pfdtable;

/**
 * @var pfdtable_top
 * @brief Page frame descriptor table top.
 */
extern struct page *pfdtable_top;

/**
 * @var pfdtable_end
 * @brief Page frame descriptor table end.
 */
extern struct page *pfdtable_end;


/**
 * @fn static inline void page_init(struct page *page, uintptr_t phys_addr)
 * @brief initialize page.
 *
 * @param page Page.
 * @param phys_addr Physical address.
 */
static inline void page_init(struct page *page, uintptr_t phys_addr)
{
  page->pfn = phys_addr >> PAGE_SHIFT;
  page->free = 1;
  page->in_bucket = 0;
  page->zone_id = ZONE_UNASSIGNED;
  page->next = NULL;
}

/**
 * @fn static inline void *page_address(struct page *page)
 * @brief  In the function name, we didn't add a 'virt' prefix to
 * the 'address' part cause dealing with virtual addresses
 * is the default action throughout the kernel's C part.
 *
 * @param page Page.
 * @return Virtual address of given page.
 */
static inline void *page_address(struct page *page)
{
  return VIRTUAL((uintptr_t)page->pfn << PAGE_SHIFT);
}

/**
 * @fn static inline uintptr_t page_phys_addr(struct page *page)
 * @brief Physical addresses are to be ONLY used at early boot
 * setup and while filling paging tables entries.
 * The return type is intentionally set to int instead of a
 * pointer; we don't want to have invalid pointers dangling
 * around.
 *
 * @param page Page.
 * @return Physical address of given page
 */
static inline uintptr_t page_phys_addr(struct page *page)
{
  return (uintptr_t) page->pfn << PAGE_SHIFT;
}

/**
 * @fn static inline bool page_is_free(struct page *page)
 * @brief Is page free?
 *
 * @param page Page
 * @return True if page is free.
 */
static inline bool page_is_free(struct page *page)
{
  return page->free;
}


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_MM_H__ */
