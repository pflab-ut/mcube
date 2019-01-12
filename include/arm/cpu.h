/**
 * @file include/arm/cpu.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_CPU_H__
#define __MCUBE_ARM_CPU_H__

#define USER_LEVEL 0
#define KERNEL_LEVEL 1
#define HYPERVISOR_LEVEL 2
#define TRUST_ZONE_LEVEL 3


#define REG_LENGTH 64

#ifdef __ASSEMBLY__

.macro get_cpu_id reg reg2
  mrs \reg2, mpidr_el1
  lsr \reg, \reg2, #8
  and \reg2, \reg2, #0xffffff
  and \reg, \reg, #0xff000000
  orr \reg, \reg, \reg2
.endm

.macro get_el
  mrs x0, CurrentEL
  lsr x0, x0, #2
  ret
.endm
   
#else
static inline unsigned long get_cpu_id(void)
{
  unsigned long reg;
  asm volatile("mrs %0, mpidr_el1" : "=r"(reg));
  reg = ((reg >> 8) & 0x00000000ff000000) | (reg & 0x0000000000ffffff);
  return reg;
}


static inline unsigned long get_el(void)
{
  unsigned long el;
  asm volatile("mrs %0, CurrentEL" : "=r"(el));
  return el >> 2;
}


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_ARM_CPU_H__ */
