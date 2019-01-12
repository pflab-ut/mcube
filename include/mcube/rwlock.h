/**
 * @file include/mcube/rwlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_RWLOCK_H__
#define __MCUBE_MCUBE_RWLOCK_H__

#ifndef __ASSEMBLY__

/**
 * @struct rwlock
 * @brief Read/Write lock
 */
struct rwlock {
  /** Number of readers. */
  unsigned int nread;
  /** Writer lock. */
  spinlock_t wlock;
};

typedef struct rwlock rwlock;

#define INIT_RWLOCK {.nread = 0, .wlock = SPIN_UNLOCKED}

static inline void readers_lock(rwlock *lock)
{
  spin_lock(&lock->wlock);
  lock->nread++;
  spin_unlock(&lock->wlock);
}

static inline void readers_unlock(rwlock *lock)
{
  spin_lock(&lock->wlock);
  lock->nread--;
  spin_unlock(&lock->wlock);
}

static inline void writers_lock(rwlock *lock)
{
  spin_lock(&lock->wlock);
  while (lock->nread > 0) {
    spin_unlock(&lock->wlock);
    spin_lock(&lock->wlock);
  }
}

static inline void writers_unlock(rwlock *lock)
{
  spin_unlock(&lock->wlock);
}

static inline int try_readers_lock(rwlock *lock)
{
  if (spin_trylock(&lock->wlock)) {
    lock->nread++;
    spin_unlock(&lock->wlock);
    return true;
  }
  return false;
}

static inline int try_writers_lock(rwlock *lock)
{
  if (spin_trylock(&lock->wlock)) {
    if (lock->nread > 0) {
      spin_unlock(&lock->wlock);
      return false;
    }
    return true;
  }
  return false;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_RWLOCK_H__ */
