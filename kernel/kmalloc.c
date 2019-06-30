/**
 * @file kernel/kmalloc.c
 *
 * @author Hiroyuki Chishiro
 */

#include <mcube/mcube.h>


#if CONFIG_ARCH_AXIS
spinlock_t kmalloc_lock;
#else
spinlock_t kmalloc_lock = INIT_SPINLOCK;
#endif /* CONFIG_ARCH_AXIS */

/**
 * @var heap
 * @brief Heap.
 */
static heap_t heap = {0};

/**
 * @var region[HEAP_INIT_SIZE]
 * @brief Region.
 */
static uint8_t region[HEAP_INIT_SIZE] = {0};

void *kmalloc(size_t size)
{
  void *ret;
  spin_lock(&kmalloc_lock);
  ret = heap_alloc(&heap, size);
  spin_unlock(&kmalloc_lock);
  return ret;
}

void kfree(void *ptr)
{
  if (!ptr) {
    return;
  }

  spin_lock(&kmalloc_lock);
  heap_free(&heap, ptr);
  spin_unlock(&kmalloc_lock);
}

void init_kmalloc(void)
{
  memset(&heap, 0, sizeof(heap_t));
  memset(region, 0, HEAP_INIT_SIZE);
  init_heap(&heap, (long) region);
}

