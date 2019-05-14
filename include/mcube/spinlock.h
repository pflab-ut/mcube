/**
 * @file include/mcube/spinlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_SPINLOCK_H__
#define __MCUBE_MCUBE_SPINLOCK_H__


#define SPIN_UNLOCKED 0x1
#define SPIN_LOCKED 0x0

#ifndef __ASSEMBLY__

/*
 * Careful! Spinlocks, ironically enough, are globals and thus
 * must be themselves protected against concurrent SMP access!
 */
typedef struct spinlock {
  uint32_t val;

  /*
   * Interrupts get disabled when entering spin-protected
   * critical regions; original state of CPU's 'IF' flag
   * is saved here ... to get restored upon lock release.
   */
  union rflags flags;
} spinlock_t;


#define INIT_SPINLOCK (spinlock_t) {.val = SPIN_UNLOCKED, .flags.raw = 0}


void spin_init(spinlock_t *lock);
static inline void spin_lock(spinlock_t *lock);
static inline bool spin_trylock(spinlock_t *lock);
static inline void spin_unlock(spinlock_t *lock);

extern spinlock_t sched_lock;
extern spinlock_t kbuf_lock;


void init_spinlock(void);


#endif /* __ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_SPINLOCK_H__ */
