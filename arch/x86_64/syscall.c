/**
 * @file arch/x86_64/syscall.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


int call_sys_sched(void)
{
  syscall(SYS_sched);
  return 0;
}

int call_sys_end_job(void)
{
  unsigned long cpu = get_cpu_id();
  syscall(SYS_end_job, (unsigned long) &current_th[cpu]->id);
  return 0;
}

unsigned long call_sys_get_exec_time(void)
{
  unsigned long cpu_time = 0;
  unsigned long cpu = get_cpu_id();
  syscall(SYS_get_exec_time, (unsigned long) &current_th[cpu]->id,
          (unsigned long) &cpu_time);
  return cpu_time;
}


int call_sys_write(char *buf)
{
  int ret = 0;
  syscall(SYS_write, &ret, (unsigned long) buf);
  return 0;
}

unsigned long call_sys_get_cpu_id(void)
{
  unsigned long ret = 0;
  syscall(SYS_get_cpu_id, (unsigned long) &ret);
  return ret;
}

int call_sys_get_mode_level(void)
{
  int ret = 0;
  syscall(SYS_get_mode_level, (unsigned long) &ret);
  return ret;
}

int call_sys_move_to_kernel_level(void)
{
  int ret = 0;
  syscall(SYS_move_to_kernel_level, (unsigned long) &ret);
  return ret;
}

void init_syscall(void)
{
  uint64_t val;
  cpuid_info_t cinfo;

  /* Check CPUID.0x80000001 for syscall support */
  cpuid(0x80000001, &cinfo);

  if (!is_syscall_available(&cinfo) || !is_long_mode_available(&cinfo)) {
    panic("syscall is not supported.");
  }

  printk("enable syscall\n");

  /* EFLAG mask */
  wrmsr(MSR_IA32_FMASK, 0x0002);

  /* Entry point to the system call */
  wrmsr(MSR_IA32_LSTAR, (uint64_t) syscall_entry);

  /* Syscall/sysret segments */
  val = GDT_RING0_CODE_SEL | ((GDT_RING3_CODE32_SEL + 3) << 16);
  wrmsr(MSR_IA32_STAR, val << 32);

  /* Enable syscall */
  val = rdmsr(MSR_IA32_EFER);
  val |= 1;
  wrmsr(MSR_IA32_EFER, val);

  /* Call assembly syscall_setup() */
  //  syscall_setup(syscall_table, (uint64_t) NR_SYSCALLS);
  printk("call change_ring0_to_ring3()\n");
  change_ring0_to_ring3(GDT_RING3_CODE64_SEL + 3);

  //  __asm__ __volatile__ ("movq %cr0,%rax");
  //  call_sys_sched();
  //  call_sys_end_job();
  //  call_sys_get_cpu_id();
  for (;;)
    ;
}
