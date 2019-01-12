/**
 * @file include/axis/spinlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_AXIS_SPINLOCK_H__
#define	__MCUBE_AXIS_SPINLOCK_H__

#ifndef	__ASSEMBLY__


static inline void spin_lock(spinlock_t *lock)
{
  /* TODO: implement */
}

static inline bool spin_trylock(spinlock_t *lock)
{
  /* TODO: implement */
  return 0;
}

static inline void spin_unlock(spinlock_t *lock)
{
  /* TODO: implement */
}



#endif	/* __ASSEMBLY__ */

#endif	/* __MCUBE_AXIS_SPINLOCK_H__ */
