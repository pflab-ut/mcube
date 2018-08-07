/**
 * @file arch/x86/irq.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>



void do_switch_thread(void)
{
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

