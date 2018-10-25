/**
 * @file lib/syscall.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


asmlinkage int sys_sched(void)
{
	print("sys_sched()\n");
	return 0;
}

asmlinkage int sys_end_job(unsigned long *id_ptr)
{
	struct thread_struct *th = &ths[*id_ptr-1];
	print("sys_end_job()\n");
	print("id_ptr = %lu\n", *id_ptr);
	th->sched.remaining = 0;
	do_end_job(th);
	return 0;
}


asmlinkage int sys_get_exec_time(unsigned long *id_ptr, unsigned long *cputime_ptr)
{
	struct thread_struct *th = &ths[*id_ptr-1];
	print("sys_get_cputime()\n");
#if 0
	print("id_ptr = 0x%lx id = %lu\n", id_ptr, *id_ptr);
	print("cputime_ptr = 0x%lx cputime = %lu\n", cputime_ptr, *cputime_ptr);
#endif
	*cputime_ptr = th->sched.sum_exec_time + tsc2nsec(get_current_cpu_time() - th->sched.begin_cpu_time);
	return 0;
}

asmlinkage int sys_write(char *buf)
{
  return puts(buf);
}

asmlinkage int sys_get_cpu_id(void)
{
  return get_cpu_id();
}

asmlinkage int sys_get_mode_level(void)
{
#if CONFIG_ARCH_ARM
  extern unsigned long ret_from_sys;
  unsigned long addr;
  asm volatile("mov %0, X30" : "=r"(addr));
  //  print("el0_ni_sys = 0x%lx\n", (unsigned long) &ret_from_sys);
  //  print("addr = 0x%lx\n", addr);
  if ((unsigned long) &ret_from_sys == addr) {
    /* call from system call in EL0, and hence return value must be 0. */
    //    print("call from system call\n");
    return 0;
  } else {
    /* otherwise */
    return get_el();
  }
#else
  return 0;
#endif
}

asmlinkage int sys_bad_syscall(int number)
{
	print("sys_bad_syscall()\n");
	print("number = %d\n", number);
	inf_loop();
	/* not reached */
	return -1;
}

void *const sys_call_table[] = {
  sys_sched,
  sys_end_job,
  sys_get_exec_time,
  sys_write,
  sys_get_cpu_id,
  sys_get_mode_level,
  sys_bad_syscall
};
