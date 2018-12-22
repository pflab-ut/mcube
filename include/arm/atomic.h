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

static inline void atomic_inc(atomic_t *v)
{
  unsigned long tmp, old, new;
  old = v->counter;
  new = v->counter + 1;
#if 1
  unsigned long oldval;
  //  asm volatile("prfm    pstl1strm, %0" :: "r"(&v->counter));
  asm volatile("1: ldaxr %w0, [%1]" : "=&r"(oldval) : "r"(&v->counter));
  asm volatile("eor %0, %1, %2" : "=r"(tmp) : "r"(oldval), "r"(old));
  asm volatile("cbnz %0, 2f" :: "r"(tmp));
  asm volatile("stxr %w0, %w2, [%1]" : "=&r"(tmp) : "r"(&v->counter), "r"(new));
  asm volatile("cbnz %0, 1b" :: "r"(tmp));
  asm volatile("2:");
#else  
  /* Large System Extension (LSE) in ARMv8.1 */
  asm volatile("mov %w0, %w1" : "=r"(tmp) : "r"(old));
  asm volatile("cas %w0, %w1, [%2]" : "=r"(tmp), "=r"(new), "=r"(v->counter));
#endif
}

static inline void atomic_dec(atomic_t *v)
{
  unsigned long tmp, old, new;
  old = v->counter;
  new = v->counter - 1;
#if 1
  unsigned long oldval;
  //  asm volatile("prfm    pstl1strm, %0" :: "r"(&v->counter));
  asm volatile("1: ldaxr %w0, [%1]" : "=&r"(oldval) : "r"(&v->counter));
  asm volatile("eor %0, %1, %2" : "=r"(tmp) : "r"(oldval), "r"(old));
  asm volatile("cbnz %0, 2f" :: "r"(tmp));
  asm volatile("stxr %w0, %w2, [%1]" : "=&r"(tmp) : "r"(&v->counter), "r"(new));
  asm volatile("cbnz %0, 1b" :: "r"(tmp));
  asm volatile("2:");
#else  
  /* Large System Extension (LSE) in ARMv8.1 */
  asm volatile("mov %w0, %w1" : "=r"(tmp) : "r"(old));
  asm volatile("cas %w0, %w1, [%2]" : "=r"(tmp), "=r"(new), "=r"(v->counter));
#endif
}

static inline int fetch_and_inc(int *i)
{
	/* TODO: use atomic instruction */
	return *++i;
}

static inline int fetch_and_dec(int *i)
{
	/* TODO: use atomic instruction */
	return *--i;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_ARM_ATOMIC_H__ */
