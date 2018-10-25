/**
 * @file arch/x86/irq.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void wait_until_next_interrupt(void)
{
  halt();
}

void do_switch_thread_arch(interrupt_context_t *context)
{
	unsigned long cpu = get_cpu_id();
  if (current_th[cpu] != prev_th[cpu]) {
    prev_th[cpu]->rsp = context->rsp;
    prev_th[cpu]->interrupt_program_counter = context->retaddr;
    prev_th[cpu]->context = context;
    asm volatile("mov %0, rsp" : "=r"(prev_th[cpu]->current_sp));  
    if (!(current_th[cpu]->thflags & THFLAGS_START_TH)) {
      /* start thread */
      current_th[cpu]->thflags |= THFLAGS_START_TH;
      context->retaddr = (uint64_t) run_user_thread;
      context->rsp = (uint64_t) get_context_top(current_th[cpu]);
    } else {
      /* resume thread */
      asm volatile("mov rsp, %0" :: "r"(current_th[cpu]->current_sp));
      context = current_th[cpu]->context;
      context->retaddr = current_th[cpu]->interrupt_program_counter;
      context->rsp = current_th[cpu]->rsp;
      //      print("context->retaddr = 0x%lx rsp = 0x%lx\n", context->retaddr, context->rsp);
      //      inf_loop();
    }
    prev_th[cpu] = current_th[cpu];
  }
}



void init_irq(void)
{
  extern struct desc_ptr IDT_Pointer;
  init_irq_asm();
  print("IDT_Pointer = 0x%lx\n", (unsigned long) &IDT_Pointer);
  print("IDT_Pointer.size = 0x%x\n", IDT_Pointer.size);
  print("IDT_Pointer.addr = 0x%lx\n", IDT_Pointer.addr);
  //  lidt(&IDT_Pointer);
  asm volatile("lidt [%0]" :: "r"(&IDT_Pointer));
}

