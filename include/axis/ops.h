/**
 * @file include/axis/ops.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_OPS_H__
#define __MCUBE_AXIS_OPS_H__

#ifndef __ASSEMBLY__

static inline void nop(void)
{
  asm volatile ("nop");
}

static inline void sync(void)
{
  asm volatile("sync");
}

static inline void wi(void)
{
  asm volatile("wi");
}

static inline void ert(void)
{
  asm volatile("ert");
}


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AXIS_OPS_H__ */
