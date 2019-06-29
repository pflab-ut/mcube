/**
 * @file include/sim/ops.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_OPS_H__
#define __MCUBE_SIM_OPS_H__

#ifndef __ASSEMBLY__


/**
 * @fn static inline uint64_t rdtsc(void)
 * @brief read time stamp counter.
 *
 * @return Time stamp counter.
 */
static inline uint64_t rdtsc(void)
{
  uint64_t x;

  asm volatile("cpuid" : : : "rax", "rbx", "rcx", "rdx");
  asm volatile("rdtsc\n\t"
               "shlq       $32, %%rdx\n\t"
               "orq        %%rdx, %%rax\n\t"
               "movq       %%rax, %0" : "=g"(x) : : "rax", "rdx");

  return x;
}

/**
 * @fn static inline uint64_t rdtscp(void)
 * @brief read time stamp counter and processor ID.
 *
 * @return Time stamp counter and processor ID.
 */
static inline uint64_t rdtscp(void)
{
  uint32_t high, low;
  asm volatile("rdtscp" : "=a"(low), "=d"(high));
  return (uint64_t) low | ((uint64_t) high << 32);
}

/**
 * @def halt()
 * @brief Wrapper definition for infinite @c hlt.
 */
#define halt()                                  \
  do {                                          \
    printk("halt\n");                           \
    inf_loop();                                 \
  } while (0);


/**
 * @fn static inline void nop(void)
 * @brief Wrapper function for @c nop.
 */
static inline void nop(void)
{
  asm volatile("nop");
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_OPS_H__ */
