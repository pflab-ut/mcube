/**
 * @file include/aarch64/raspi3/rand.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_RASPI3_RAND_H__
#define __MCUBE_AARCH64_RASPI3_RAND_H__

/**
 * @def RANDOM_NUMBER_GEN_CTRL
 * @brief Random number generator control.
 */
#define RANDOM_NUMBER_GEN_CTRL (RANDOM_NUMBER_GEN_BASE + 0x00)

/**
 * @def RANDOM_NUMBER_GEN_STATUS
 * @brief Random number generator status.
 */
#define RANDOM_NUMBER_GEN_STATUS (RANDOM_NUMBER_GEN_BASE + 0x04)

/**
 * @def RANDOM_NUMBER_GEN_DATA
 * @brief Random number generator data.
 */
#define RANDOM_NUMBER_GEN_DATA (RANDOM_NUMBER_GEN_BASE + 0x08)

/**
 * @def RANDOM_NUMBER_GEN_INT_MASK
 * @brief Random number generator interrupt mask.
 */
#define RANDOM_NUMBER_GEN_INT_MASK (RANDOM_NUMBER_GEN_BASE + 0x10)

/**
 * @def RANDOM_NUMBER_GEN_CTRL_ENABLE
 * @brief Random number generator control enable.
 */
#define RANDOM_NUMBER_GEN_CTRL_ENABLE (0x1 << 0)

/**
 * @def RANDOM_NUMBER_GEN_STATUS_INIT
 * @brief Random number generator status initialization.
 */
#define RANDOM_NUMBER_GEN_STATUS_INIT (0x1 << 18)

/**
 * @def RANDOM_NUMBER_GEN_INT_MASK_SET
 * @brief Random number generator interrupt mask set.
 */
#define RANDOM_NUMBER_GEN_INT_MASK_SET (0x1 << 0)


#ifndef __ASSEMBLY__

/**
 * @fn void init_rand_raspi3(void)
 * @brief initialize random number generator in Raspberry Pi 3.
 */
void init_rand_raspi3(void);

/**
 * @fn unsigned int rand_raspi3(unsigned int min, unsigned int max)
 * @brief Random number generator in Raspberry Pi 3.
 *
 * @param min Minimum.
 * @param max Maximum.
 * @return Random number.
 */
unsigned int rand_raspi3(unsigned int min, unsigned int max);


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AARCH64_RASPI3_RAND_H__ */

