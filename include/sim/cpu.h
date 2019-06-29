/**
 * @file include/sim/cpu.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_CPU_H__
#define __MCUBE_SIM_CPU_H__

/**
 * @def USER_LEVEL
 * @brief User level.
 */
#define USER_LEVEL 0

/**
 * @def KERNEL_LEVEL
 * @brief Kernel level.
 */
#define KERNEL_LEVEL 3

/**
 * @def REG_LENGTH
 * @brief Register length.
 */
#define REG_LENGTH 64


#ifndef __ASSEMBLY__


/**
 * @var current_cpu
 * @brief Current CPU.
 */
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
