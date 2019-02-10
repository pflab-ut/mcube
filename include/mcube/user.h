/**
 * @file include/mcube/user.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_USER_H__
#define __MCUBE_MCUBE_USER_H__

#ifndef __ASSEMBLY__

int user_main(void);
int user_arch_main(void);
int user_test_main(void);

bool test_atomic(void);
bool test_bitmap(void);
bool test_ext2(void);
bool test_hash(void);
bool test_kmalloc(void);
bool test_page_alloc(void);
bool test_printk(void);
bool test_ring_buf(void);
bool test_string(void);
bool test_vm(void);

#endif /* __ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_USER_H__ */

