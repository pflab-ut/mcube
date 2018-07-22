/**
 * @file arch/x86/irq.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>



void do_switch_thread(void)
{
  struct full_regs *prev_fr, *current_fr = NULL;
	unsigned long cpu = get_cpu_id();

	//	printk("current_th[%lu] = 0x%x id = %lu\n", cpu, current_th[cpu], current_th[cpu]->id);
	//	pdebug_jiffies();
	//	pdebug_bitmap(run_tq[0].bitmap);
	//	pdebug_array(run_tq[0].array);

	if (prev_th[cpu] != current_th[cpu]) {
		prev_fr = get_context_top(prev_th[cpu]);
    //		asm volatile("movq %%rsp, %0" : "=r"(prev_fr->cregs.rsp));
		asm volatile("mov %0, rsp" : "=r"(prev_fr->cregs.rsp));
		current_fr = get_context_top(current_th[cpu]);
		//		printk("do_switch_thread(): prev_th[%lu] = 0x%x\n", cpu, prev_th[cpu]);
		//		printk("do_switch_thread(): current_th[%lu] = 0x%x\n", cpu, current_th[cpu]);
		//		printk("do_switch_thread(): current_fr->rsp = %x current_fr = %x\n", current_fr->rsp, current_fr);
		if (!(current_th[cpu]->thflags & THFLAGS_START_TH)) {
			//			printk("current_th[0]->thflags = %x\n", current_th[cpu]->thflags);
			current_th[cpu]->thflags |= THFLAGS_START_TH;

			printk("do_switch_thread(): current_th[%lu] = 0x%lx\n", cpu, (unsigned long) current_th[cpu]);
		
			/* subtract first argument from rsp */
      //			asm volatile("movq %0, %%rsp" :: "r"(current_fr->cregs.rsp-4));
			asm volatile("mov rsp, [%0]" :: "r"(current_fr->cregs.rsp-4));
			/* set first argument to current thid */
      //			asm volatile("movq %0, 8(%%rsp)" :: "r"((uint64_t) current_th[cpu]->id));
			asm volatile("mov rsp, [%0]" :: "r"((uint64_t) current_th[cpu]->id + 8));
			//			asm volatile("push %0" :: "r"((uint64_t) current_th[cpu]->id));
			asm volatile("pushf"); /* EFLAGS */
      //			asm volatile("push %cs");
			asm volatile("push %0" :: "r"(run_user_thread));
      //      asm volatile("movq $0x0,LAPIC_EOI");
      mmio_out64(LAPIC_EOI, 0);
      asm volatile("sti");
			asm volatile("iret");
		}
    
    //		asm volatile("movq %0, %%rsp" :: "r"(current_fr->cregs.rsp));
    //		asm volatile("mov rsp, %[addr]" :: [addr] "r"(current_fr->cregs.rsp));
		asm volatile("mov rsp, %0" :: "r"(current_fr->cregs.rsp));
	}
	//	printk("terminate optional part\n");
	//	printk("current_th[%lu]->id = %lu thflags = %x\n", cpu, current_th[cpu]->id, current_th[cpu]->thflags);
	/* terminate optional part */
	if (current_th[cpu]->thflags & THFLAGS_TERM_OPTIONAL) {
		current_th[cpu]->thflags &= ~THFLAGS_TERM_OPTIONAL;
		current_fr = get_context_top(current_th[cpu]);

    //		asm volatile("movq %0, %%rsp" :: "r"(current_fr));
		asm volatile("mov rsp, %0" :: "r"(current_fr));
		/* set first argument to current thid */
    //		asm volatile("movq %0, 8(%%rsp)" :: "r"((uint64_t) current_th[cpu]->id));
		asm volatile("mov [rsp+8], %0" :: "r"((uint64_t) current_th[cpu]->id));
		asm volatile("pushf"); /* EFLAGS */
    //		asm volatile("push %cs");
		asm volatile("push %0" :: "r"(run_user_thread));
    //    asm volatile("movq $0x0,LAPIC_EOI");
    mmio_out64(LAPIC_EOI, 0);
    asm volatile("sti");
  	asm volatile("iret");
	}
}


void init_irq(void)
{
  extern struct desc_ptr IDT_Pointer;
  init_irq_asm();
  printk("IDT_Pointer = 0x%lx\n", (unsigned long) &IDT_Pointer);
  printk("IDT_Pointer.size = 0x%x\n", IDT_Pointer.size);
  printk("IDT_Pointer.addr = 0x%lx\n", IDT_Pointer.addr);
  //  lidt(&IDT_Pointer);
  asm volatile("lidt [%0]" :: "r"(&IDT_Pointer));
}

