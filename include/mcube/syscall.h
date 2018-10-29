/**
 * @file include/mcube/syscall.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_SYSCALLS_H__
#define	__MCUBE_MCUBE_SYSCALLS_H__


#define	SYS_sched 0
#define SYS_end_job 1
#define	SYS_get_exec_time 2
#define SYS_write 3
#define SYS_get_cpu_id 4
#define SYS_get_mode_level 5
#define	NR_syscalls 6



#ifndef __ASSEMBLY__


int call_sys_sched(void);
int call_sys_end_job(void);
int call_sys_get_exec_time(void);
int call_sys_write(char *buf);
int call_sys_get_cpu_id(void);
int call_sys_get_mode_level(void);


#if CONFIG_ARCH_SIM

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>


#elif CONFIG_ARCH_X86

asmlinkage int syscall0(int number);
asmlinkage int syscall1(int number, unsigned long arg1);
asmlinkage int syscall2(int number, unsigned long arg1, unsigned long arg2);

#elif CONFIG_ARCH_ARM || CONFIG_ARCH_AXIS

#else
#error "Unknown Architecture"
#endif /* CONFIG_ARCH_SIM */

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_SYSCALLS_H__ */
