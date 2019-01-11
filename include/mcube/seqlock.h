/**
 * @file include/mcube/seqlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_SEQLOCK_H__
#define	__MCUBE_MCUBE_SEQLOCK_H__

#ifndef __ASSEMBLY__

/**
 * @struct seqlock
 * @brief Sequence lock
 */
struct seqlock {
	/** Sequence. */
	unsigned sequence;
	/** Lock. */
	atomic_int lock;
};

typedef struct seqlock seqlock;

/* Lock out other writers and update the count.
 * Acts like a normal spin_lock/unlock.
 * Don't need preempt_disable() because that is in the spin_lock already.
 */
static inline void write_seqlock(seqlock *sl)
{
	mcube_spin_lock(&sl->lock);
	++sl->sequence;
}   

static inline void write_sequnlock(seqlock *sl) 
{
	sl->sequence++;
	mcube_spin_unlock(&sl->lock);
}

static inline int write_tryseqlock(seqlock *sl)
{
	int ret = mcube_spin_trylock(&sl->lock);

	if (ret) {
		++sl->sequence;
	}
	return ret;
}

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_SEQLOCK_H__ */
