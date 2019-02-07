/**
 * @file user/test/test.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

bool test_atomic(void);
bool test_bitmap(void);
bool test_ext2(void);
bool test_ring_buf(void);
bool test_string(void);
bool test_kmalloc(void);
bool test_printk(void); 


int user_test_main(void)
{
  test_atomic();
#if 0
  test_bitmap();
  test_ext2();
  test_kmalloc();
  test_printk();
  test_ring_buf();
  test_string();
#endif
  return 0;
}
