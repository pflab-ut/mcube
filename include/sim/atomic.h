/**
 * @file include/sim/atomic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_ATOMIC_H__
#define __MCUBE_SIM_ATOMIC_H__

#ifndef __ASSEMBLY__

static inline uint64_t compare_and_swap(uint64_t *ptr, uint64_t new, uint64_t old)
{
  uint64_t out;

  // newline after `lock' for the work around of apple's gas(?) bug.
  asm volatile("lock cmpxchgq %2,%1"
                       : "=a" (out), "+m" (*ptr)
                       : "q" (new), "0" (old)
                       : "cc");

  return out;
}


/*
 * Atomically execute:
 *  old = *val & 0x1; *val |= 0x1;
 *  return old;
 */
static inline uint8_t atomic_bit_test_and_set(uint32_t *val)
{
  uint8_t ret;

  asm volatile(//"LOCK bts $0, %0;"
              "LOCK btr $0, %0;"
              "     setc    %1;"
              //"     setnc    %1;" 
    : "+m" (*val), "=qm" (ret)
    :
    : "cc", "memory");

  return ret;
}

/*
 * Atomically execute:
 *  return *++val;
 */
static inline uint64_t atomic_inc(uint64_t *val)
{
  compare_and_swap(val, *val + 1, *val);
  return *val;
}

/*
 * Atomically execute:
 *  return *--val;
 */
static inline uint64_t atomic_dec(uint64_t *val)
{
  compare_and_swap(val, *val - 1, *val);
  return *val;
}



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_ATOMIC_H__ */
