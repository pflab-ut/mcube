/**
 * @file include/arm/raspi3/power.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_ARM_RASPI3_POWER_H__
#define	__MCUBE_ARM_RASPI3_POWER_H__

#define POWER_MANAGEMENT_RSTC (POWER_MANAGEMENT_BASE + 0x1c)
#define POWER_MANAGEMENT_RSTS (POWER_MANAGEMENT_BASE + 0x20)
#define POWER_MANAGEMENT_WDOG (POWER_MANAGEMENT_BASE + 0x24)

#define POWER_MANAGEMENT_WDOG_MAGIC   0x5a000000
#define POWER_MANAGEMENT_RSTC_FULLRST 0x00000020


#ifndef __ASSEMBLY__

void power_off(void);
void reset(void);


#endif /* !__ASSEMBLY__ */


#endif /*	__MCUBE_ARM_RASPI3_POWER_H__ */

