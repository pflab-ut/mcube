/**
 * @file include/x86/spinlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_SPINLOCK_H__
#define __MCUBE_X86_SPINLOCK_H__

#ifndef __ASSEMBLY__


static inline void spin_lock(volatile atomic_int *lock)
{
  volatile atomic_int x = SPIN_LOCKED;
  for (;;) {
    //		asm volatile("lock xchgl %1,%0" : "=r" (lock), "=r" (x): "0" (lock), "1" (x));
    asm volatile("lock xchg %1, [%0]" : "=r"(lock), "=r"(x) : "0"(lock), "1"(x));
    
    if (x == SPIN_UNLOCKED) {
      break;
    }
    pause();
  }
}


static inline int spin_trylock(volatile atomic_int *lock)
{
	atomic_int oldval;
	asm volatile("xchg %0,[%1]" :"=q" (oldval), "+m" (*lock) :"0" (0) : "memory");
	return oldval > 0;
}


static inline void spin_unlock(volatile atomic_int *lock)
{
	atomic_int oldval = SPIN_UNLOCKED;
  asm volatile("xchg %0, [%1]" : "=r"(oldval), "+m"(*lock) : "0"(oldval) : "memory");
}



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_SPINLOCK_H__ */
