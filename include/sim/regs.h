/**
 * @file include/sim/regs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_REGS_H__
#define __MCUBE_SIM_REGS_H__

#ifndef __ASSEMBLY__


/**
 * @struct context_regs
 * @brief Context registers.
 */
struct context_regs {
  /** Dummy register in SIM. */
  unsigned long r_dummy[0];
};

typedef struct context_regs context_regs;

/**
 * @struct full_regs
 * @brief Full registers.
 */
struct full_regs {
  /** Dummy register in SIM. */
  unsigned long r_dummy[0];
};

typedef struct full_regs full_regs;


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_REGS_H__ */
