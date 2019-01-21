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
static inline void spin_lock(struct lock_spin *lock)
{
  union x86_rflags rflags;

  /* Reentrancy-safe place: stack or a register */
  rflags = local_irq_disable_save();

  while (atomic_bit_test_and_set(&lock->val) == SPIN_LOCKED) {
    local_irq_restore(rflags);

    while (lock->val == SPIN_LOCKED) {
      cpu_pause();
    }

    local_irq_disable();
  }

  /*
   * Careful! Spinlocks (ironically enough) are globals & thus
   * must be themselves protected against concurrent SMP access.
   * Access a lock's elements if and only if it's already held.
   */
  lock->rflags = rflags;
}

/*
 * A spin_lock() carbon-copy, minus the spinning part. lock() could've
 * been written in terms of trylock(),  but I'll leave it as it is for
 * code clarity at the cost of duplication.
 *
 * Return 'true' if @lock was acquired from the _first_ try.
 */
static inline bool spin_trylock(struct lock_spin *lock)
{
  union x86_rflags rflags;

  rflags = local_irq_disable_save();

  if (atomic_bit_test_and_set(&lock->val) == SPIN_LOCKED) {
    local_irq_restore(rflags);
    return false;
  }

  lock->rflags = rflags;
  return true;
}

/*
 * Mark the lock as available
 */
static inline void spin_unlock(struct lock_spin *lock)
{
  union x86_rflags rflags;

  /* Access a lock's elements iff it's already held. */
  rflags = lock->rflags;
  barrier();
  lock->val = SPIN_UNLOCKED;

  local_irq_restore(rflags);
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
