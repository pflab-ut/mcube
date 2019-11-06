/**
 * @file arch/axis/dump.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void dump_registers(struct full_regs *regs)
{
  /* print general purpose registers. */
  for (int i = 0; i < NR_GENERAL_PURPOSE_REGS; i += 2) {
    printk("r%02d: 0x%016lx  r%02d: 0x%016lx\n",
           i, regs->gpr_regs.regs[i], i + 1, regs->gpr_regs.regs[i + 1]);
  }
}
