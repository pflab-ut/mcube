/**
 * @file user/test/test_kmalloc.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */
#include <mcube/mcube.h>

#if 0//CONFIG_ARCH_X86_64

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

  if (!(p = kmalloc(size))) {
    panic("Error: cannot allocate memory %lu\n", size);
  }

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

    if (!(p[i].p = kmalloc(size))) {
      panic("Error: cannot allocate memory %lu\n", size);
    }

    assert(IS_ALIGNED((uintptr_t)p[i].p, 16));
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

    if (!(p[i].p = kmalloc(size))) {
      panic("Error: cannot allocate memory %lu\n", size);
    }

    assert(IS_ALIGNED((uintptr_t)p[i].p, 16));
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
  void *a, *b, *c, *d, *e, *f, *g, *h;
  printk("test_kmalloc()\n");

  if (!(a = kmalloc(8))) {
    panic("Error: cannot allocate memory %lu\n", 8);
  }

  printk("a = 0x%lx size: 8\n", (unsigned long) a);
  kfree(a);

  if (!(b = kmalloc(128))) {
    panic("Error: cannot allocate memory %lu\n", 1028);
  }

  printk("b = 0x%lx size: 128\n", (unsigned long) b);

  if (!(c = kmalloc(8))) {
    panic("Error: cannot allocate memory %lu\n", 8);
  }

  printk("c = 0x%lx size: 8\n", (unsigned long) c);

  printk("\nfreeing b \n");
  kfree(b);

  if (!(d = kmalloc(8))) {
    panic("Error: cannot allocate memory %lu\n", 8);
  }

  printk("d = 0x%lx size: 8\n", (unsigned long) d);

  if (!(e = kmalloc(16))) {
    panic("Error: cannot allocate memory %lu\n", 16);
  }

  printk("e = 0x%lx size: 16\n", (unsigned long) e);

  if (!(f = kmalloc(8))) {
    panic("Error: cannot allocate memory %lu\n", 8);
  }

  printk("f = 0x%lx size: 8\n", (unsigned long) f);

  if (!(g = kmalloc(8))) {
    panic("Error: cannot allocate memory %lu\n", 8);
  }

  printk("g = 0x%lx size: 8\n", (unsigned long) g);

  printk("\nfreeing d and f\n");
  kfree(d);
  kfree(f);

  printk("\nfreeing e\n");
  kfree(e);

  if (!(h = kmalloc(128))) {
    panic("Error: cannot allocate memory %lu\n", 128);
  }

  printk("h = 0x%lx size: 128\n", (unsigned long) h);
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

#endif /* CONFIG_ARCH_X86_64 */
