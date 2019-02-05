/**
 * @file include/arm/atomic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_ATOMIC_H__
#define __MCUBE_ARM_ATOMIC_H__

#ifndef __ASSEMBLY__

#define LOCK_PREFIX_HERE

#define LOCK_PREFIX LOCK_PREFIX_HERE

static inline void compare_and_swap(volatile uint64_t *ptr, uint64_t new, uint64_t old)
{
  uint64_t tmp;
#if 1
  uint64_t oldval;
  //  asm volatile("prfm    pstl1strm, %0" :: "r"(&v->counter));
  asm volatile("1: ldaxr %w0, [%1]" : "=&r"(oldval) : "r"(ptr));
  asm volatile("eor %w0, %w1, %w2" : "=r"(tmp) : "r"(oldval), "r"(old));
  asm volatile("cbnz %w0, 2f" :: "r"(tmp));
  asm volatile("stxr %w0, %w2, [%1]" : "=&r"(tmp) : "r"(ptr), "r"(new));
  asm volatile("cbnz %w0, 1b" :: "r"(tmp));
  asm volatile("2:");
#else  
  /* Large System Extension (LSE) in ARMv8.1 */
  asm volatile("mov %w0, %w1" : "=r"(tmp) : "r"(old));
  asm volatile("cas %w0, %w1, [%2]" : "=r"(tmp), "=r"(new), "=r"(*ptr));
#endif
}


static inline void atomic_inc(uint64_t *val)
{
  uint64_t new, old;
  new = *val + 1;
  old = *val;
  compare_and_swap(val, new, old);
}

static inline void atomic_dec(uint64_t *val)
{
  uint64_t new, old;
  new = *val - 1;
  old = *val;
  compare_and_swap(val, new, old);
}

static inline uint64_t fetch_and_inc(uint64_t *val)
{
  uint64_t new, old;
  new = *val + 1;
  old = *val;
  compare_and_swap(val, new, old);
  return *val;
}

static inline uint64_t fetch_and_dec(uint64_t *val)
{
  uint64_t new, old;
  new = *val - 1;
  old = *val;
  compare_and_swap(val, new, old);
  return *val;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_ARM_ATOMIC_H__ */
