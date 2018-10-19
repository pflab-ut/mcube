/**
 * @file lib/syscall.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


asmlinkage int sys_sched(void)
{
	printk("sys_sched()\n");
	return 0;
}

asmlinkage int sys_end_job(unsigned long *id_ptr)
{
	struct thread_struct *th = &ths[*id_ptr-1];
	printk("sys_end_job()\n");
	printk("id_ptr = %lu\n", *id_ptr);
	th->sched.remaining = 0;
	do_end_job(th);
	return 0;
}


asmlinkage int sys_get_exec_time(unsigned long *id_ptr, unsigned long *cputime_ptr)
{
	struct thread_struct *th = &ths[*id_ptr-1];
	printk("sys_get_cputime()\n");
#if 0
	printk("id_ptr = 0x%lx id = %lu\n", id_ptr, *id_ptr);
	printk("cputime_ptr = 0x%lx cputime = %lu\n", cputime_ptr, *cputime_ptr);
#endif
	*cputime_ptr = th->sched.sum_exec_time + tsc2nsec(get_current_cpu_time() - th->sched.begin_cpu_time);
	return 0;
}


asmlinkage int sys_bad_syscall(int number)
{
	printk("sys_bad_syscall()\n");
	printk("number = %d\n", number);
	inf_loop();
	/* not reached */
	return -1;
}

asmlinkage void sys_write(char *buf)
{
  puts(buf);
}

void *const syscall_table[] = {
  sys_sched,
  sys_end_job,
  sys_get_exec_time,
  sys_write,
  sys_bad_syscall
};
