/**
 * @file user/test/test_page_alloc.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Copyright (C) 2009-2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */
#include <mcube/mcube.h>

#if CONFIG_ARCH_X86
/*
 * Keep track of pages we allocate and free
 */
#define PAGES_COUNT  100000
static struct page *pages[PAGES_COUNT];

static char tmpbuf[PAGE_SIZE];

/*
 * Sanity checks for the zones table
 */
static inline void _validate_zones_data(void)
{
  int zid;

  for (zid = 0; zid < ARRAY_SIZE(zones); zid++) {
    assert((int) zones[zid].id == zid);
    assert(zones[zid].description != NULL);
    assert(zones[zid].start < zones[zid].end);
    assert(zones[zid].boot_freepages >= zones[zid].freepages_count);
  }

  /* ZONE_ANY must be the least priority */
  assert(ZONE_ANY == ARRAY_SIZE(zones) - 1);

  printk("_Memory: %s: Success\n", __FUNCTION__);
}

enum _count_type {
  _BOOT,      /* # free pages available at boot */
  _CURRENT,    /* # free pages available now */
};

static uint64_t _get_all_freepages_count(enum _count_type type)
{
  uint64_t count;
  struct zone *zone = NULL;

  count = 0;
  ascending_prio_for_each(zone) {
    if (type == _BOOT) {
      count += zone->boot_freepages;
    } else {
      count += zone->freepages_count;
    }
  }

  return count;
}

/*
 * Assure all e820-available memory spaces are covered in
 * pfdtable cells and are marked as free.
 */
static int _test_boot_freepages_count(void)
{
  uint64_t count, reported_count, pfdtable_count;
  struct e820_range *range = NULL;

  count = 0;
  e820_for_each(range) {
    if (range->type != E820_AVAIL) {
      continue;
    }
    if (e820_sanitize_range(range, kmem_end)) {
      continue;
    }

    count += range->len / PAGE_SIZE;
  }

  reported_count = _get_all_freepages_count(_BOOT);
  if (count != reported_count) {
    panic("_Memory: Available e820 pages = %d, pfdtable boot "
          "avial-pages counter = %d\n", count, reported_count);
  }
  pfdtable_count = pfdtable_top - pfdtable;
  if (count != pfdtable_count) {
    panic("_Memory: Available e820 pages = %d, pfdtable # of "
          "elements = %d\n", count, pfdtable_count);
  }
  printk("_Memory: %s: Success\n", __FUNCTION__);
  return true;
}

/*
 * Most BIOSes represent the above-1MB available memory pages
 * as one contiguous e820 range, leading to a not very good
 * test coverage of the important add_avail_range() method.
 *
 * Here, we extremely stress that method by emulating the ad-
 * dition of a huge number of available e820 ranges. We do so
 * by transforming _each_ available page to an e820 range in
 * itself; e.g. transforming a 1MB e820 range to 0x100 (256)
 * ranges, each range representing a single 0x1000 (4K) page.
 *
 * WARNING: Because addr_to_page() is O(n), where n is the #
 * of e820 ranges in the system, this 'test' - which messes
 * heavily with the guts of the allocator - makes the perfo-
 * rmance of that important function (addr_..) extremely bad.
 *
 * Prerequisite-1: rmode E820h-struct previously validated
 *
 * Prerequisite-2: Do NOT call this after the allocator got
 * used: it _erases_ and re-creates all of its sturcutres!!
 */
__unused static int _torture_pfdtable_add_range(void)
{
  uint64_t old_end, old_count, count, repeat, ranges;
  struct e820_range *range, subrange;
  struct rmap *rmap;
  struct zone *zone = NULL;

  ascending_prio_for_each(zone) {
    assert(zone->freepages_count == zone->boot_freepages);
  }

  /* Each range is to be transformed to x ranges, where x =
   * the number of pages in that range. Modify the pfdrmap
   * table end mark accordingly */
  ranges = 0;
  for (rmap = pfdrmap; rmap != pfdrmap_top; rmap++) {
    range = &(rmap->range);
    ranges += range->len / PAGE_SIZE;
  }
  pfdrmap_top = pfdrmap;
  pfdrmap_end = pfdrmap + ranges;

  /* The pfdrmap became much larger: extend our memory area */
  old_count = _get_all_freepages_count(_BOOT);
  old_end = kmem_end;
  kmem_end = round_up((uintptr_t)pfdrmap_end, PAGE_SIZE);

  /* Clear pfdtable structures */
  pfdtable_top = pfdtable;
  ascending_prio_for_each(zone) {
    zone->freelist = NULL;
    zone->freepages_count = 0;
    zone->boot_freepages = 0;
  }

  /* Refill the table, page by page! */
  e820_for_each(range) {
    if (range->type != E820_AVAIL) {
      continue;
    }
    if (e820_sanitize_range(range, kmem_end)) {
      continue;
    }
    subrange = *range;
    repeat = subrange.len / PAGE_SIZE;
    subrange.len = PAGE_SIZE;
    while (repeat--) {
      pfdtable_add_range(&subrange);
      subrange.base += PAGE_SIZE;
    }
  }

  descending_prio_for_each(zone) {
    zone->boot_freepages = zone->freepages_count;
  }

  /* More kernel memory area led to less available pages.
   * Compare old and new count with this fact in mind */
  old_count -= ((kmem_end - old_end) / PAGE_SIZE);
  count = _get_all_freepages_count(_BOOT);
  if (old_count != count) {
    panic("_Memory: Refilling pfdtable found %d free pages; %d "
          "pages originally reported", count, old_count);
  }
  printk("_Memory: %s: Success!\n", __FUNCTION__);
  return true;
}

/*
 * Check if given page frame is allocation-valid.
 * Prerequisite: rmode E820h-struct previously validated
 */
static int _page_is_free(struct page *page)
{
  uintptr_t start, end, paddr;
  struct e820_range *range = NULL;

  paddr = page_phys_addr(page);
  if (paddr < PHYS(kmem_end)) {
    return false;
  }

  e820_for_each(range) {
    if (range->type != E820_AVAIL) {
      continue;
    }
    start = range->base;
    end = start + range->len;
    if (paddr >= start && (paddr+PAGE_SIZE) <= end) {
      return true;
    }
  }

  return false;
}

/*
 * As a way of page allocator disruption, allocate
 * then free a memory page. Is this really effective?
 */
static inline void _disrupt(void)
{
  struct page *p1, *p2;
  void *addr;

  p1 = get_zeroed_page(ZONE_1GB);

  /* Test reverse mapping */
  addr = page_address(p1);
  p2 = addr_to_page(addr);
  if (p1 != p2) {
    panic("_Memory: FAIL: Reverse mapping addr=0x%lx lead to "
          "page descriptor 0x%lx; it's actually for page %lx\n",
          addr, p2, p1);
  }
  memset64(addr, UINT64_MAX, PAGE_SIZE);
  free_page(p1);
}

/*
 * Check page allocator results coherency by allocating given
 * number of pages and checking them against corruption at
 * different points.
 *
 * If max available number of free pages is given, then
 * iterative calls will also test against any page leaks.
 */
static int _test_pagealloc_coherency(int nr_pages)
{
  int i, res;
  uint64_t old_count, count;
  void *addr;
  struct page *page;

  old_count = _get_all_freepages_count(_CURRENT);

  for (i = 0; i < 100; i++) {
    _disrupt();
  }
  
  for (i = 0; i < nr_pages; i++) {
    _disrupt();
    pages[i] = get_free_page(ZONE_ANY);
    if (!_page_is_free(pages[i])) {
      panic("_Memory: Allocated invalid page address "
            "0x%lx\n", page_phys_addr(pages[i]));
    }
    /* Test reverse mapping */
    addr = page_address(pages[i]);
    page = addr_to_page(addr);
    if (page != pages[i]) {
      panic("_Memory: FAIL: Reverse mapping addr=0x%lx lead "
            "to page descriptor 0x%lx, while it's actually "
            "on %lx\n", addr, page, pages[i]);
    }
    memset32(addr, i, PAGE_SIZE);
  }

  for (i = 0; i < nr_pages; i++) {
    memset32(tmpbuf, i, PAGE_SIZE);
    addr = page_address(pages[i]);
    res = memcmp(addr, tmpbuf, PAGE_SIZE);
    if (res) {
      panic("_Memory: FAIL: [%d] page at 0x%lx got "
            "corrupted\n", i, PHYS(addr));
    }
    /* Test reverse mapping */
    page = addr_to_page(addr);
    if (page != pages[i]) {
      panic("_Memory: FAIL: Reverse mapping addr=0x%lx lead "
            "to page descriptor 0x%lx, while it's actually "
            "on %lx\n", addr, page, pages[i]);
    }
    free_page(pages[i]);
    _disrupt();
  }

  for (i = 0; i < 100; i++) {
    _disrupt();
  }

  /* We've freed everything we allocated, number of free
   * pages should not change */
  count = _get_all_freepages_count(_CURRENT);
  if (old_count != count) {
    panic("_Memory: free pages leak; original number = %ld, "
          "current number = %ld\n", old_count, count);
  }
  printk("_Memory: %s: Success\n", __FUNCTION__);
  return 0;
}

/*
 * Consume all given zone's pages then observe the result
 * of overallocation. Use ZONE_ANY to consume all memory.
 */
__unused static void _test_zone_exhaustion(enum zone_id zid)
{
  struct page *page;
  struct zone *zone;
  uint64_t count;
  void* addr;

  zone = get_zone(zid);
  if (zid == ZONE_ANY) {
    count = _get_all_freepages_count(_CURRENT);
  } else {
    count = zone->freepages_count;
  }
  
  while (count--) {
    page = get_free_page(zid);
    assert(page_phys_addr(page) >= zone->start);
    assert((page_phys_addr(page) + PAGE_SIZE) <= zone->end);

    addr = page_address(page);
    memset64(addr, UINT64_MAX, PAGE_SIZE);

    printk("_Memory: Zone `%s': page %lx, %d free pages\n",
           zone->description, addr, count);
  }

  printk("_Memory: Allocated all zone's pages. System "
         "should panic from next allocation\n");

  (void) get_free_page(zid);
}

/*
 * Page allocation tests driver
 */
bool test_page_alloc(void)
{
  uint64_t count;

  _validate_zones_data();
  _test_boot_freepages_count();

  /* Beware of the pre-requisites first */
  /* _torture_pfdtable_add_range(); */

  count = _get_all_freepages_count(_CURRENT);
  if (count > PAGES_COUNT) {
    count = PAGES_COUNT;
  }

  printk("_Memory: Allocating (and seeding) %ld pages on "
         "each run\n", count);

  /* Burn, baby, burn */
  for (int i = 0; i < 100; i++) {
    printk("[%d] ", i);
    _test_pagealloc_coherency(count);
  }

  /* Pick one: */
  /* _test_zone_exhaustion(ZONE_1GB); */
  /* _test_zone_exhaustion(ZONE_ANY); */
  return true;
}

#else

bool test_page_alloc(void)
{
  return true;
}

#endif /* CONFIG_ARCH_X86 */

