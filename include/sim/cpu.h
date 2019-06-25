/**
 * @file include/sim/cpu.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_CPU_H__
#define __MCUBE_SIM_CPU_H__

#define USER_LEVEL 0
#define KERNEL_LEVEL 3

#define REG_LENGTH 64


#ifndef __ASSEMBLY__


extern unsigned long current_cpu;

/**
 * @fn static inline unsigned long get_cpu_id(void)
 * @brief get CPU ID.
 *
 * @return CPU ID.
 */
static inline unsigned long get_cpu_id(void)
{
  return current_cpu;
}


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_CPU_H__ */
