/**
 * @file arch/axis/spinlock.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


spinlock_t atomic_lock;

/*
 * NOTE: system call is not implemented in AXIS architecture because it has only kernel level.
 * This implementation exists for compatability for other architectures.
 */

void arch_spin_lock(__unused spinlock_t *lock)
{
  unsigned long cpu = get_cpu_id();
  bakery_lock(current_th[cpu]->id);
}

bool arch_spin_trylock(__unused spinlock_t *lock)
{
  /* TODO: implement */
  return 0;
}

void arch_spin_unlock(__unused spinlock_t *lock)
{
  unsigned long cpu = get_cpu_id();
  bakery_unlock(current_th[cpu]->id);
}
