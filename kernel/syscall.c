/**
 * @file kernel/syscall.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


asmlinkage int sys_sched(void)
{
  printk("sys_sched()\n");
  //  do_sched();
  return 0;
}

asmlinkage int sys_end_job(unsigned long *id_ptr)
{
  printk("sys_end_job()\n");
  struct thread_struct *th = &ths[*id_ptr - 1];
  printk("id_ptr = %lu\n", *id_ptr);
  th->sched.remaining = 0;
  do_end_job(th);
  return 0;
}


asmlinkage int sys_get_exec_time(unsigned long *id_ptr,
                                 unsigned long *cputime_ptr)
{
  struct thread_struct *th = &ths[*id_ptr - 1];
  printk("sys_get_cputime()\n");
#if 0
  printk("id_ptr = 0x%lx id = %lu\n", id_ptr, *id_ptr);
  printk("cputime_ptr = 0x%lx cputime = %lu\n", cputime_ptr, *cputime_ptr);
#endif
  *cputime_ptr = th->sched.sum_exec_time + tsc2nsec(get_current_cpu_time()
                                                    - th->sched.begin_cpu_time);
  return 0;
}

asmlinkage int sys_mcube_write(char *buf)
{
  return puts(buf);
}

asmlinkage int sys_get_cpu_id(void)
{
  printk("sys_get_cpu_id()\n");
  return get_cpu_id();
}

asmlinkage int sys_get_mode_level(void)
{
#if CONFIG_ARCH_ARM_RASPI3 || CONFIG_ARCH_ARM_SYNQUACER
  unsigned long spsr;
  asm volatile("mrs %0, spsr_el1" : "=r"(spsr));

  //  printk("spsr_el1 = 0x%lx\n", spsr);
  if ((spsr & SPSR_ELx_MODE_FIELD_MASK) == SPSR_ELx_MODE_FIELD_EL0t) {
    return USER_LEVEL;
  } else {
    return KERNEL_LEVEL;
  }

#else
  return KERNEL_LEVEL;
#endif /* CONFIG_ARCH_ARM_RASPI3 || CONFIG_ARCH_ARM_SYNQUACER */
}

asmlinkage int sys_move_to_kernel_level(void)
{
#if CONFIG_ARCH_ARM_RASPI3 || CONFIG_ARCH_ARM_SYNQUACER
  /* NOTE: this implementation does not work well and this problem may depend on Qemu. */
  unsigned long spsr;
  unsigned long elr;
  //  printk("sys_move_to_kernel_level()\n");
  asm volatile("mrs %0, spsr_el1" : "=r"(spsr));
  printk("spsr = 0x%lx\n", spsr);
  spsr |= (spsr & SPSR_ELx_MODE_FIELD_MASK) | SPSR_ELx_MODE_FIELD_EL1h;
  //  spsr = 0x60000005;
  //  spsr = 0x60000015;
  //  spsr = 0x15;
  asm volatile("msr spsr_el1, %0" :: "r"(spsr));
  printk("spsr = 0x%lx\n", spsr);
  asm volatile("mrs %0, elr_el1" : "=r"(elr));
  printk("elr = 0x%x\n", elr);
  //  printk("sys_get_mode_level() = %d\n", call_sys_get_mode_level());
#endif /* CONFIG_ARCH_ARM_RASPI3 || CONFIG_ARCH_ARM_SYNQUACER */
  return 0;
}

asmlinkage int sys_bad_syscall(int number)
{
  printk("sys_bad_syscall()\n");
  printk("number = %d\n", number);
  return -1;
}

void *syscall_table[] = {
  sys_sched,
  sys_end_job,
  sys_get_exec_time,
  sys_mcube_write,
  sys_get_cpu_id,
  sys_get_mode_level,
  sys_move_to_kernel_level,
  sys_bad_syscall
};
