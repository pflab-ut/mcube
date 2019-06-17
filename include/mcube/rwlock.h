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
 * @brief Read/Write lock.
 */
struct rwlock {
  /**
   * Number of readers.
   */
  unsigned int nread;

  /**
   * Writer lock.
   */
  spinlock_t wlock;
};

/**
 * @typedef rwlock
 * @brief Typedef of @struct rwlock.
 */
typedef struct rwlock rwlock;

/**
 * @def INIT_RWLOCK
 * @brief initialize rwlock.
 */
#define INIT_RWLOCK {.nread = 0, .wlock = SPIN_UNLOCKED}

/**
 * @fn static inline void readers_lock(rwlock *lock)
 * @brief lock reader.
 *
 * @param lock Lock.
 */
static inline void readers_lock(rwlock *lock)
{
  spin_lock(&lock->wlock);
  lock->nread++;
  spin_unlock(&lock->wlock);
}

/**
 * @fn static inline void readers_unlock(rwlock *lock)
 * @brief unlock reader.
 *
 * @param lock Lock.
 */
static inline void readers_unlock(rwlock *lock)
{
  spin_lock(&lock->wlock);
  lock->nread--;
  spin_unlock(&lock->wlock);
}

/**
 * @fn static inline void writers_lock(rwlock *lock)
 * @brief lock writer.
 *
 * @param lock Lock.
 */
static inline void writers_lock(rwlock *lock)
{
  spin_lock(&lock->wlock);

  while (lock->nread > 0) {
    spin_unlock(&lock->wlock);
    spin_lock(&lock->wlock);
  }
}

/**
 * @fn static inline void writers_unlock(rwlock *lock)
 * @brief unlock writer.
 *
 * @param lock Lock.
 */
static inline void writers_unlock(rwlock *lock)
{
  spin_unlock(&lock->wlock);
}

/**
 * @fn static inline int try_readers_lock(rwlock *lock)
 * @brief try reader lock.
 *
 * @param lock Lock.
 * @return True if lock is acquired.
 */
static inline int try_readers_lock(rwlock *lock)
{
  if (spin_trylock(&lock->wlock)) {
    lock->nread++;
    spin_unlock(&lock->wlock);
    return true;
  }

  return false;
}

/**
 * @fn static inline int try_writers_lock(rwlock *lock)
 * @brief try writer lock.
 *
 * @param lock Lock.
 * @return True if lock is acquired.
 */
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
