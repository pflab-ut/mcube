/**
 * @file include/aarch64/raspi3/rand.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_RASPI3_RAND_H__
#define __MCUBE_AARCH64_RASPI3_RAND_H__

#define RANDOM_NUMBER_GEN_CTRL     (RANDOM_NUMBER_GEN_BASE + 0x00)
#define RANDOM_NUMBER_GEN_STATUS   (RANDOM_NUMBER_GEN_BASE + 0x04)
#define RANDOM_NUMBER_GEN_DATA     (RANDOM_NUMBER_GEN_BASE + 0x08)
#define RANDOM_NUMBER_GEN_INT_MASK (RANDOM_NUMBER_GEN_BASE + 0x10)


#define RANDOM_NUMBER_GEN_CTRL_ENABLE (0x1 << 0)
#define RANDOM_NUMBER_GEN_STATUS_INIT (0x1 << 18)
#define RANDOM_NUMBER_GEN_INT_MASK_SET (0x1 << 0)


#ifndef __ASSEMBLY__

void init_rand_raspi3(void);
unsigned int rand_raspi3(unsigned int min, unsigned int max);


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AARCH64_RASPI3_RAND_H__ */

