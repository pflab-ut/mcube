/**
 * @file include/arm/spinlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_SPINLOCK_H__
#define __MCUBE_ARM_SPINLOCK_H__

#ifndef __ASSEMBLY__


static inline void spin_lock(volatile atomic_int *lock)
{
  unsigned int tmp;
  asm volatile(
               "sevl\n"
               "1:wfe\n"
               "2:ldaxr %w0, [%1]\n"
               "cbz %w0, 1b\n"
               "stxr %w0, %w2, [%1]\n"
               "cbnz %w0, 2b\n"
               : "=&r" (tmp)
               : "r" (lock), "r" (SPIN_LOCKED)
               : "memory");
  
}


static inline int spin_trylock(volatile atomic_int *lock)
{
  unsigned int tmp;

  asm volatile(
               "ldaxr %w0, [%1]\n"
               "cbz %w0, 1f\n"
               "stxr %w0, %w2, [%1]\n"
               "1:\n"
               : "=&r" (tmp)
               : "r" (lock), "r" (SPIN_LOCKED)
               : "memory");

  return !tmp;
}


static inline void spin_unlock(volatile atomic_int *lock)
{
  asm volatile(
               "stlr %w1, [%0]\n"
               : : "r" (lock), "r" (SPIN_UNLOCKED) : "memory");
}

static inline void lock_scheduler(void)
{
#if CONFIG_ASSIGN_GLOBAL
	spin_lock(&sched_lock);
#endif /* CONFIG_ASSIGN_GLOBAL */
}

static inline void unlock_scheduler(void)
{
#if CONFIG_ASSIGN_GLOBAL
	spin_unlock(&sched_lock);
#endif /* CONFIG_ASSIGN_GLOBAL */
}


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_ARM_SPINLOCK_H__ */
