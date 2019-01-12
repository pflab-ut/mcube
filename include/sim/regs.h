/**
 * @file include/sim/regs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_REGS_H__
#define __MCUBE_SIM_REGS_H__

#ifndef __ASSEMBLY__

#define SEPARATE_SIZE  0

#define STACK_ALIGN(x)  (((x) + 7) & -8)

/**
 * @struct context_regs
 * @brief Context register
 */
struct context_regs {
  /** Dummy register in SIM. */
  unsigned long r_dummy[0];
};

typedef struct context_regs context_regs;

/**
 * @struct full_regs
 * @brief Full register
 */
struct full_regs {
  /** Dummy register in SIM. */
  unsigned long r_dummy[0];
};

typedef struct full_regs full_regs;


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_REGS_H__ */
