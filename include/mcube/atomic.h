/**
 * @file include/mcube/types.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_ATOMIC_H__
#define	__MCUBE_MCUBE_ATOMIC_H__

#ifndef __ASSEMBLY__

/**
 * @brief Atomic variable
 *
 * The atomic structure has atomic information.
 */
struct atomic {
	/** Counter. */
  int counter;
};

typedef struct atomic atomic;

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_ATOMIC_H__ */
