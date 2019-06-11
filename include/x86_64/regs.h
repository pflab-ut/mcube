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
  /** General purpose registers in X86_64. */
  uint64_t rax;
  uint64_t rbx;
  uint64_t rcx;
  uint64_t rdx;
  uint64_t rsi;
  uint64_t rdi;
  uint64_t rbp;
  uint64_t r8;
  uint64_t r9;
  uint64_t r10;
  uint64_t r11;
  uint64_t r12;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;
} __packed;


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_REGS_H__ */
