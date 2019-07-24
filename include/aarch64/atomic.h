/**
 * @file include/aarch64/atomic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_ATOMIC_H__
#define __MCUBE_AARCH64_ATOMIC_H__

#ifndef __ASSEMBLY__

/**
 * @fn static inline uint64_t compare_and_swap(volatile uint64_t *ptr, uint64_t new_val,
 *                                             uint64_t old_val)
 * @brief compare and swap.
 *
 * @param ptr Pointer to data.
 * @param new_val New value.
 * @param old_val Old value.
 */
static inline void compare_and_swap(volatile uint64_t *ptr, uint64_t new_val,
                                    uint64_t old_val)
{
  uint64_t tmp;
#if 1
  uint64_t tmp2;
  //  asm volatile("prfm    pstl1strm, %0" :: "r"(&v->counter));
  asm volatile("1: ldaxr %x0, [%1]" : "=&r"(tmp2) : "r"(ptr));
  asm volatile("eor %x0, %x1, %x2" : "=r"(tmp) : "r"(tmp2), "r"(old_val));
  asm volatile("cbnz %x0, 2f" :: "r"(tmp));
  asm volatile("stxr %w0, %x2, [%1]" : "=&r"(tmp) : "r"(ptr), "r"(new_val));
  asm volatile("cbnz %x0, 1b" :: "r"(tmp));
  asm volatile("2:");
#else
  /* Large System Extension (LSE) in ARMv8.1 */
  asm volatile("mov %x0, %x1" : "=r"(tmp) : "r"(old_val));
  asm volatile("cas %x0, %x1, [%2]" : "=r"(tmp), "=r"(new_val), "=r"(*ptr));
#endif
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

#endif /* __MCUBE_AARCH64_ATOMIC_H__ */
