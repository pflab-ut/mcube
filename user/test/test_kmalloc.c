/**
 * @file user/test/test_kmalloc.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */
#include <mcube/mcube.h>

#if CONFIG_ARCH_X86

/*
 * Kernel memory allocator test cases.
 *
 * Thanks to NewOS's Travis Geiselbrecht (giest) for aiding
 * me in outlying different MM testing scenarious over IRC!
 *
 * Those tests can get more harnessed once we have a random
 * number generator ready.
 */


/*
 * Max # of allocations to do during testing
 */
#define ALLOCS_COUNT  100000

/*
 * Big array of pointers to allocated addresses and
 * their area sizes.
 */
static struct {
  int size;
  void *p;
} p[ALLOCS_COUNT];

static char tmpbuf[PAGE_SIZE];

/*
 * As a way of memory allocator disruption, allocate,
 * memset, then free a given @(size)ed memory buffer.
 */
static void _disrupt(int size)
{
  char *p;

  p = kmalloc(size);
  memset(p, 0xff, size);
  kfree(p);
}

/*
 * @count: number of allocations to perform
 * @rounded: if true, only do power-of-2-rounded allocs
 */
static void _test_allocs(int count, int rounded)
{
  int i, size;

  size = (rounded) ? MINALLOC_SZ : 1;

  for (i = 0; i < count; i++) {
    _disrupt(size);

    p[i].p = kmalloc(size);
    assert(is_aligned((uintptr_t)p[i].p, 16));
    p[i].size = size;

    if (rounded) {
      memset32(p[i].p, i, size);
      size *= 2;
      size = (size > MAXALLOC_SZ) ? MINALLOC_SZ : size;
    } else {
      memset(p[i].p, i, size);
      size++;
      size = (size > MAXALLOC_SZ) ? 1 : size;
    }
  }

  for (i = 0; i < count; i++) {
    size = p[i].size;
    _disrupt(size);

    (rounded) ? memset32(tmpbuf, i, size) : memset(tmpbuf, i, size);

    if (__builtin_memcmp(p[i].p, tmpbuf, size)) {
      panic("_Bucket: FAIL: [%d] buffer at 0x%lx, with size "
            "%d bytes got corrupted", i, p[i].p, size);
    }

    kfree(p[i].p);

    size = ((size / 2) > 1) ? size / 2 : MINALLOC_SZ;
    p[i].p = kmalloc(size);
    assert(is_aligned((uintptr_t)p[i].p, 16));
    p[i].size = size;
    (rounded) ? memset32(p[i].p, i, size) : memset(p[i].p, i, size);

    _disrupt(size);
  }

  for (i = 0; i < count; i++) {
    _disrupt(45);

    size = p[i].size;
    (rounded) ? memset32(tmpbuf, i, size) : memset(tmpbuf, i, size);

    if (__builtin_memcmp(p[i].p, tmpbuf, size)) {
      panic("_Bucket: FAIL: [%d] buffer at 0x%lx, with size "
            "%d bytes got corrupted", i, p[i].p, size);
    }

    kfree(p[i].p);
    _disrupt(32);
  }

  printk("_Bucket: %s: Success\n", __FUNCTION__);
}


/*
 * Memory allocator tests driver
 *
 * For extra fun, after removing the thread-unsafe
 * globals, call this 3 or 4 times in parallel.
 */
bool test_kmalloc(void)
{
  uint64_t i, count, repeat;

  count = ALLOCS_COUNT;
  repeat = 100;

  for (i = 0; i < repeat; i++) {
    printk("[%d] ", i);
    _test_allocs(count, 1);
  }

  memset(p, 0, sizeof(p));

  for (i = 0; i < repeat; i++) {
    printk("[%d] ", i);
    _test_allocs(count, 0);
  }

  for (i = MINBUCKET_IDX; i <= MAXBUCKET_IDX; i++) {
    printk("Buf size = %d: free bufs = %d, total pages requested "
           "= %d\n", 1 << i, kmembuckets[i].totalfree,
           kmembuckets[i].totalpages);
  }

  return true;
}

#else

bool test_kmalloc(void)
{
  printk("test_kmalloc()\n");
  void *a = kmalloc(8);
  printk("a = %lu size: 8\n", (unsigned long) a);
  kfree(a);
  void *b = kmalloc(128);
  printk("b = %lu size: 128\n", (unsigned long) b);
  void *c = kmalloc(8);
  printk("c = %lu size: 8\n", (unsigned long) c);

  printk("\nfreeing b \n");
  kfree(b);

  void *d = kmalloc(8);
  printk("d = %lu size: 8\n", (unsigned long) d);

  void *e = kmalloc(16);
  printk("e = %lu size: 16\n", (unsigned long) e);

  void *f = kmalloc(8);
  printk("f = %lu size: 8\n", (unsigned long) f);

  void *g = kmalloc(8);
  printk("g = %lu size: 8\n", (unsigned long) g);

  printk("\nfreeing d and f\n");
  kfree(d);
  kfree(f);

  printk("\nfreeing e\n");
  kfree(e);

  void *h = kmalloc(128);
  printk("h = %lu size: 128\n", (unsigned long) h);
  printk("\n");

  for (int i = 1; i <= 2048; i += i) {
    printk("size: %lu -> bin: %lu\n", i, get_bin_index(i));
  }

  printk("\nfreeing a, c, g, h\n");
  kfree(a);
  kfree(c);
  kfree(g);
  kfree(h);

  return true;
}

#endif /* CONFIG_ARCH_X86 */
