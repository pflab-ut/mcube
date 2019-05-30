/**
 * @file include/aarch64/spinlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_SPINLOCK_H__
#define __MCUBE_AARCH64_SPINLOCK_H__

#ifndef __ASSEMBLY__


static inline void spin_lock(spinlock_t *lock)
{
  unsigned int tmp;
  asm volatile("sevl\n"
               "1:wfe\n"
               "2:ldaxr %w0, [%1]\n"
               "cbz %w0, 1b\n"
               "stxr %w0, %w2, [%1]\n"
               "cbnz %w0, 2b\n"
               : "=&r"(tmp)
               : "r"(lock), "r"(SPIN_LOCKED)
               : "memory");

}


static inline bool spin_trylock(spinlock_t *lock)
{
  unsigned int tmp;

  asm volatile("ldaxr %w0, [%1]\n"
               "cbz %w0, 1f\n"
               "stxr %w0, %w2, [%1]\n"
               "1:\n"
               : "=&r"(tmp)
               : "r"(lock), "r"(SPIN_LOCKED)
               : "memory");

  return !tmp;
}


static inline void spin_unlock(spinlock_t *lock)
{
  asm volatile("stlr %w1, [%0]\n"
               :: "r"(lock), "r"(SPIN_UNLOCKED) : "memory");
}


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AARCH64_SPINLOCK_H__ */
