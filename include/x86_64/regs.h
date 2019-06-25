/**
 * @file include/x86_64/regs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_REGS_H__
#define __MCUBE_X86_64_REGS_H__

#ifndef __ASSEMBLY__


/**
 * @struct full_regs
 * @brief Full registers.
 */
struct full_regs {
  /*
   * General purpose registers in X86_64.
   */

  /**
   * RAX.
   */
  uint64_t rax;

  /**
   * RBX.
   */
  uint64_t rbx;

  /**
   * RCX.
   */
  uint64_t rcx;

  /**
   * RDX.
   */
  uint64_t rdx;

  /**
   * RSI.
   */
  uint64_t rsi;

  /**
   * RDI.
   */
  uint64_t rdi;

  /**
   * RBP.
   */
  uint64_t rbp;

  /**
   * R8.
   */
  uint64_t r8;

  /**
   * R9.
   */
  uint64_t r9;

  /**
   * R10.
   */
  uint64_t r10;

  /**
   * R11.
   */
  uint64_t r11;

  /**
   * R12.
   */
  uint64_t r12;

  /**
   * R13.
   */
  uint64_t r13;

  /**
   * R14.
   */
  uint64_t r14;

  /**
   * R15.
   */
  uint64_t r15;
} __packed /** packed. */;


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_REGS_H__ */
