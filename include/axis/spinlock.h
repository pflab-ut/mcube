/**
 * @file include/axis/spinlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_AXIS_SPINLOCK_H__
#define	__MCUBE_AXIS_SPINLOCK_H__

#ifndef	__ASSEMBLY__


static inline void spin_lock(volatile spinlock *lock)
{
}

static inline void spin_unlock(volatile spinlock *lock)
{
}

static inline int spin_trylock(volatile spinlock *lock)
{
  return 0;
}


#endif	/* __ASSEMBLY__ */

#endif	/* __MCUBE_AXIS_SPINLOCK_H__ */
