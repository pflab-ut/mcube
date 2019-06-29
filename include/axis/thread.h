/**
 * @file include/axis/thread.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_THREAD_H__
#define __MCUBE_AXIS_THREAD_H__

/**
 * @def STACK_FRAME_SIZE
 * @brief Stack frame size for interrupt enter/exit.
 */
#define STACK_FRAME_SIZE 128

/**
 * @def CPU_CONTEXT_STACK_FRAME_SIZE
 * @brief CPU context's stack frame size for context switch.
 */
#define CPU_CONTEXT_STACK_FRAME_SIZE 56

#ifndef __ASSEMBLY__

/**
 * @struct cpu_context
 * @brief CPU context information.
 */
struct cpu_context {
  /**
   * S0.
   */
  unsigned long s0;

  /**
   * S1.
   */
  unsigned long s1;

  /**
   * S2.
   */
  unsigned long s2;

  /**
   * S3.
   */
  unsigned long s3;

  /**
   * S4.
   */
  unsigned long s4;

  /**
   * S5.
   */
  unsigned long s5;

  /**
   * S6.
   */
  unsigned long s6;

  /**
   * S7.
   */
  unsigned long s7;

  /**
   * K0.
   */
  unsigned long k0;

  /**
   * K1.
   */
  unsigned long k1;

  /**
   * GP.
   */
  unsigned long gp;

  /**
   * SP.
   */
  unsigned long sp;

  /**
   * FP.
   */
  unsigned long fp;

  /**
   * RA.
   */
  unsigned long ra;
};

/**
 * @def INIT_CPU_CONTEXT
 * @brief initialize CPU context.
 */
#define INIT_CPU_CONTEXT (struct cpu_context) {0}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_AXIS_THREAD_H__ */
