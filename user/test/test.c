/**
 * @file user/test/test.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

bool test_atomic(void);
bool test_bitmap(void);
bool test_ext2(void);
bool test_hash(void); 
bool test_kmalloc(void);
bool test_printk(void); 
bool test_ring_buf(void);
bool test_string(void);
bool test_vm(void); 


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
