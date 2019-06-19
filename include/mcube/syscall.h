/**
 * @file include/mcube/syscall.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_SYSCALL_H__
#define __MCUBE_MCUBE_SYSCALL_H__


#if !CONFIG_ARCH_SIM

/**
 * @def SYS_sched
 * @brief call scheduler.
 */
#define SYS_sched 0

/**
 * @def SYS_end_job
 * @brief end job.
 */
#define SYS_end_job 1

/**
 * @def SYS_get_exec_time
 * @brief get execution time.
 */
#define SYS_get_exec_time 2

/**
 * @def SYS_write
 * @brief write.
 */
#define SYS_write 3

/**
 * @def SYS_get_cpu_id
 * @brief get CPU ID.
 */
#define SYS_get_cpu_id 4

/**
 * @def SYS_get_mode_level
 * @brief get mode level.
 */
#define SYS_get_mode_level 5

/**
 * @def SYS_move_to_kernel_level
 * @brief move to kernel level.
 */
#define SYS_move_to_kernel_level 6

/**
 * @def NR_SYSCALLS
 * @brief Number of system calls.
 */
#define NR_SYSCALLS 7

#endif /* !CONFIG_ARCH_SIM */

#ifndef __ASSEMBLY__


/* user-level APIs */

/**
 * @fn int call_sys_sched(void)
 * @brief call sys_sched().
 *
 * @return Zero if success.
 */
int call_sys_sched(void);

/**
 * @fn int call_sys_end_job(void)
 * @brief call sys_end_job().
 *
 * @return Zero if success.
 */
int call_sys_end_job(void);

/**
 * @fn unsigned long call_sys_get_exec_time(void)
 * @brief call sys_get_exec_time().
 *
 * @return Execution time.
 */
unsigned long call_sys_get_exec_time(void);

/**
 * @fn int call_sys_write(char *buf)
 * @brief call sys_write().
 *
 * @zero Zero if success.
 */
int call_sys_write(char *buf);

/**
 * @fn unsigned long call_sys_get_cpu_id(void)
 * @brief call sys_get_cpu_id().
 *
 * @return CPU ID.
 */
unsigned long call_sys_get_cpu_id(void);

/**
 * @fn int call_sys_get_mode_level(void)
 * @brief call sys_get_mode_level()
 *
 * @return Kernel or user level.
 */
int call_sys_get_mode_level(void);

/**
 * @fn int call_sys_move_to_kernel_level(void)
 * @brief call move_to_kernel_level()
 *
 * @return Zero if success.
 */
int call_sys_move_to_kernel_level(void);

/* kernel-level APIs */
/**
 * @fn asmlinkage int sys_sched(void)
 * @brief call scheduler.
 *
 * @param Zero if success.
 */
asmlinkage int sys_sched(void);

/**
 * @fn asmlinkage int sys_end_job(unsigned long *id_ptr)
 * @brief end job.
 *
 * @param id_ptr Pointer to ID.
 * @return Zero if success.
 */
asmlinkage int sys_end_job(unsigned long *id_ptr);

/**
 * @fn asmlinkage int sys_get_exec_time(unsigned long *id_ptr,
 *                                      unsigned long *cputime_ptr)
 * @brief get execution time.
 *
 * @param id_ptr Pointer to ID.
 * @param cputime_ptr Pointer to CPU time.
 * @return Execution time.
 */
asmlinkage int sys_get_exec_time(unsigned long *id_ptr,
                                 unsigned long *cputime_ptr);

/**
 * @fn asmlinkage int sys_mcube_write(char *buf)
 * @brief
 */
asmlinkage int sys_mcube_write(char *buf);

/**
 * @fn asmlinkage int sys_get_cpu_id(void)
 * @brief get CPU ID.
 *
 * @return CPU ID.
 */
asmlinkage int sys_get_cpu_id(void);

/**
 * @fn asmlinkage int sys_get_mode_level(void)
 * @brief Kernel or user level.
 *
 * @return Zero if success.
 */
asmlinkage int sys_get_mode_level(void);

/**
 * @fn asmlinkage int sys_move_to_kernel_level(void)
 * @brief Kernel or user level.
 */
asmlinkage int sys_move_to_kernel_level(void);

/**
 * @fn asmlinkage int sys_bad_syscall(int number)
 * @brief Bad system call.
 *
 * @param number Number.
 * @return Zero if success.
 */
asmlinkage int sys_bad_syscall(int number);

#if !CONFIG_ARCH_SIM

/* int syscall(arg0, ..., arg5) */
/**
 * @fn asmlinkage int syscall(int number, ...)
 * @brief System call.
 *
 * @param number Number.
 * @return Zero if success.
 */
asmlinkage int syscall(int number, ...);

/**
 * @fn void syscall_entry(void)
 * @brief System call entry.
 */
void syscall_entry(void);

/**
 * @fn void syscall_setup(void *table, uint64_t nr_syscalls)
 * @brief System call setup.
 */
void syscall_setup(void *table, uint64_t nr_syscalls);

/**
 * @var syscall_table[NR_SYSCALLS + 1]
 * @brief System call table.
 */
extern void *syscall_table[NR_SYSCALLS + 1];

#endif /* !CONFIG_ARCH_SIM */


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_MCUBE_SYSCALL_H__ */
