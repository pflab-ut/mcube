/**
 * @file include/sim/spinlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_SPINLOCK_H__
#define __MCUBE_SIM_SPINLOCK_H__

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


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_SPINLOCK_H__ */
