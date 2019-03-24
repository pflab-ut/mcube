/**
 * @file arch/arm/dump.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void dump_registers(struct full_regs *regs)
{
  int i;

  for (i = 0; i < 32; i += 2) {
    printk("x%02d: 0x%016lx  x%02d: 0x%016lx\n",
           i, regs->cregs.gpr[i], i + 1, regs->cregs.gpr[i + 1]);
  }
}

