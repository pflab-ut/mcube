/**
 * @file include/aarch64/regs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_REGS_H__
#define __MCUBE_AARCH64_REGS_H__

#ifndef __ASSEMBLY__

/**
 * @struct full_regs
 * @brief Full registers.
 */
struct full_regs {
  /** General purpose registers in AARCH64. */
  uint64_t gpr[32];
  /** Exception link register */
  uint64_t elr;
  /** Saved program status register */
  uint64_t spsr;
} __packed;


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_AARCH64_REGS_H__ */
