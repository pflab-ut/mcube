/**
 * @file include/arm/ops.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_ARM_OPS_H__
#define	__MCUBE_ARM_OPS_H__

#ifndef __ASSEMBLY__


static inline void svc(unsigned long id)
{
  asm volatile("svc %0" :: "r"(id));
}


static inline void wfi(void)
{
  asm volatile("wfi");
}

static inline void wfe(void)
{
  asm volatile("wfe");
}

static inline void pause(void)
{
}

static inline void nop(void)
{
	asm volatile("nop");
}

/* instruction synchronization barrier */
static inline void isb(void)
{
	asm volatile("isb");
}

/* debug */
static inline void dbg(void)
{
	asm volatile("dbg");
}

/* data synchronization barrier */
static inline void dsb(void)
{
	asm volatile("dsb");
}

/* data memory barrier */
static inline void dmb(void)
{
	asm volatile("dmb");
}

static inline void sev(void)
{
	asm volatile("sev");
}



#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_ARM_OPS_H__ */
