/**
 * @file arch/arm/dump.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void dump_registers(struct full_regs *regs)
{
  printk("x0:  0x%016lx  x1:  0x%016lx\n", regs->cregs.gpr[0], regs->cregs.gpr[1]);
  printk("x2:  0x%016lx  x3:  0x%016lx\n", regs->cregs.gpr[2], regs->cregs.gpr[3]);
  printk("x4:  0x%016lx  x5:  0x%016lx\n", regs->cregs.gpr[4], regs->cregs.gpr[5]);
  printk("x6:  0x%016lx  x7:  0x%016lx\n", regs->cregs.gpr[6], regs->cregs.gpr[7]);
  printk("x8:  0x%016lx  x9:  0x%016lx\n", regs->cregs.gpr[8], regs->cregs.gpr[9]);
  printk("x10: 0x%016lx  x11: 0x%016lx\n", regs->cregs.gpr[10], regs->cregs.gpr[11]);
  printk("x12: 0x%016lx  x13: 0x%016lx\n", regs->cregs.gpr[12], regs->cregs.gpr[13]);
  printk("x14: 0x%016lx  x15: 0x%016lx\n", regs->cregs.gpr[14], regs->cregs.gpr[15]);
  printk("x16: 0x%016lx  x17: 0x%016lx\n", regs->cregs.gpr[16], regs->cregs.gpr[17]);
  printk("x18: 0x%016lx  x19: 0x%016lx\n", regs->cregs.gpr[18], regs->cregs.gpr[19]);
  printk("x20: 0x%016lx  x21: 0x%016lx\n", regs->cregs.gpr[20], regs->cregs.gpr[21]);
  printk("x22: 0x%016lx  x23: 0x%016lx\n", regs->cregs.gpr[22], regs->cregs.gpr[23]);
  printk("x24: 0x%016lx  x25: 0x%016lx\n", regs->cregs.gpr[24], regs->cregs.gpr[25]);
  printk("x26: 0x%016lx  x27: 0x%016lx\n", regs->cregs.gpr[26], regs->cregs.gpr[27]);
  printk("x28: 0x%016lx  x29: 0x%016lx\n", regs->cregs.gpr[28], regs->cregs.gpr[29]);
  printk("x30: 0x%016lx  x31: 0x%016lx\n", regs->cregs.gpr[30], regs->cregs.gpr[31]);
}


