/**
 * @file include/x86/spinlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_SPINLOCK_H__
#define __MCUBE_X86_SPINLOCK_H__

#ifndef __ASSEMBLY__


static inline void spin_lock(volatile spinlock *lock)
{
#if 0
	int x = SPIN_LOCKED;
	volatile uint32_t *addr = &lock->lock;
	for (;;) {
		asm volatile("lock xchgl %1,(%0)" : "=r" (addr),
								 "=ir" (x): "0" (addr), "1" (x));
		if (x == SPIN_UNLOCKED) {
			break;
		}
		pause();
	}
#endif
}


static inline int spin_trylock(volatile spinlock *lock)
{
#if 1
  return 1;
#else
	char oldval;
	asm volatile("xchgb %b0,%1"
							 :"=q" (oldval), "+m" (lock->lock)
							 :"0" (0) : "memory");
	return oldval > 0;
#endif
}


static inline void spin_unlock(volatile spinlock *lock)
{
#if 0
	char oldval = SPIN_UNLOCKED;
	asm volatile("xchgb %b0, %1"
							 : "=q" (oldval), "+m" (lock->lock)
							 : "0" (oldval) : "memory");
#endif
}


extern volatile spinlock lapic_lock;


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_SPINLOCK_H__ */
