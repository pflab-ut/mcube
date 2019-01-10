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
 * @struct context_regs
 * @brief Context register
 */
struct context_regs {
	/** General purpose registers in ARM. */
	uint64_t gpr[32];
} __attribute__((packed));

typedef struct context_regs context_regs;


/**
 * @struct full_regs
 * @brief Full register
 */
struct full_regs {
	/** General purpose registers in ARM. */
	struct context_regs cregs;
  /** Exception link register */
  uint64_t elr;
  /** Saved program status register */
  uint64_t spsr;
} __attribute__((packed));

typedef struct full_regs full_regs;

void dump_registers(struct full_regs *regs);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_ARM_REGS_H__ */
