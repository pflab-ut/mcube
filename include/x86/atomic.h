/**
 * @file include/x86/atomic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_ATOMIC_H__
#define __MCUBE_X86_ATOMIC_H__

#ifndef __ASSEMBLY__

/*
 * Atomically execute:
 *  old = *val & 0x1; *val |= 0x1;
 *  return old;
 */
static inline uint8_t atomic_bit_test_and_set(uint32_t *val)
{
  uint8_t ret;

  asm volatile (
    "LOCK bts $0, %0;"
    "     setc    %1;"
    : "+m" (*val), "=qm" (ret)
    :
    : "cc", "memory");

  return ret;
}

/*
 * Atomically execute:
 *  return *val++;
 */
static inline uint64_t atomic_inc(uint64_t *val)
{
  uint64_t i = 1;

  asm volatile (
    "LOCK xaddq %0, %1"
    : "+r"(i), "+m" (*val)
    :
    : "cc");

  return i;
}



#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_X86_ATOMIC_H__ */
