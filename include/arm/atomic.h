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

static inline void compare_and_swap(volatile int *ptr, int new, int old)
{
  int tmp;
#if 1
  int oldval;
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


static inline void atomic_inc(atomic_t *v)
{
  int new, old;
  new = v->counter + 1;
  old = v->counter;
  compare_and_swap(&v->counter, new, old);
}

static inline void atomic_dec(atomic_t *v)
{
  int new, old;
  new = v->counter - 1;
  old = v->counter;
  compare_and_swap(&v->counter, new, old);
}

static inline int fetch_and_inc(int *i)
{
  int new, old;
  new = *i + 1;
  old = *i;
  compare_and_swap(i, new, old);
  return *i;
}

static inline int fetch_and_dec(int *i)
{
  int new, old;
  new = *i - 1;
  old = *i;
  compare_and_swap(i, new, old);
  return *i;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_ARM_ATOMIC_H__ */
