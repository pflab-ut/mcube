/**
 * @file include/mcube/seqlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_SEQLOCK_H__
#define	__MCUBE_MCUBE_SEQLOCK_H__

#ifndef __ASSEMBLY__

/**
 * @brief Sequence lock information
 *
 * The seqlock_struct structure has sequence lock information.
 */
struct seqlock_struct {
	/** Sequence. */
	unsigned sequence;
	/** Lock. */
	atomic_int lock;
};

typedef struct seqlock_struct seqlock_struct;

/* Lock out other writers and update the count.
 * Acts like a normal spin_lock/unlock.
 * Don't need preempt_disable() because that is in the spin_lock already.
 */
static inline void write_seqlock(seqlock_struct *sl)
{
	spin_lock(&sl->lock);
	++sl->sequence;
}   

static inline void write_sequnlock(seqlock_struct *sl) 
{
	sl->sequence++;
	spin_unlock(&sl->lock);
}

static inline int write_tryseqlock(seqlock_struct *sl)
{
	int ret = spin_trylock(&sl->lock);

	if (ret) {
		++sl->sequence;
	}
	return ret;
}

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_SEQLOCK_H__ */
