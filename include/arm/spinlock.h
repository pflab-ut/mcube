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
}


static inline int spin_trylock(volatile atomic_int *lock)
{
	return 0;
}


static inline void spin_unlock(volatile atomic_int *lock)
{
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
