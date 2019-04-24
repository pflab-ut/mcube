/**
 * @file include/sim/thread.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_THREAD_H__
#define __MCUBE_SIM_THREAD_H__

#ifndef __ASSEMBLY__

/**
 * @struct cpu_context
 * @brief  CPU context information.
 */
struct cpu_context {
  /* dummy */
  unsigned long eax;
  unsigned long ebx;
  unsigned long ecx;
  unsigned long edx;
};

#define INIT_CPU_CONTEXT (struct cpu_context) {0}



#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_SIM_THREAD_H__ */
