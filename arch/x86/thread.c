/**
 * @file arch/x86/thread.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void set_initial_context(struct thread_struct *th,
                         void (*pc)(void *),
                         void *func,
                         unsigned long bottom)
{
#if 0  
	struct full_regs *fregs;
	th->stack_top = bottom;
	fregs = get_context_top(th);

	//	printk("bottom = %x\n", bottom);

	fregs->cregs.rip = (uint64_t) pc;
	fregs->cregs.rflags = INIT_EFLAGS;
	fregs->cregs.rax = (uint64_t) func;
	fregs->cregs.rcx = 0;
	fregs->cregs.rdx = 0;
	fregs->cregs.rbx = 0;
	fregs->cregs.rsp = (uint64_t) &fregs->cregs;
	fregs->cregs.rbp = 0;
	fregs->cregs.rsi = 0;
	fregs->cregs.rdi = 0;
	fregs->cregs.res = 1 * 8;
	fregs->cregs.rcs = 2 * 8;
	fregs->cregs.rss = 1 * 8;
	fregs->cregs.rds = 1 * 8;
	fregs->cregs.rfs = 1 * 8;
	fregs->cregs.rgs = 1 * 8;
	fregs->cregs.ldtr = 0;
	fregs->cregs.iomap = IOMAP_START;

	//	printk("&fregs->cregs.eip = %x fregs->cregs.eip = %x\n", &fregs->cregs.eip, fregs->cregs.eip);
	set_gdsc(gdt_start + TASK_GDT0 + th->id, sizeof(struct context_regs) - 1,
					 (uint64_t) &fregs->cregs, AR_TSS32);
#endif
}





void __attribute__((noreturn)) run_user_thread(void)
{
  size_t cpu = get_cpu_id();
  void *ret = NULL;
  printk("run_user_thread()\n");
  if (current_th[cpu]->run_user_func) {
    ret = (*current_th[cpu]->run_user_func)(current_th[cpu]->arg);
  }
  if (ret) {
    printk("ret = 0x%lx\n", (unsigned long) ret);
  }
  halt();
  inf_loop();
}

void wait(unsigned long count)
{
  while (get_current_cpu_time() < count)
    ;
}


void __attribute__((noreturn)) exit(int status)
{
	cli();
	halt();
	inf_loop();
}
