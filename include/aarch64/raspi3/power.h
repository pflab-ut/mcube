/**
 * @file include/aarch64/raspi3/power.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_RASPI3_POWER_H__
#define __MCUBE_AARCH64_RASPI3_POWER_H__

/**
 * @def POWER_MANAGEMENT_RSTC
 * @brief Power management reset controller.
 */
#define POWER_MANAGEMENT_RSTC (POWER_MANAGEMENT_BASE + 0x1c)

/**
 * @def POWER_MANAGEMENT_RSTS
 * @brief Power management restart.
 */
#define POWER_MANAGEMENT_RSTS (POWER_MANAGEMENT_BASE + 0x20)

/**
 * @def POWER_MANAGEMENT_WDOG
 * @brief Power management watchdog.
 */
#define POWER_MANAGEMENT_WDOG (POWER_MANAGEMENT_BASE + 0x24)

/**
 * @def POWER_MANAGEMENT_WDOG_MAGIC
 * @brief Power management watchdog magic.
 */
#define POWER_MANAGEMENT_WDOG_MAGIC 0x5a000000

/**
 * @def POWER_MANAGEMENT_RSTC_FULLRST
 * @brief Power management reset controller full reset.
 */
#define POWER_MANAGEMENT_RSTC_FULLRST 0x00000020


#ifndef __ASSEMBLY__

/**
 * @fn void power_off(void)
 * @brief Power off.
 */
void power_off(void);

/**
 * @fn void reset(void)
 * @brief Reset.
 */
void reset(void);


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AARCH64_RASPI3_POWER_H__ */

