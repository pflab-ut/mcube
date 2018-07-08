/**
 * @file include/arm/regs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_REGS_H__
#define __MCUBE_ARM_REGS_H__

#ifndef __ASSEMBLY__


#define	SEPARATE_SIZE	0

#define	STACK_ALIGN(x)	(((x) + 7) & -8)

/**
 * @brief Context register
 *
 * The context_regs structure has context register information.
 */
struct context_regs {
	/** General purpose registers in ARM. */
	uint64_t gpr[16];
};

typedef struct context_regs context_regs;


/**
 * @brief Full register
 *
 * The full_regs structure has full register information.
 */
struct full_regs {
	/** General purpose registers in ARM. */
	struct context_regs cregs;
};

typedef struct full_regs full_regs;


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_ARM_REGS_H__ */
