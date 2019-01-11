/**
 * @file include/mcube/rwlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_RWLOCK_H__
#define	__MCUBE_MCUBE_RWLOCK_H__

#ifndef __ASSEMBLY__

/**
 * @struct rwlock
 * @brief Read/Write lock
 */
struct rwlock {
	/** Number of readers. */
	unsigned int nread;
	/** Writer lock. */
	atomic_int wlock;
};

typedef struct rwlock rwlock;

#define	INIT_RWLOCK	{nread: 0, wlock: MCUBE_SPIN_UNLOCKED}

static inline void readers_lock(rwlock *lock)
{
	mcube_spin_lock(&lock->wlock);
	lock->nread++;
	mcube_spin_unlock(&lock->wlock);
}

static inline void readers_unlock(rwlock *lock)
{
	mcube_spin_lock(&lock->wlock);
	lock->nread--;
	mcube_spin_unlock(&lock->wlock);
}

static inline void writers_lock(rwlock *lock)
{
	mcube_spin_lock(&lock->wlock);
	while (lock->nread > 0) {
		mcube_spin_unlock(&lock->wlock);
		mcube_spin_lock(&lock->wlock);
	}
}

static inline void writers_unlock(rwlock *lock)
{
	mcube_spin_unlock(&lock->wlock);
}

static inline int try_readers_lock(rwlock *lock)
{
	if (mcube_spin_trylock(&lock->wlock)) {
		lock->nread++;
		mcube_spin_unlock(&lock->wlock);
		return TRUE;
	}
	return FALSE;
}

static inline int try_writers_lock(rwlock *lock)
{
	if (mcube_spin_trylock(&lock->wlock)) {
		if (lock->nread > 0) {
			mcube_spin_unlock(&lock->wlock);
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_RWLOCK_H__ */
