/**
 * @file include/axis/atomic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_ATOMIC_H__
#define __MCUBE_AXIS_ATOMIC_H__

#ifndef __ASSEMBLY__


/*
 * Atomically execute:
 *  return *val++;
 */
static inline uint64_t atomic_inc(uint64_t *val)
{
  (*val)++;
  return *val;
}



#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AXIS_ATOMIC_H__ */
