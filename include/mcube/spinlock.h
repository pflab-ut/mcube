/**
 * @file include/mcube/spinlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_SPINLOCK_H__
#define	__MCUBE_MCUBE_SPINLOCK_H__

#define	SPIN_UNLOCKED	0x1
#define	SPIN_LOCKED	0x0

#ifndef	__ASSEMBLY__

#define spin_lock_irqsave(lock, flags)
#define spin_unlock_irqrestore(lock, flags)

/**
 * @brief Spinlock
 *
 * The spinlock structure has spinlock information.
 */
struct spinlock {
	/** Lock counter. */
	uint32_t lock;
};

typedef struct spinlock spinlock;

#define INIT_SPINLOCK (struct spinlock) {		\
		.lock = SPIN_UNLOCKED,									\
			}

extern volatile spinlock printk_lock;
extern volatile spinlock smp_sched_lock;
extern volatile spinlock sched_lock;

static inline void spin_lock(volatile spinlock *lock);
static inline void spin_unlock(volatile spinlock *lock);
static inline int spin_trylock(volatile spinlock *lock);

void init_spinlock(void);

#endif	/* __ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_SPINLOCK_H__ */
