/**
 * @file include/x86_64/thread.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_X86_64_THREAD_H__
#define	__MCUBE_X86_64_THREAD_H__

#ifndef __ASSEMBLY__

/**
 * @struct cpu_context
 * @brief CPU context information.
 */
struct cpu_context {
  /**
   * RAX.
   */
  unsigned long rax;

  /**
   * RBX.
   */
  unsigned long rbx;

  /**
   * RCX.
   */
  unsigned long rcx;

  /**
   * RDX.
   */
  unsigned long rdx;
};

/**
 * @def INIT_CPU_CONTEXT
 * @brief initialize CPU context.
 */
#define INIT_CPU_CONTEXT (struct cpu_context) {0}

#endif /* __ASSEMBLY__ */


#endif	/* __MCUBE_X86_64_THREAD_H__ */
