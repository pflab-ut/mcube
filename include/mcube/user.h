/**
 * @file include/mcube/user.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_USER_H__
#define __MCUBE_MCUBE_USER_H__

#ifndef __ASSEMBLY__

int user_main(int argc, char *argv[]);
int user_arch_main(int argc, char *argv[]);
int user_test_main(int argc, char *argv[]);
int user_ap_main(int argc, char *argv[]);

bool test_atomic(void);
bool test_bitmap(void);
bool test_debug(void);
bool test_ext2(void);
bool test_file(void);
bool test_hash(void);
bool test_kmalloc(void);
bool test_kthread(void);
bool test_list(void);
bool test_page_alloc(void);
bool test_printk(void);
bool test_ring_buf(void);
bool test_sched(void);
bool test_socket(int argc, char *argv[]);
bool test_string(void);
bool test_unrolled_list(void);
bool test_vm(void);

#if CONFIG_ARCH_X86
bool test_apic(void);
bool test_percpu(void);
bool test_pit(void);
#endif /* CONFIG_ARCH_X86 */



#endif /* __ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_USER_H__ */

