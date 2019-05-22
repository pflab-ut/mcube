/**
 * @file arch/x86/syscall.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
//============================================================================
/// @brief      System call support.
//
// Copyright 2016 Brett Vickers.
// Use of this source code is governed by a BSD-style license that can
// be found in the MonkOS LICENSE file.
//============================================================================


int call_sys_sched(void)
{
  //  syscall0(SYS_sched);
  return 0;
}

int call_sys_end_job(void)
{
  //  unsigned long cpu = get_cpu_id();
  //  syscall1(SYS_end_job, (unsigned long) &current_th[cpu]->id);
  return 0;
}

unsigned long call_sys_get_exec_time(void)
{
  //  unsigned long cpu = get_cpu_id();
  unsigned long cpu_time = 0;
  //  syscall2(SYS_get_exec_time, (unsigned long) &current_th[cpu]->id, (unsigned long) &cpu_time);
  return cpu_time;
}


int call_sys_write(char *buf)
{
  return puts(buf);
}

int call_sys_get_mode_level(void)
{
  int ret = 0;
  //  syscall1(SYS_get_mode_level, (unsigned long) &ret);
  return ret;
}


#if 0
int call_user_sys_write(char *buf)
{
  int ret;
  //  syscall1(SYS_write, (unsigned long) buf);
  printk("&ret = 0x%lx\n", (unsigned long) &ret);
  //  syscall2(SYS_write, (unsigned long) &ret, (unsigned long) buf);
  return ret;
}


static void handle_syscall(interrupt_context_t *context)
{
  /*
   * EAX: System Call ID (i.e., SYSCALL_IRQ 0x40)
   * RDI: 1st Argument
   * RSI: 2nd Argument
   * RDX: 3rd Argument
   * R10: 4th Argument
   * R8:  5th Argument
   * R9:  6th Argument
   */

  //  unsigned long cpu = get_cpu_id();
  //  unsigned long rcx;
  //asm volatile("movq %0, rcx" : "=r"(rcx));
  // Do nothing for now.
  //  printk("handle_syscall()\n");
  //  dump_registers(&context->regs);

  /*
  switch (context->regs.rdi) {
  case SYS_sched:
    do_sched();
    break;
  case SYS_end_job:
    do_end_job(current_th[cpu]);
    break;
  case SYS_get_exec_time:
    *((volatile unsigned long *) context->regs.rsi)
      = current_th[cpu]->sched.wcet - current_th[cpu]->sched.remaining;
    break;
  case SYS_write:
    //    printk("context->regs.rdx = 0x%lx\n", context->regs.rdx);
    *((volatile unsigned long *) context->regs.rsi)
      = puts((char *) context->regs.rdx);
    break;
  case SYS_get_cpu_id:
    break;
  case SYS_get_mode_level:
    *((volatile unsigned long *) context->regs.rsi) = KERNEL_LEVEL;
    break;
  default:
    printk("Error: Unknown System Call ID %ld\n", context->regs.rdi);
    break;
  }
  */
}
#endif

void init_syscall(void)
{
  uint64_t val;
  cpuid_info_t cinfo;

  /* Check CPUID.0x80000001 for syscall support */
  cpuid(0x80000001, &cinfo);

  if (!((cinfo.rdx >> 11) & 1) || !((cinfo.rdx >> 29) & 1)) {
    panic("syscall is not supported.");
  }

  printk("syscall is OK!\n");

  /* EFLAG mask */
  wrmsr(MSR_IA32_FMASK, 0x0002);

  /* Entry point to the system call */
  wrmsr(MSR_IA32_LSTAR, (uint64_t) syscall_entry);
#if 0

  /* Syscall/sysret segments */
  val = GDT_RING0_CODE_SEL | ((GDT_RING3_CODE32_SEL + 3) << 16);
  wrmsr(MSR_IA32_STAR, val << 32);

  /* Enable syscall */
  val = rdmsr(MSR_IA32_EFER);
  val |= 1;
  wrmsr(MSR_IA32_EFER, val);

  /* Call assembly syscall_setup() */
  syscall_setup((uint64_t) table, (uint64_t) nr);
#endif
}
