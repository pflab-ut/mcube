/**
 * @file kernel/kmalloc.c
 *
 * @author Hiroyuki Chishiro
 */

#include <mcube/mcube.h>

#if 0//CONFIG_ARCH_X86
/*
 * Kernel Memory Allocator
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * This is an implementation of the McKusick-Karels kernel memory
 * allocator (with slight modifications) as described in:
 *
 *  'Design of a General Purpose Memory Allocator for the 4.3BSD
 *  UNIX Kernel', Kirk McKusick and J. Karels, Berkely, Usenix 1988
 *
 * and as also clearly described and compared to other allocators in the
 * greatest Unix book ever written:
 *
 *  'UNIX Internals: The New Frontiers', Uresh Vahalia, Prentice
 *  Hall, 1996
 *
 * One of the weakest points of classical power-of-2 allocators like the
 * one used in 4.2BSD was keeping memory blocks metadata inside the blocks
 * themselves. This led to worst-case behaviour of 50% memory wastage for
 * power-of-2-sized allocations.
 *
 * Due to compilers data structures alignments and the word sizes of cur-
 * rent processors, a huge number of kernel structures were in fact power
 * of 2 sized. This lead to unacceptable wastage of half kernel memory.
 *
 * McKusick solved this problem by keeping the metadata out of the blocks
 * themselves to another global data structure (kmemsize). We do something
 * similar by storing blocks metadata in the page frame descriptors (mm.h).
 * This happens at the price of mandating one single size of buffers for
 * each physical memory page frame requested from the page allocator.
 *
 * The big picture of the algorithm is that the kernel heap memory is org-
 * anized in power-of-2 free allocation units lists from 16 to 4096 bytes.
 * Those allocation units (buffers) are created on demand. If no empty buf
 * of a specific size exist (as in the initial state), a page is requested
 * from the page allocator and tokenized to buffers of the requested size.
 *
 * Terminology:
 * - 'Bucket': Free memory is organized in 'buckets', where each bucket
 * contains one head of the power-of-2 lists + its bookkeeping information.
 *
 * - 'Buffer': The smallest unit of allocation, with sizes ranging from 16
 * to 4096 bytes. Pages are tokenized to groups of equaly sized buffers.
 *
 * Finally:
 * I chose this algorithm for its elegance and simplicity. It sufferes
 * from some drawbacks like the inability of coalescing free memory blocks
 * as in the buddy system, leading to external fragmentation in case of a
 * bursty usage pattern of one size. There's also no simple way for retur-
 * ning pages to the page allocator. Nonetheless, the algorithm is still a
 * super-fast O(1), and it does handle small requests effeciently :)
 */

struct bucket kmembuckets[MAXBUCKET_IDX + 1];

/*
 * Request a page from the page allocator and tokenize
 * it to buffers of the given bucket's buffer sizes.
 *
 * BIG-FAT-NOTE! Call with bucket lock held
 */
static void *get_tokenized_page(int bucket_idx)
{
  struct page *page;
  char *buf, *start, *end;
  int buf_len;

  page = get_free_page(ZONE_ANY);
  page->in_bucket = 1;
  page->bucket_idx = bucket_idx;

  start = page_address(page);
  end = start + PAGE_SIZE;

  buf = start;
  buf_len = 1 << bucket_idx;

  while (buf < (end - buf_len)) {
    *(void **) buf = buf + buf_len;
    sign_buf(buf, FREEBUF_SIG);
    buf += buf_len;
  }

  *(void **) buf = NULL;
  sign_buf(buf, FREEBUF_SIG);

  return start;
}


void *__kmalloc(int bucket_idx)
{
  struct bucket *bucket;
  char *buf;

  assert(bucket_idx <= MAXBUCKET_IDX);
  bucket = &kmembuckets[bucket_idx];

  spin_lock(&bucket->lock);

  if (bucket->head) {
    buf = bucket->head;
    bucket->head = *(void **)(bucket->head);
    goto out;
  }

  assert(bucket->totalfree == 0);
  buf = get_tokenized_page(bucket_idx);
  bucket->head = *(void **)buf;
  bucket->totalpages++;
  bucket->totalfree = PAGE_SIZE / (1 << bucket_idx);

out:
  bucket->totalfree--;
  spin_unlock(&bucket->lock);

  assert(is_free_buf(buf));
  sign_buf(buf, ALLOCBUF_SIG);
  return buf;
}


/*
 * For given desired allocation size (@size), calculate the suit-
 * able bucket index in the kmembuckets table and pass it to the
 * real kmalloc. Bucket index is log2 the @size.
 *
 * We do this 'fake' and 'real' kmalloc divide to calculate bucket
 * indeces at compile-time for constant expressions, thanks to the
 * now-standard compilers' constant-folding capabilities. Luckily,
 * this is the most common case as sizeof() returns a constant!
 *
 * For a number of GCC versions, constant propagation with loops
 * is troublesome. We unfold the loop to avoid run-time costs.
 *
 * For the run-time side, although a binary search will lead to
 * less instructions, it will result in pipeline stalls: reasona-
 * ble instruction-prefetch strategies can't predict all branches.
 *
 * Returned addresses are at-least 16-byte aligned.
 */
void *kmalloc(size_t size)
{
  assert(size > 0);

  compiler_assert((MINALLOC_SZ * 256) == MAXALLOC_SZ);
  compiler_assert((MINBUCKET_IDX + 8) == MAXBUCKET_IDX);

  if (size <= MINALLOC_SZ) {
    return __kmalloc(MINBUCKET_IDX);
  }

  if (size <= MINALLOC_SZ *   2) {
    return __kmalloc(MINBUCKET_IDX + 1);
  }

  if (size <= MINALLOC_SZ *   4) {
    return __kmalloc(MINBUCKET_IDX + 2);
  }

  if (size <= MINALLOC_SZ *   8) {
    return __kmalloc(MINBUCKET_IDX + 3);
  }

  if (size <= MINALLOC_SZ *  16) {
    return __kmalloc(MINBUCKET_IDX + 4);
  }

  if (size <= MINALLOC_SZ *  32) {
    return __kmalloc(MINBUCKET_IDX + 5);
  }

  if (size <= MINALLOC_SZ *  64) {
    return __kmalloc(MINBUCKET_IDX + 6);
  }

  if (size <= MINALLOC_SZ * 128) {
    return __kmalloc(MINBUCKET_IDX + 7);
  }

  if (size <= MINALLOC_SZ * 256) {
    return __kmalloc(MINBUCKET_IDX + 8);
  }

  panic("Malloc: %d bytes requested; can't support > %d bytes", size,
        MAXALLOC_SZ);

  return NULL;
}

/*
 * Sanity checks to assure we're passed a valid address:
 * - Does given address reside in an allocated page?
 * - If yes, does it reside in a page allocated by us?
 * - If yes, does it reside in a buffer-aligned address?
 * - If yes, does it reside in a not-already-freed buffer?
 * Any NO above means an invalid address, thus a kernel bug
 */
void kfree(void *addr)
{
  struct page *page;
  struct bucket *bucket;
  int buf_size;
  char *buf;

  buf = addr;
  page = addr_to_page(buf);
  bucket = &kmembuckets[page->bucket_idx];

  if (page_is_free(page)) {
    panic("Bucket: Freeing address 0x%lx which resides in "
          "an unallocated page frame", buf);
  }

  if (!page->in_bucket) {
    panic("Bucket: Freeing address 0x%lx which resides in "
          "a foreign page frame (not allocated by us)", buf);
  }

  buf_size = 1 << page->bucket_idx;

  if (!IS_ALIGNED((uintptr_t) buf, buf_size)) {
    panic("Bucket: Freeing invalidly-aligned 0x%lx address; "
          "bucket buffer size = 0x%lx\n", buf, buf_size);
  }

  if (is_free_buf(buf)) {
    panic("Bucket: Freeing already free buffer at 0x%lx, "
          "with size = 0x%lx bytes", buf, buf_size);
  }

  sign_buf(buf, FREEBUF_SIG);

  spin_lock(&bucket->lock);

  *(void **) buf = bucket->head;
  bucket->head = buf;
  bucket->totalfree++;

  spin_unlock(&bucket->lock);
}

void init_kmalloc(void)
{
  for (int i = 0; i <= MAXBUCKET_IDX; i++) {
    spin_init(&kmembuckets[i].lock);
  }
}

#else

#if CONFIG_ARCH_AXIS
spinlock_t kmalloc_lock;
#else
spinlock_t kmalloc_lock = INIT_SPINLOCK;
#endif /* CONFIG_ARCH_AXIS */


heap_t heap = {0};
uint8_t region[HEAP_INIT_SIZE] = {0};

void *kmalloc(size_t size)
{
  void *ret;
  spin_lock(&kmalloc_lock);
  ret = heap_alloc(&heap, size);
  spin_unlock(&kmalloc_lock);
  return ret;
}

void kfree(void *addr)
{
  if (!addr) {
    return;
  }

  spin_lock(&kmalloc_lock);
  heap_free(&heap, addr);
  spin_unlock(&kmalloc_lock);
}

void init_kmalloc(void)
{
  printk("init_kmalloc()\n");
  memset(&heap, 0, sizeof(heap_t));
  memset(region, 0, HEAP_INIT_SIZE);
  init_heap(&heap, (long) region);
}

#endif /* CONFIG_ARCH_X86 */

