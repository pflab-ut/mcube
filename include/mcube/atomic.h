/**
 * @file include/mcube/types.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_ATOMIC_H__
#define	__MCUBE_MCUBE_ATOMIC_H__

#ifndef __ASSEMBLY__

#if CONFIG_ARCH_SIM || CONFIG_ARCH_X86
#include <stdatomic.h>
#else
typedef int atomic_int;
#endif /* CONFIG_ARCH_SIM || CONFIG_ARCH_X86 */



/**
 * @struct atomic
 * @brief Atomic variable
 */
typedef struct atomic {
	/** Counter. */
  int counter;
} atomic_t;


#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_ATOMIC_H__ */
