/**
 * @file arch/sim/thread.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if CONFIG_USER_TASK_PTHREAD
#include <pthread.h>
#endif /* CONFIG_USER_TASK_PTHREAD */


void sim_wait(uint64_t count)
{
	while (rdtsc() < count)
		;	
}


void __attribute__((noreturn)) sim_exit(int status)
{
#if CONFIG_USER_TASK_PTHREAD
	pthread_exit(NULL);
#else
	exit(status);
#endif /* CONFIG_USER_TASK_PTHREAD */
}


/* %eax = number, %ebx = arg1 */
asmlinkage int syscall0(int number)
{
#if 1
	return 0;
#else
	int ret;
	/* default offset is 0x4 and # of save registers is 0, so that base offset is 0x4 */
	/* 0x4(%esp) = number */
	asm volatile("movl 0x4(%esp), %eax");
	asm volatile("int %0" :: "i"(SYSCALL_IRQ));
	/* set return value */
	asm volatile("movl %0, %%eax" : "=r"(ret));
	return ret;
#endif
}


/* %eax = number, %ebx = arg1 */
asmlinkage int syscall1(int number, int arg1)
{
#if 1
	return 0;
#else
	int ret;
	/* save registers for arguments */
	asm volatile("pushl %ebx");
	/* default offset is 0x4 and # of save registers is 1, so that base offset is 0x8 */
	/* 0xc(%esp) = arg1 */
	asm volatile("movl 0xc(%esp), %ebx");
	/* 0x8(%esp) = number */
	asm volatile("movl 0x8(%esp), %eax");
	asm volatile("int %0" :: "i"(SYSCALL_IRQ));
	/* resume registers */
	asm volatile("popl %ebx");
	/* set return value */
	asm volatile("movl %0, %%eax" : "=r"(ret));
	return ret;
#endif
}

/* %eax = number, %ebx = arg1, %ecx = arg2 */
asmlinkage int syscall2(int number, int arg1, int arg2)
{
#if 1
	return 0;
#else
	int ret;
	/* save registers for arguments */
	asm volatile("pushl %ecx");
	asm volatile("pushl %ebx");
	/* default offset is 0x4 and # of save registers is 1, so that base offset is 0xc */
	/* 0x14(%esp) = arg2 */
	asm volatile("movl 0x14(%esp), %ecx");
	/* 0x10(%esp) = arg1 */
	asm volatile("movl 0x10(%esp), %ebx");
	/* 0xc(%esp) = number */
	asm volatile("movl 0xc(%esp), %eax");
	/* do syscall */
	asm volatile("int %0" :: "i"(SYSCALL_IRQ));
	/* resume registers */
	asm volatile("popl %ebx");
	asm volatile("popl %ecx");
	/* set return value */
	asm volatile("movl %0, %%eax" : "=r"(ret));
	return ret;
#endif
}


/**
 * The run_user_thread() function runs user thread.
 */
void __attribute__((noreturn)) run_user_thread(void)
{
  unsigned long cpu = get_cpu_id();
  void *ret = NULL;
  //  prinf("begin_user_thread()\n");
  if (current_th[cpu]->run_user_func) {
    ret = (*current_th[cpu]->run_user_func)(current_th[cpu]->arg);
  }
  if (ret) {
    printf("ret = 0x%lx\n", (unsigned long) ret);
  }
  halt();
  inf_loop();
}

