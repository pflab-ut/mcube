/**
 * @file arch/x86/dsctbl.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

desc_ptr gdt, idt;
global_descriptor *gdt_start = (global_descriptor *) MEM_GDT;
interrupt_descriptor *idt_start = (interrupt_descriptor *) MEM_IDT;


void set_gdsc(struct global_descriptor *gd, uint32_t limit,
              uint32_t base, uint32_t ar)
{
	if (limit > 0xfffff) {
		ar |= 0x8000; /* G_bit = 1 */
		limit /= 0x1000;
	}
	gd->limit_low = limit & 0xffff;
	gd->base_low = base & 0xffff;
	gd->base_mid = (base >> 16) & 0xff;
	gd->access_right = ar & 0xff;
	gd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	gd->base_high = (base >> 24) & 0xff;
}

void set_idsc(struct interrupt_descriptor *id, uint64_t offset,
              uint32_t selector, uint32_t ar)
{
	id->offset_low = offset & 0xffff;
	id->selector = selector;
	id->dw_count = (ar >> 8) & 0xff;
	id->access_right = ar & 0xff;
	id->offset_high = (offset >> 16) & 0xffff;
}

void load_gdt(uint16_t size, uint32_t addr)
{
	gdt.size = size;
	gdt.addr = addr;
	lgdt(&gdt);
}



void load_idt(uint16_t size, uint32_t addr)
{
	idt.size = size;
	idt.addr = addr;
  lidt(&idt);
}



void init_dsctbl(void)
{
	int i;
	printk("init_dsctbl()\n");

	gdt_start = (global_descriptor *) MEM_GDT;
	idt_start = (interrupt_descriptor *) MEM_IDT;
	/* init GDT */
	for (i = 0; i < (MEM_GDT_END - MEM_GDT) / 8; i++) {
		set_gdsc(gdt_start + i, 0, 0, 0);
	}
	set_gdsc(gdt_start + 1,
           0xffffffff,
           0x00000000,
           AR_DATA32_RW);
	set_gdsc(gdt_start + 2,
           KMEM_KERNEL_IMAGE_END - KMEM_KERNEL_IMAGE - 1,
           KMEM_KERNEL_IMAGE, AR_CODE32_ER);
	load_gdt(MEM_GDT_END - MEM_GDT - 1, MEM_GDT);

	/* init IDT */
	for (i = 0; i < (MEM_IDT_END - MEM_IDT) / 8; i++) {
		set_idsc(idt_start + i, 0, 0, 0);
	}

	load_idt(MEM_IDT_END - MEM_IDT - 1, MEM_IDT);

	/* explicit scheduling event from user task */
  set_idsc(idt_start + SCHED_IRQ, (unsigned long) &common_interrupt, 2 * 8, AR_INTGATE32);

	/* register syscall */
  //	set_idsc(idt_start + SYSCALL_IRQ, (uint32_t) &system_call, 2 * 8, AR_INTGATE32);
}
