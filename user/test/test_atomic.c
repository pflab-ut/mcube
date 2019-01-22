/**
 * @file user/test/test_atomic.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

int test_atomic(void)
{
  printk("_Atomic: 0 -> 99 should be printed:\n");
  for (uint64_t i = 0; i < 100; atomic_inc(&i)) {
    printk("%d ", i);
  }
  putchar('\n');

  printk("_Atomic: 0xfffffffffffffff0 - 0xffffffffffffffff should "
         "be printed:\n");
  for (uint64_t i = -0x10; i != 0; atomic_inc(&i)) {
    printk("0x%lx ", i);
  }
  putchar('\n');
  return 0;
}
