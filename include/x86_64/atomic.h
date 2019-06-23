/**
 * @file include/x86_64/atomic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_ATOMIC_H__
#define __MCUBE_X86_64_ATOMIC_H__

#ifndef __ASSEMBLY__


/**
 * @fn static inline uint64_t compare_and_swap(uint64_t *ptr, uint64_t new,
 *                                             uint64_t old)
 * @brief compare and swap.
 *
 * @param ptr Pointer to data.
 * @param new New value.
 * @param old Old value.
 */
static inline uint64_t compare_and_swap(uint64_t *ptr, uint64_t new,
                                        uint64_t old)
{
  uint64_t out;

  // newline after `lock' for the work around of apple's gas(?) bug.
  asm volatile("lock cmpxchgq %2,%1"
               : "=a"(out), "+m"(*ptr)
               : "q"(new), "0"(old)
               : "cc");

  return out;
}

/**
 * @fn static inline uint8_t atomic_bit_test_and_set(uint32_t *val)
 * @brief atomic bit test and set.
 *
 * @param val Value.
 */
static inline uint8_t atomic_bit_test_and_set(uint32_t *val)
{
  uint8_t ret;

  asm volatile(//"LOCK bts $0, %0;"
    "LOCK btr $0, %0;"
    "     setc    %1;"
    //"     setnc    %1;"
    : "+m"(*val), "=qm"(ret)
    :
    : "cc", "memory");

  return ret;
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


#endif /* __MCUBE_X86_64_ATOMIC_H__ */
