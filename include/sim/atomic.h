/**
 * @file include/sim/atomic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_ATOMIC_H__
#define __MCUBE_SIM_ATOMIC_H__

#ifndef __ASSEMBLY__

#define LOCK_PREFIX_HERE                        \
  ".section .smp_locks,\"a\"\n"                 \
  ".balign 4\n"                                 \
  ".long 671f - .\n" /* offset */               \
  ".previous\n"                                 \
  "671:"

#define LOCK_PREFIX LOCK_PREFIX_HERE "\n\tlock; "

static inline void atomic_inc(uint64_t *val)
{
  asm volatile(LOCK_PREFIX "incl %0"
               : "+m" (*val));
}

static inline void atomic_dec(uint64_t *val)
{
  asm volatile(LOCK_PREFIX "decl %0"
               : "+m" (*val));
}

static inline int fetch_and_inc(uint64_t *val)
{
  asm volatile(LOCK_PREFIX "incl %0"
               : "+m" (*val));
  return *val;
}

static inline int fetch_and_dec(int *i)
{
  asm volatile(LOCK_PREFIX "decl %0"
               : "+m" (*i));
  return *i;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_ATOMIC_H__ */
