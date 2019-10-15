/**
 * @file include/axis/spinlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_SPINLOCK_H__
#define __MCUBE_AXIS_SPINLOCK_H__

#ifndef __ASSEMBLY__


/**
 * @fn void arch_spin_lock(__unused spinlock_t *lock)
 * @brief Architecture-specific spin_lock function.
 *
 * @param lock Lock.
 */
void arch_spin_lock(__unused spinlock_t *lock);

/**
 * @fn bool arch_spin_trylock(__unused spinlock_t *lock)
 * @brief Architecture-specific spin_trylock function.
 *
 * @param lock Lock.
 * @return True if @a lock was acquired from the _first_ try.
 */
bool arch_spin_trylock(__unused spinlock_t *lock);

/**
 * @fn bool arch_spin_unlock(__unused spinlock_t *lock)
 * @brief Architecture-specific spin_unlock function.
 *
 * @param lock Lock.
 */
void arch_spin_unlock(__unused spinlock_t *lock);


/**
 * @fn static inline void spin_lock(__unused spinlock_t *lock)
 * @brief Always try to acquire the lock while LOCK# is asserted.
 * Should the lock be already acquired, busy loop till that lock is marked
 * by its owner as free.
 * Several cores might have observed that free state,
 * let each one try to reacquire the lock again under LOCK#: only _one_
 * CPU will see the lock free state again and win. Others will just
 * re-enter the busy-wait state.
 *
 * This intentionally minimizes accessing data while LOCK# is asserted.
 *
 * NOTE! Take extreme care of triggered interrupts while spinning: IRQ
 * handlers may execute a new spin_lock() in context of the older lock
 * spinning loop; i.e. reentrant execution of below code, possibly
 * using the very same parameter or lock!
 *
 * @param lock Lock.
 */
static inline void spin_lock(__unused spinlock_t *lock)
{
  arch_spin_lock(lock);
}

/**
 * @fn static inline bool spin_trylock(__unused spinlock_t *lock)
 * @brief A spin_lock() carbon-copy, minus the spinning part.
 * lock() could've been written in terms of trylock(),
 * but I'll leave it as it is for code clarity at the cost of duplication.
 *
 * @param lock Lock.
 * @return True if @a lock was acquired from the _first_ try.
 */
static inline bool spin_trylock(__unused spinlock_t *lock)
{
  arch_spin_trylock(lock);
  return 0;
}

/**
 * @fn static inline void spin_unlock(__unused spinlock_t *lock)
 * @brief mark the lock as available.
 *
 * @param lock Lock.
 */
static inline void spin_unlock(__unused spinlock_t *lock)
{
  arch_spin_unlock(lock);
}



#endif /* __ASSEMBLY__ */

#endif /* __MCUBE_AXIS_SPINLOCK_H__ */
