/**
 * @file arch/arm/synquacer/irq.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


int handle_uart_interrupt(void)
{
  return IRQ_HANDLED;
}

int handle_dmac_interrupt(void)
{
  return IRQ_HANDLED;
}


asmlinkage int do_irq(unsigned long irq, struct full_regs *regs)
{
  printk("do_irq()\n");
  do_switch_thread();
  return 0;
}

