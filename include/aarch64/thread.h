/**
 * @file include/aarch64/thread.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_THREAD_H__
#define __MCUBE_AARCH64_THREAD_H__

#ifndef __ASSEMBLY__

/**
 * @struct cpu_context
 * @brief  CPU context information.
 */
struct cpu_context {
  /**
   * X19.
   */
  unsigned long x19;

  /**
   * X20.
   */
  unsigned long x20;

  /**
   * X21.
   */
  unsigned long x21;

  /**
   * X22.
   */
  unsigned long x22;

  /**
   * X23.
   */
  unsigned long x23;

  /**
   * X24.
   */
  unsigned long x24;

  /**
   * X25.
   */
  unsigned long x25;

  /**
   * X26.
   */
  unsigned long x26;

  /**
   * X27.
   */
  unsigned long x27;

  /**
   * X28.
   */
  unsigned long x28;

  /**
   * FP.
   */
  unsigned long fp;

  /**
   * SP.
   */
  unsigned long sp;

  /**
   * PC.
   */
  unsigned long pc;
};

/**
 * @def INIT_CPU_CONTEXT
 * @brief initialize CPU context.
 */
#define INIT_CPU_CONTEXT (struct cpu_context) {0}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_AARCH64_THREAD_H__ */
