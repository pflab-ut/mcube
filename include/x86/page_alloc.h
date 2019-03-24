/**
 * @file include/x86/page_alloc.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_PAGE_ALLOC_H__
#define __MCUBE_X86_PAGE_ALLOC_H__


#ifndef __ASSEMBLY__

/*
 * Page allocator Zone Descriptor
 */
struct zone {
  /* Statically initialized */
  enum zone_id id;    /* Self reference (for iterators) */
  uint64_t start;      /* Physical range start address */
  uint64_t end;      /* Physical range end address */
  const char *description;  /* For kernel log messages */

  /* Dynamically initialized */
  struct page *freelist;    /* Connect zone's unallocated pages */
  spinlock_t freelist_lock;  /* Above list protection */
  uint64_t freepages_count;  /* Stats: # of free pages now */
  uint64_t boot_freepages;  /* Stats: # of free pages at boot */
};

extern struct zone zones[NR_ZONES];
extern uint64_t kmem_end;

/*
 * Do not reference the zones[] table directly
 *
 * Use below iterators in desired priority order, or check
 * get_zone(), which does the necessery ID sanity checks.
 */

#define descending_prio_for_each(zone)          \
  for (zone = &zones[0]; zone <= &zones[ZONE_ANY]; zone++)

#define ascending_prio_for_each(zone)          \
  for (zone = &zones[ZONE_ANY]; zone >= &zones[0]; zone--)

static inline struct zone *get_zone(enum zone_id zid)
{
  if (zid != ZONE_1GB && zid != ZONE_ANY) {
    panic("Memory - invalid zone id = %d", zid);
  }

  return &zones[zid];
}

static inline void zones_init(void)
{
  struct zone *zone;

  descending_prio_for_each(zone) {
    zone->freelist = NULL;
    spin_init(&zone->freelist_lock);
    zone->freepages_count = 0;
    zone->boot_freepages = 0;
  }
}

/*
 * Reverse Mapping Descriptor
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
  struct e820_range range;
  struct page *pfd_start;
};

extern struct rmap *pfdrmap;
extern struct rmap *pfdrmap_top;
extern struct rmap *pfdrmap_end;

struct zone *page_assign_zone(struct page *page);
void rmap_add_range(struct e820_range *range, struct page *start);
void pfdtable_add_range(struct e820_range *range);
struct page *get_free_page(enum zone_id zid);
struct page *get_zeroed_page(enum zone_id zid);
void free_page(struct page *page);
struct page *addr_to_page(void *addr);
void pagealloc_init(void);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_PAGE_ALLOC_H__ */
