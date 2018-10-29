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


int call_sys_write(char *buf)
{
  return puts(buf);
}

int call_sys_get_mode_level(void)
{
  int ret;
  syscall1(SYS_get_mode_level, (unsigned long) &ret);
  return ret;
}

int call_user_sys_write(char *buf)
{
  int ret;
  //  syscall1(SYS_write, (unsigned long) buf);
  printk("&ret = 0x%lx\n", (unsigned long) &ret);
  syscall2(SYS_write, (unsigned long) &ret, (unsigned long) buf);
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
  
  //  unsigned long rcx;
  //asm volatile("movq %0, rcx" : "=r"(rcx));
  // Do nothing for now.
  //  printk("handle_syscall()\n");
  //  dump_registers(&context->regs);

  switch (context->regs.rdi) {
  case SYS_sched:
    break;
  case SYS_end_job:
    break;
  case SYS_get_exec_time:
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
}

void init_syscall(void)
{
#if 1
  set_isr(SYSCALL_IRQ, handle_syscall);
#else
  // Request the CPU's extended features.
  registers4_t regs4;
  cpuid(0x80000001, &regs4);

  // Bit 11 of rdx tells us if the SYSCALL/SYSRET instructions are
  // available. If they're not, raise an invalid opcode exception.
  if (!(regs4.edx & (1 << 11))) {
    invalid_opcode();
  }

  // Update the MSR_STAR with the segment selectors that will be used
  // by SYSCALL and SYSRET.
  uint64_t star = rdmsr(MSR_STAR);
  star &= 0x00000000ffffffff;
  star |= (uint64_t) GDT64_SEGMENT_SELECTOR_KERNEL_CODE << 32;
  star |= (uint64_t)((GDT64_SEGMENT_SELECTOR_USER_CODE - 16) | 3) << 48;
  wrmsr(MSR_STAR, star);
  //  printk("star = %x %x\n", star >> 32, star & 0xffffffff);
  // Write the address of the system call handler used by SYSCALL.
  wrmsr(MSR_LSTAR, (uint64_t) handle_syscall);

  // Write the CPU flag mask used during SYSCALL.
  wrmsr(MSR_SYSCALL_MASK, 0);

#endif
}
