/**
 * @file include/axis/atomic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_ATOMIC_H__
#define __MCUBE_AXIS_ATOMIC_H__

#ifndef __ASSEMBLY__

/**
 * @var atomic_lock
 * @brief Lock for atomic operations.
 */
extern spinlock_t atomic_lock;


/**
 * @fn static inline uint64_t atomic_inc(uint64_t *val)
 * @brief atomic increment.
 *
 * @param val Pointer to value.
 * @return Updated value.
 */
static inline uint64_t atomic_inc(uint64_t *val)
{
  spin_lock(&atomic_lock);
  (*val)++;
  spin_unlock(&atomic_lock);
  return *val;
}

/**
 * @fn static inline uint64_t atomic_dec(uint64_t *val)
 * @brief atomic decrement.
 *
 * @param val Pointer to value.
 * @return Updated value.
 */
static inline uint64_t atomic_dec(uint64_t *val)
{
  spin_lock(&atomic_lock);
  (*val)--;
  spin_unlock(&atomic_lock);
  return *val;
}


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AXIS_ATOMIC_H__ */
