/**
 * @file include/sim/thread.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_SIM_THREAD_H__
#define	__MCUBE_SIM_THREAD_H__

#ifndef __ASSEMBLY__

struct cpu_context {
  /* dummy */
  unsigned long eax;
  unsigned long ebx;
  unsigned long ecx;
  unsigned long edx;
};

#define INIT_CPU_CONTEXT (struct cpu_context) {0}


static inline int get_pthread_id(void)
{
	return syscall(SYS_gettid);
}

#if CONFIG_USER_TASK_PTHREAD

/**
 * @brief POSIX thread argument
 *
 * The pthread_arg structure has POSIX thread information.
 */
struct pthread_arg {
	/** Thread ID. */
	uint32_t thid;
	/** Function pointer to execution. */
	int (*func)(void *arg);
};

typedef struct pthread_arg pthread_arg;

extern void *pthread_func(void *arg);
#endif /* CONFIG_USER_TASK_PTHREAD */

extern void exec_thread(void);


#endif /* !__ASSEMBLY__ */


#endif	/* __MCUBE_SIM_THREAD_H__ */
