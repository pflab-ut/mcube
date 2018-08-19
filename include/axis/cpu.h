/**
 * @file include/axis/cpu.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_AXIS_CPU_H__
#define	__MCUBE_AXIS_CPU_H__


#ifndef __ASSEMBLY__

static inline void set_cpu_id(unsigned long *x,
                              unsigned long *y,
                              unsigned long *local_cpu_id,
                              unsigned long cpu_id)
{
  *x = (cpu_id >> 24) & 0xff;
  *y = (cpu_id >> 16) & 0xff;
  *local_cpu_id = cpu_id & 0x7;
}



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_AXIS_CPU_H__ */
