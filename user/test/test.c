/**
 * @file user/test/test.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


int user_test_main(void)
{
  test_atomic();
#if 0
  test_bitmap();
  test_ext2();
  test_hash();
  test_kmalloc();
  test_printk();
  test_ring_buf();
  test_string();
  test_vm();
#endif
  return 0;
}
