/**
 * @file include/mcube/spinlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_SPINLOCK_H__
#define	__MCUBE_MCUBE_SPINLOCK_H__


#define	MCUBE_SPIN_UNLOCKED	0x1
#define	MCUBE_SPIN_LOCKED	0x0

#ifndef	__ASSEMBLY__

#define spin_lock_irqsave(lock, flags)
#define spin_unlock_irqrestore(lock, flags)


extern volatile atomic_int sched_lock;

static inline void mcube_spin_lock(volatile atomic_int *lock);
static inline void mcube_spin_unlock(volatile atomic_int *lock);
static inline int mcube_spin_trylock(volatile atomic_int *lock);

void init_spinlock(void);

static inline void lock_scheduler(void)
{
}

static inline void unlock_scheduler(void)
{
}


#endif	/* __ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_SPINLOCK_H__ */
