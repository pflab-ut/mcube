/**
 * @file include/arm/cpu.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_ARM_CPU_H__
#define	__MCUBE_ARM_CPU_H__

#define REG_LENGTH 64

#ifdef __ASSEMBLY__

.macro get_cpu_id
  mrs x1, mpidr_el1
  lsr x0, x1, #8
  and x1, x1, #0xffffff
  and x0, x0, #0xff000000
  orr x0, x0, x1
.endm
     
#else
static inline unsigned long get_cpu_id(void)
{
  unsigned long reg;
  asm volatile("mrs %0, mpidr_el1" : "=r"(reg));
  reg = ((reg >> 8) & 0x00000000ff000000) | (reg & 0x0000000000ffffff);
  return reg;
}


int get_el(void);

#endif /* !__ASSEMBLY__ */


#endif	/* __MCUBE_ARM_CPU_H__ */
