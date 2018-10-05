/**
 * @file include/axis/thread.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_AXIS_THREAD_H__
#define	__MCUBE_AXIS_THREAD_H__

/** Stack frame size for interrupt enter/exit. */
#define STACK_FRAME_SIZE 128 

/* CPU context's stack frame size for context switch. */
#define CPU_CONTEXT_STACK_FRAME_SIZE 56

#ifndef __ASSEMBLY__

/**
 * @struct cpu_context
 * @brief  CPU context information.
 */
struct cpu_context {
  unsigned long s0;
  unsigned long s1;
  unsigned long s2;
  unsigned long s3;
  unsigned long s4;
  unsigned long s5;
  unsigned long s6;
  unsigned long s7;
  unsigned long k0;
  unsigned long k1;
  unsigned long gp;
  unsigned long sp;
  unsigned long fp;
  unsigned long ra;
};

#define INIT_CPU_CONTEXT (struct cpu_context) {0}

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_AXIS_THREAD_H__ */
