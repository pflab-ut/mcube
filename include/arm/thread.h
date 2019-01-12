/**
 * @file include/arm/thread.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_THREAD_H__
#define __MCUBE_ARM_THREAD_H__

#ifndef __ASSEMBLY__

/**
 * @struct cpu_context
 * @brief  CPU context information.
 */
struct cpu_context {
  unsigned long x19;
  unsigned long x20;
  unsigned long x21;
  unsigned long x22;
  unsigned long x23;
  unsigned long x24;
  unsigned long x25;
  unsigned long x26;
  unsigned long x27;
  unsigned long x28;
  unsigned long fp;
  unsigned long sp;
  unsigned long pc;
};


#define INIT_CPU_CONTEXT (struct cpu_context) {0}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_ARM_THREAD_H__ */
