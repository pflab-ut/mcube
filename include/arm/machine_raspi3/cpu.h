/**
 * @file include/arm/machine_raspi3/cpu.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_ARM_MACHINE_RASPI3_CPU_H__
#define	__MCUBE_ARM_MACHINE_RASPI3_CPU_H__

#ifndef __ASSEMBLY__

static inline int get_nr_cpu_cores(void)
{
  return 4;
}

#endif /* !__ASSEMBLY__ */

#endif /*	__MCUBE_ARM_MACHINE_RASPI3_CPU_H__ */

