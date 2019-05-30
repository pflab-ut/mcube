/**
 * @file include/aarch64/system.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_SYSTEM_H__
#define __MCUBE_AARCH64_SYSTEM_H__

#ifndef __ASSEMBLY__

static inline void generate_software_interrupt(volatile unsigned long id)
{
  svc(id);
}

static inline unsigned long get_current_el(void)
{
  unsigned long reg;
  asm volatile("mrs %0, CurrentEL" : "=r"(reg));
  return reg >> 2;
}


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AARCH64_SYSTEM_H__ */
