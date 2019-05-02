/**
 * @file include/arm/regs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_REGS_H__
#define __MCUBE_ARM_REGS_H__

#ifndef __ASSEMBLY__


/**
 * @struct full_regs
 * @brief Full register
 */
struct full_regs {
  /** General purpose registers in ARM. */
  uint64_t gpr[32];
  /** Exception link register */
  uint64_t elr;
  /** Saved program status register */
  uint64_t spsr;
} __packed;

typedef struct full_regs full_regs;


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_ARM_REGS_H__ */
