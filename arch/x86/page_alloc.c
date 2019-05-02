/**
 * @file arch/x86/page_alloc.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Memory Management: the page allocator
 *
 * Copyright (C) 2009-2010 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * The page allocator allocates and reclaims physical memory pages to the
 * rest of the kernel upon request.
 *
 * The algorithm implemented here is the one used in Unix SVR2 and descr-
 * ibed in Maurice Bach's work. Each page of physical memory is represnted
 * by a page frame descriptor, and collected in the 'page frame descriptor
 * table' pfdtable. Space is allocated for this table - directly after
 * kernel's memory area - once for the lifetime of the system.
 *
 * To allocate and reclaim pages, the allocator links free entries of the
 * pfdtable into a freelist. Allocation and reclamation is merely an act
 * of list pointers manipulations, similar to SVR2 buffer cache lists.
 *
 * Two major differences from the SVR2 algorithm exist. The first is
 * 'reverse mapping' an address to its page descriptor. Assuming no gaps
 * in available physical memory exist (as in the old Unix days), one could
 * 've been able to find the desired page descriptor by
 *
 *    pfdtable index = phys addr >> PAGE_SHIFT
 *
 * where PAGE_SHIFT = log2(PAGE_SIZE). But in the PC architecture, thanks
 * to the PCI hole and similar areas, reserved memory regions can reach a
 * full GB __in between__ available physical memory space, making the abo-
 * ve approach impractical.
 *
 * That approach could only be used now if descriptors for reserved memory
 * areas were also included in the pfdtable - a huge memory loss. Here is
 * an example of a sparse e820 physical memory map from an 8-GByte-RAM PC:
 *
 *   0x0000000000000000 - 0x000000000009fc00 (low-memory)
 *   0x000000000009fc00 - 0x00000000000a0000 (reserved)
 *   0x00000000000e6000 - 0x0000000000100000 (reserved)
 *   0x0000000000100000 - 0x00000000cff90000 (available)
 *   0x00000000cff90000 - 0x00000000cffa8000 (ACPI reclaim)
 *   0x00000000cffa8000 - 0x00000000cffd0000 (ACPI NVS)
 *   0x00000000cffd0000 - 0x00000000d0000000 (reserved)
 *   0x00000000fff00000 - 0x0000000100000000 (reserved)
 *   0x0000000100000000 - 0x0000000230000000 (available)
 *
 * Thus, a new reverse mapping mechanism suitable for PC/ACPI was created.
 *
 * The second difference from SVR2 is the support of 'zones': asking the
 * allocator for a page only from a certain physical region, which is a
 * feature needed by early boot setup. Instead of having a single freeli-
 * st, we now have a unique one for each physical memory zone.
 *
 * Finally, the API (not code) for allocating and reclaiming pages comes
 * from linux-2.6; these function names were there since linux-0.1!
 */

#include <mcube/mcube.h>

/*
 * Page Frame Descriptor Table
 *
 * Page descriptors covering kernel memory areas are not
 * included here. Space for this table is manually alloc-
 * ated directly after kernel's BSS section.
 *
 * @pfdtable_top: current table end mark
 * @pfdtable_end: don't exceed this dynamically set mark
 */
struct page *pfdtable;
struct page *pfdtable_top;
struct page *pfdtable_end;


/*
 * Zone Descriptor Table
 *
 * If a physical page got covered by more than one zone
 * cause of overlapping zone boundaries, such page will get
 * assigned to the highest priority zone covering its area.
 *
 * After MM init, each page will get assigned to _one_, and
 * _only_ one, zone. Each page descriptor will then have an
 * assigned zone ID for its entire lifetime.
 */
struct zone zones[] = {
  [ZONE_1GB] = {
    .id = ZONE_1GB,
    .start = 0x100000,  /* 1-MByte */
    .end = 0x40000000,  /* 1-GByte */
    .description = "Early-boot Zone",
  },

  [ZONE_ANY] = {
    .id = ZONE_ANY,
    .start = 0x0,
    .end = KERN_PHYS_END_MAX,
    .description = "Any Zone",
  },
};



/*
 * @pfdrmap: The global table used to reverse-map an address
 * @pfdrmap_top, @pfdrmap_end: similar to @pfdtable above
 */
struct rmap *pfdrmap;
struct rmap *pfdrmap_top;
struct rmap *pfdrmap_end;


/*
 * Watermak the end of our kernel memory area which is above
 * 1MB and which also contains the pfdtable and the pfdrmap.
 *
 * By this action, we've permanently allocated space for
 * these tables for the entire system's lifetime.
 */
uint64_t kmem_end = -1;

/*
 * Assign a zone to given page
 *
 * FIXME: we need to mark early boot functions like this,
 * and statically assure not being called afterwards.
 */
struct zone *page_assign_zone(struct page *page)
{
  uint64_t start, end;
  struct zone *zone = NULL;

  assert(page->free == 1);
  assert(page->zone_id == ZONE_UNASSIGNED);

  start = page_phys_addr(page);
  end = start + PAGE_SIZE;

  descending_prio_for_each(zone) {
    assert(page_aligned(zone->start));
    assert(page_aligned(zone->end));

    if (start >= zone->start && end <= zone->end) {
      page->zone_id = zone->id;
      return zone;
    }
  }

  panic("Memory - Physical page 0x%lx cannot be attached to any zone", start);
}

/*
 * Add a reverse-mapping entry for given e820 range.
 * @start: first pfdtable cell represenging @range.
 * NOTE! no locks; access this only from init paths.
 */
void rmap_add_range(struct e820_range *range, struct page *start)
{
  struct rmap *rmap;

  rmap = pfdrmap_top;

  assert(rmap + 1 <= pfdrmap_end);
  rmap->range = *range;
  rmap->pfd_start = start;

  pfdrmap_top = rmap + 1;
}

/*
 * Create (and initialize) new pfdtable entries for given
 * memory range, which should be e820 available and above
 * our kernel memory area.
 *
 * NOTE! No need to acquire zones freelist locks as this
 * should _only_ be called from the serial init path.
 *
 * Prerequisite: kernel memory area end pre-calculated
 */
void pfdtable_add_range(struct e820_range *range)
{
  uint64_t start, end, nr_pages;
  struct zone *zone;
  struct page *page;

  assert(range->type == E820_AVAIL);

  start = range->base;
  end = range->base + range->len;
  assert(page_aligned(start));
  assert(page_aligned(end));
  assert(page_aligned(kmem_end));
  assert(start >= PHYS(kmem_end));
  assert(start < end);

  /* New entries shouldn't overflow the table */
  page = pfdtable_top;
  nr_pages = (end - start) / PAGE_SIZE;
  assert((page + nr_pages) <= pfdtable_end);

  rmap_add_range(range, page);

  while (start != end) {
    page_init(page, start);
    zone = page_assign_zone(page);

    page->next = zone->freelist;
    zone->freelist = page;
    zone->freepages_count++;

    page++;
    start += PAGE_SIZE;
  }

  pfdtable_top = page;
}


/*
 * Page allocation and reclamation, in O(1)
 *
 * NOTE! do not call these in IRQ-context!
 */

static struct page *__get_free_page(enum zone_id zid)
{
  struct zone *zone;
  struct page *page;

  zone = get_zone(zid);

  spin_lock(&zone->freelist_lock);

  if (!zone->freelist) {
    page = NULL;
    goto out;
  }

  page = zone->freelist;
  zone->freelist = zone->freelist->next;
  zone->freepages_count--;

  assert(page->free == 1);
  page->free = 0;

out:
  spin_unlock(&zone->freelist_lock);
  return page;
}

struct page *get_free_page(enum zone_id zid)
{
  struct zone *zone = NULL;
  struct page *page;
  uint64_t start, end;

  if (zid == ZONE_ANY) {
    ascending_prio_for_each(zone) {
      page = __get_free_page(zone->id);

      if (page) {
        break;
      }
    }
  } else {
    page = __get_free_page(zid);
  }

  if (!page) {
    panic("Memory - No more free pages available at "
          "`%s'", get_zone(zid)->description);
  }

  start = page_phys_addr(page);
  end = start + PAGE_SIZE;
  zone = get_zone(zid);

  assert(start >= zone->start);
  assert(end <= zone->end);
  return page;
}

struct page *get_zeroed_page(enum zone_id zid)
{
  struct page *page;

  page = get_free_page(zid);
  memset64(page_address(page), 0, PAGE_SIZE);

  return page;
}

void free_page(struct page *page)
{
  struct zone *zone;

  zone = get_zone(page->zone_id);

  spin_lock(&zone->freelist_lock);

  page->next = zone->freelist;
  zone->freelist = page;
  zone->freepages_count++;

  if (page->free != 0) {
    panic("Memory - Freeing already free page at 0x%lx\n",
          page_address(page));
  }

  page->free = 1;

  spin_unlock(&zone->freelist_lock);
}

/*
 * Reverse mapping, in O(n)
 *
 * 'N' is the # of 'available' e820 ranges.
 */

/*
 * Return the page descriptor representing @addr.
 */
struct page *addr_to_page(void *addr)
{
  struct rmap *rmap;
  struct page *page;
  struct e820_range *range;
  uint64_t paddr, start, end, offset;

  paddr = PHYS(addr);
  paddr = ROUND_DOWN(paddr, PAGE_SIZE);

  for (rmap = pfdrmap; rmap != pfdrmap_top; rmap++) {
    range = &(rmap->range);
    start = range->base;
    end = start + range->len;

    if (paddr < start) {
      continue;
    }

    if (paddr >= end) {
      continue;
    }

    page = rmap->pfd_start;
    offset = (paddr - start) / PAGE_SIZE;
    page += offset;
    assert(page_phys_addr(page) < end);
    return page;
  }

  panic("Memory - No page descriptor found for given 0x%lx "
        "address", addr);
}

/*
 * Initialize the page allocator structures
 */
void pagealloc_init(void)
{
  struct e820_range *range = NULL;
  struct e820_setup *setup;
  struct zone *zone = NULL;
  uint64_t avail_pages, avail_ranges;

  zones_init();

  /*
   * While building the page descriptors in the pfdtable,
   * we want to be sure not to include pages that override
   * our own pfdtable area. i.e. we want to know the
   * pfdtable area length _before_ forming its entries.
   *
   * Since the pfdtable length depends on available phys
   * memory, we move over the e820 map in two passes.
   *
   * First pass: estimate pfdtable area length by counting
   * provided e820-availale pages and ranges.
   */

  setup = e820_get_memory_setup();
  avail_pages = setup->avail_pages;
  avail_ranges = setup->avail_ranges;

  printk("Memory: Available physical memory = %d MB\n",
         ((avail_pages * PAGE_SIZE) / 1024) / 1024);

  pfdtable = ramdisk_memory_area_end();
  pfdtable_top = pfdtable;
  pfdtable_end = pfdtable + avail_pages;

  printk("Memory: Page Frame descriptor table size = %d KB\n",
         (avail_pages * sizeof(pfdtable[0])) / 1024);

  pfdrmap = (struct rmap *) pfdtable_end;
  pfdrmap_top = pfdrmap;
  pfdrmap_end = pfdrmap + avail_ranges;

  kmem_end = ROUND_UP((uintptr_t)pfdrmap_end, PAGE_SIZE);
  printk("Memory: Kernel memory area end = 0x%lx\n", kmem_end);

  /*
   * Second Pass: actually fill the pfdtable entries
   *
   * Including add_range(), this loop is O(n), where
   * n = number of available memory pages in the system
   */

  e820_for_each(range) {
    if (range->type != E820_AVAIL) {
      continue;
    }

    if (e820_sanitize_range(range, kmem_end)) {
      continue;
    }

    pfdtable_add_range(range);
  }

  /*
   * Statistics
   */

  ascending_prio_for_each(zone) {
    zone->boot_freepages = zone->freepages_count;
  }
}

