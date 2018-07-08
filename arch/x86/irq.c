/**
 * @file arch/x86/irq.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>



asmlinkage int do_syscall(int number)
{
	printk("number = %d\n", number);
	return 0;
}


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
		asm volatile("movq %%rsp, %0" : "=r"(prev_fr->cregs.rsp));
		current_fr = get_context_top(current_th[cpu]);
		//		printk("do_switch_thread(): prev_th[%lu] = 0x%x\n", cpu, prev_th[cpu]);
		//		printk("do_switch_thread(): current_th[%lu] = 0x%x\n", cpu, current_th[cpu]);
		//		printk("do_switch_thread(): current_fr->rsp = %x current_fr = %x\n", current_fr->rsp, current_fr);
		if (!(current_th[cpu]->thflags & THFLAGS_START_TH)) {
			//			printk("current_th[0]->thflags = %x\n", current_th[cpu]->thflags);
			current_th[cpu]->thflags |= THFLAGS_START_TH;

			printk("do_switch_thread(): current_th[%lu] = 0x%lx\n", cpu, (unsigned long) current_th[cpu]);
		
			/* subtract first argument from rsp */
			asm volatile("movq %0, %%rsp" :: "r"(current_fr->cregs.rsp-4));
			/* set first argument to current thid */
			asm volatile("movq %0, 8(%%rsp)" :: "r"((uint64_t) current_th[cpu]->id));
			//			asm volatile("push %0" :: "r"((uint64_t) current_th[cpu]->id));
			asm volatile("pushf"); /* EFLAGS */
      //			asm volatile("push %cs");
			asm volatile("push %0" :: "r"(run_user_thread));
      //      asm volatile("movq $0x0,LAPIC_EOI");
      mmio_out64(LAPIC_EOI, 0);
      asm volatile("sti");
			asm volatile("iret");
		}

		asm volatile("movq %0, %%rsp" :: "r"(current_fr->cregs.rsp));
	}

	//	printk("terminate optional part\n");
	//	printk("current_th[%lu]->id = %lu thflags = %x\n", cpu, current_th[cpu]->id, current_th[cpu]->thflags);
	/* terminate optional part */
	if (current_th[cpu]->thflags & THFLAGS_TERM_OPTIONAL) {
		current_th[cpu]->thflags &= ~THFLAGS_TERM_OPTIONAL;
		current_fr = get_context_top(current_th[cpu]);

		asm volatile("movq %0, %%rsp" :: "r"(current_fr));
		/* set first argument to current thid */
		asm volatile("movq %0, 8(%%rsp)" :: "r"((uint64_t) current_th[cpu]->id));
		asm volatile("pushf"); /* EFLAGS */
    //		asm volatile("push %cs");
		asm volatile("push %0" :: "r"(run_user_thread));
    //    asm volatile("movq $0x0,LAPIC_EOI");
    mmio_out64(LAPIC_EOI, 0);
    asm volatile("sti");
  	asm volatile("iret");
	}
}

asmlinkage int do_IRQ(unsigned long irq, struct full_regs *regs)
{
	unsigned long cpu = get_cpu_id();
  //	uint64_t rsp;
	printk("do_IRQ(): cpu = %lu sys_jiffies = %lu\n", cpu, sys_jiffies);
	end_budget(current_th[cpu]);

	//	inf_loop();
	//	asm volatile("movl %%rsp, %0" :: "r"(rsp));
	//	printk("do_IRQ(): cpu = %lu start rsp = 0x%x\n", cpu, rsp);
	//	printk("rdtscp = %lx\n", rdtscp());
	/* just in case */
#if 1
	if (sched_end == TRUE) {
		printk("sched_end == TRUE\n");
		return 0;
	}
#endif
	//	printk("do_IRQ(): smp_barrier(1): cpu = %lu\n", cpu);
  //	smp_barrier(1);
	//	pdebug_thread(current_th[cpu]);
	//	printk("current_th[%lu] = 0x%x id = %lu\n", cpu, current_th[cpu], current_th[cpu]->id);
	prev_th[cpu] = current_th[cpu];
	printk("do_IRQ(): cpu = %lu irq = 0x%lx\n", cpu, irq);
	/* dummy interrupt for one-shot timer */
	if (irq == SCHED_IRQ) {
		goto __do_IRQ;
	}

	/* LAPIC */
	/* overwrite irq */
	if ((irq = find_lapic_first_bit()) != NR_IRQS) {
		//		printk("do_IRQ(): LAPIC irq = 0x%x\n", irq);
		goto __do_IRQ;
	}

#if 0
	/* master 8259 */
	/* set ISR */
	outb(IS_OCW3 | OCW3_ISR, PIC0_OCW3);
	/* read ISR */
	irq = inb(PIC0_ISR);
	if (irq) {
		//		printk("PIC0 irq = %x\n", irq);
		/* set IRQ bit */
		irq = bsf(irq);
		//		printk("PIC0 bsf(irq) = %x\n", irq);
		if (irq != SLAVE_IRQ) {
			goto __do_IRQ;
		}
	}
	
	/* slave 8259 */
	/* set ISR */
	outb(IS_OCW3 | OCW3_ISR, PIC1_OCW3);
	/* read ISR */
	irq = inb(PIC1_ISR);
	if (irq) {
		//		printk("PIC1 irq = %x\n", irq);
		/* set IRQ bit + # of Master IRQs */
		irq = bsf(irq) + NR_MASTER_IRQS;
		//		printk("PIC1 bsf(irq) = %x\n", irq);
		goto __do_IRQ;
	}
#endif


	printk("%s:%s():%d: unknown IRQ 0x%lx\n", __FILE__, __func__, __LINE__, irq);
	goto error;


 __do_IRQ:

	//	printk("do_IRQ(): cpu = %d irq = %x\n", cpu, irq);
	//	return 0;
	__do_IRQ(irq);

  //	smp_barrier(2);
	if (cpu == 0) {
		//		print_overhead();
	}
	printk("do_IRQ(): current_th[%lu]->id = %lu\n", cpu, current_th[cpu]->id);
  //	smp_barrier(3);


	begin_budget(current_th[cpu]);

	//	pdebug_thread(current_th[cpu]);
	//	printk("do_IRQ(): current_th[%d] = 0x%x\n", cpu, current_th[cpu]);
	do_switch_thread();
	//	printk("LAPIC_CUR_COUNT = %x\n", mmio_in64(LAPIC_CUR_COUNT));
	//	asm volatile("movl %%rsp, %0" :: "r"(rsp));
	//	printk("do_IRQ(): cpu = %d end rsp = 0x%x\n", cpu, rsp);
	//	pdebug_thread(current_th[cpu]);
	return 0;
 error:
	inf_loop();
  /* not reached */
  return -1;
}


void init_IRQ(void)
{
  printk("init_IRQ()\n");
	/* disable PIC0 interrupt */
	outb(OCW1_DISABLE_ALL_IRQS, PIC0_IMR);
	/* disable PIC1 interrupt */
	outb(OCW1_DISABLE_ALL_IRQS, PIC1_IMR);

	/* edge trigger mode */
	outb(ICW1_EDGE_TRIGGER, PIC0_ICW1);
	/* handle IRQ0-7 to INT20-27 */
	outb(IRQ_OFFSET, PIC0_ICW2);
	/* connect IRQ2 to PIC1 */
	outb(0x1 << SLAVE_IRQ, PIC0_ICW3);
	/* non buffer mode */
	outb(ICW4_NON_BUFFER, PIC0_ICW4);

	/* edge trigger mode */
	outb(ICW1_EDGE_TRIGGER, PIC1_ICW1);
	/* handle IRQ8-15 to INT28-2f */
	outb(IRQ_OFFSET + NR_MASTER_IRQS, PIC1_ICW2);
	/* connect IRQ2 to PIC1 */
	outb(SLAVE_IRQ, PIC1_ICW3);
	/* non buffer mode */
	outb(ICW4_NON_BUFFER, PIC1_ICW4);
	
	/* disable all interrupts in PIC0 */
	outb(OCW1_DISABLE_ALL_IRQS, PIC0_IMR);
	/* disable all interrupts in PIC1 */
	outb(OCW1_DISABLE_ALL_IRQS, PIC1_IMR);
}
