/**
 * @file include/sim/atomic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_ATOMIC_H__
#define __MCUBE_SIM_ATOMIC_H__

#ifndef __ASSEMBLY__

/**
 * @fn static inline uint64_t compare_and_swap(uint64_t *ptr, uint64_t new_val,
 *                                             uint64_t old_val)
 * @brief compare and swap.
 *
 * @param ptr Pointer to data.
 * @param new_val New value.
 * @param old_val Old value.
 */
static inline uint64_t compare_and_swap(uint64_t *ptr, uint64_t new_val,
                                        uint64_t old_val)
{
  uint64_t out;

  // newline after `lock' for the work around of apple's gas(?) bug.
  asm volatile("lock cmpxchgq %2,%1"
               : "=a"(out), "+m"(*ptr)
               : "q"(new_val), "0"(old_val)
               : "cc");

  return out;
}



/**
 * @fn static inline uint64_t atomic_inc(uint64_t *val)
 * @brief atomic increment.
 *
 * @param val Pointer to value.
 * @return Updated value.
 */
static inline uint64_t atomic_inc(uint64_t *val)
{
  compare_and_swap(val, *val + 1, *val);
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
  compare_and_swap(val, *val - 1, *val);
  return *val;
}



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_ATOMIC_H__ */
