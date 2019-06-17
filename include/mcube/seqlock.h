/**
 * @file include/mcube/seqlock.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_SEQLOCK_H__
#define __MCUBE_MCUBE_SEQLOCK_H__

#ifndef __ASSEMBLY__

/**
 * @struct seqlock
 * @brief Sequence lock
 */
struct seqlock {
  /**
   * Sequence.
   */
  unsigned sequence;

  /**
   * Lock.
   */
  spinlock_t lock;
};

/**
 * @typedef seqlock
 * @brief Typedef of @struct seqlock.
 */
typedef struct seqlock seqlock;

/* Lock out other writers and update the count.
 * Acts like a normal spin_lock/unlock.
 * Don't need preempt_disable() because that is in the spin_lock already.
 */

/**
 * @fn static inline void write_seqlock(seqlock *sl)
 * @brief write sequence lock.
 * @param sl Sequence lock.
 */
static inline void write_seqlock(seqlock *sl)
{
  spin_lock(&sl->lock);
  ++sl->sequence;
}

/**
 * @fn static inline void write_sequnlock(seqlock *sl)
 * @brief write sequence unlock.
 * @param sl Sequence lock.
 */
static inline void write_sequnlock(seqlock *sl)
{
  sl->sequence++;
  spin_unlock(&sl->lock);
}

/**
 * @fn static inline int write_tryseqlock(seqlock *sl)
 * @brief try sequence lock.
 *
 * @param sl Sequence lock.
 */
static inline int write_tryseqlock(seqlock *sl)
{
  int ret = spin_trylock(&sl->lock);

  if (ret) {
    ++sl->sequence;
  }

  return ret;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_SEQLOCK_H__ */
