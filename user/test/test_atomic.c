/**
 * @file user/test/test_atomic.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Copyright (C) 2011 Ahmed S. Darwish <darwish.07@gmail.com>
 */
#include <mcube/mcube.h>

bool test_atomic(void)
{
  printk("_Atomic: 0 -> 99 should be printed:\n");
  for (uint64_t i = 0; i < 100; atomic_inc(&i)) {
    printk("%d ", i);
  }
  putchar('\n');

  printk("_Atomic: 0xfffffffffffffff0 - 0xffffffffffffffff should be printed:\n");
  for (uint64_t i = -0x10; i != 0; atomic_inc(&i)) {
  //  for (uint64_t i = -0x10; i != 0; i++) {
    printk("0x%016lx ", i);
  }
  putchar('\n');
  return true;
}
