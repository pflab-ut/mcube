/**
 * @file include/x86/spinlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_SPINLOCK_H__
#define __MCUBE_X86_SPINLOCK_H__

/*
 * SMP spinlocks
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#ifndef __ASSEMBLY__

/*
 * Always try to acquire the lock while LOCK# is asserted. Should the
 * lock be already acquired, busy loop till that lock is marked by its
 * owner as free. Several cores might have observed that free state,
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
 */
static inline void spin_lock(spinlock_t *lock)
{
  union rflags flags;

  /* Reentrancy-safe place: stack or a register */
  save_local_irq(&flags);

  while (atomic_bit_test_and_set(&lock->val) == SPIN_LOCKED) {
    restore_local_irq(&flags);

    while (lock->val == SPIN_LOCKED) {
      cpu_pause();
    }

    disable_local_irq();
  }

  /*
   * Careful! Spinlocks (ironically enough) are globals & thus
   * must be themselves protected against concurrent SMP access.
   * Access a lock's elements if and only if it's already held.
   */
  lock->flags = flags;
}

/*
 * A spin_lock() carbon-copy, minus the spinning part. lock() could've
 * been written in terms of trylock(),  but I'll leave it as it is for
 * code clarity at the cost of duplication.
 *
 * Return 'true' if @lock was acquired from the _first_ try.
 */
static inline bool spin_trylock(spinlock_t *lock)
{
  union rflags flags;

  save_local_irq(&flags);

  if (atomic_bit_test_and_set(&lock->val) == SPIN_LOCKED) {
    restore_local_irq(&flags);
    return false;
  }

  lock->flags = flags;
  return true;
}

/*
 * Mark the lock as available
 */
static inline void spin_unlock(spinlock_t *lock)
{
  union rflags flags;

  /* Access a lock's elements iff it's already held. */
  flags = lock->flags;
  barrier();
  lock->val = SPIN_UNLOCKED;

  restore_local_irq(&flags);
}

/*
 * NOTE! As discussed above, lock() in terms of trylock():
 *
 *  while (!spin_trylock(lock))
 *    while (lock->val == SPIN_LOCKED)
 *      cpu_pause();
 *
 */


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_SPINLOCK_H__ */
