/**
 * @file include/arm/system.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_SYSTEM_H__
#define __MCUBE_ARM_SYSTEM_H__

#ifndef __ASSEMBLY__

static inline void generate_software_interrupt(volatile uint32_t id)
{
  /* TODO: implement */
}

static inline unsigned long get_current_el(void)
{
  unsigned long reg;
  asm volatile("mrs %0, CurrentEL" : "=r"(reg));
  return reg >> 2;
}


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_ARM_SYSTEM_H__ */
