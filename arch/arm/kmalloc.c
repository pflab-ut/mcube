/**
 * @file arch/arm/kmalloc.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void *kmalloc(size_t size)
{
  return malloc(size);
}

void kfree(void *addr)
{
  free(addr);
}

