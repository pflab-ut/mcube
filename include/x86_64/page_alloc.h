/**
 * @file include/x86_64/page_alloc.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_PAGE_ALLOC_H__
#define __MCUBE_X86_64_PAGE_ALLOC_H__


#ifndef __ASSEMBLY__

/**
 * @struct zone
 * @brief Page allocator Zone Descriptor.
 */
struct zone {
  /*
   * Statically initialized.
   */

  /**
   * Self reference (for iterators).
   */
  enum zone_id id;

  /**
   * Physical range start address.
   */
  uint64_t start;

  /**
   * Physical range end address.
   */
  uint64_t end;

  /**
   * For kernel log messages.
   */
  const char *description;

  /*
   * Dynamically initialized.
   */

  /**
   * Connect zone's unallocated pages.
   */
  struct page *freelist;

  /**
   * Above list protection.
   */
  spinlock_t freelist_lock;

  /**
   * Stats: # of free pages now.
   */
  uint64_t freepages_count;

  /**
   * Stats: # of free pages at boot.
   */
  uint64_t boot_freepages;
};

/**
 * @var zones[NR_ZONES]
 * @brief Array of zones.
 */
extern struct zone zones[NR_ZONES];

/**
 * @var kmem_end
 * @brief Kernel memory end.
 */
extern uint64_t kmem_end;

/*
 * Do not reference the zones[] table directly
 *
 * Use below iterators in desired priority order, or check
 * get_zone(), which does the necessery ID sanity checks.
 */

/**
 * @def descending_prio_for_each(zone)
 * @brief descend priority for each.
 *
 * @param zone Zone.
 */
#define descending_prio_for_each(zone)          \
  for (zone = &zones[0]; zone <= &zones[ZONE_ANY]; zone++)

/**
 * @def ascending_prio_for_each(zone)
 * @brief ascend priority for each.
 *
 * @param zone Zone.
 */
#define ascending_prio_for_each(zone)          \
  for (zone = &zones[ZONE_ANY]; zone >= &zones[0]; zone--)

/**
 * @fn static inline struct zone *get_zone(enum zone_id zid)
 * @brief get zone.
 *
 * @param zid Zone ID.
 * @return Zone.
 */
static inline struct zone *get_zone(enum zone_id zid)
{
  if (zid != ZONE_1GB && zid != ZONE_ANY) {
    panic("Memory - invalid zone id = %d", zid);
  }

  return &zones[zid];
}

/**
 * @fn static inline void zones_init(void)
 * @brief initialize zones.
 */
static inline void zones_init(void)
{
  struct zone *zone;

  descending_prio_for_each(zone) {
    zone->freelist = NULL;
    init_spin(&zone->freelist_lock);
    zone->freepages_count = 0;
    zone->boot_freepages = 0;
  }
}

/**
 * @struct rmap
 * @brief Reverse mapping descriptor.
 *
 * This structure aids in reverse-mapping a virtual address
 * to its respective page descriptor.
 *
 * We store page descriptors representing a specific e820-
 * available range sequentially, thus a reference to a range
 * and the pfdtable cell representing its first page is
 * enough for reverse-mapping any address in such a range.
 */
struct rmap {
  /**
   * Range.
   */
  struct e820_range range;

  /**
   * Page frame descriptor table.
   */
  struct page *pfd_start;
};

/**
 * @var pfdrmap
 * @brief Page frame descriptor reverse map.
 */
extern struct rmap *pfdrmap;

/**
 * @var pfdrmap_top
 * @brief Page frame descriptor map top.
 */
extern struct rmap *pfdrmap_top;

/**
 * @var pfdrmap_end
 * @brief Page frame descriptor map end.
 */
extern struct rmap *pfdrmap_end;

/**
 * @fn struct zone *page_assign_zone(struct page *page)
 * @brief assign a zone to given page.
 *
 * @param page Page.
 * @return Zone.
 */
struct zone *page_assign_zone(struct page *page);


/**
 * @fn void rmap_add_range(struct e820_range *range, struct page *start)
 * @brief add range of reverse mapping descriptor.
 *
 * @param range Range.
 * @param start Start page.
 */
void rmap_add_range(struct e820_range *range, struct page *start);

/**
 * @fn void pfdtable_add_range(struct e820_range *range)
 * @brief add range of page frame descriptor table.
 *
 * @param range Range.
 */
void pfdtable_add_range(struct e820_range *range);

/**
 * @fn struct page *get_free_page(enum zone_id zid)
 * @brief get free page.
 *
 * @param zid Zone ID.
 * @return Page.
 */
struct page *get_free_page(enum zone_id zid);

/**
 * @fn struct page *get_zeroed_page(enum zone_id zid)
 * @brief get zeroed page.
 *
 * @param zid Zone ID.
 * @return Page.
 */
struct page *get_zeroed_page(enum zone_id zid);

/**
 * @fn void free_page(struct page *page)
 * @brief free page.
 *
 * @param page Page.
 */
void free_page(struct page *page);

/**
 * @fn struct page *addr_to_page(void *addr)
 * @brief Address to page.
 *
 * @param addr Address.
 * @return Page.
 */
struct page *addr_to_page(void *addr);

/**
 * @fn void pagealloc_init(void)
 * @brief initialize page allocation.
 */
void pagealloc_init(void);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_PAGE_ALLOC_H__ */
