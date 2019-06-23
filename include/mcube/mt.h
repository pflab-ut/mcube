/**
 * @file include/mcube/mt.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_MT_H__
#define __MCUBE_MCUBE_MT_H__

#ifndef __ASSEMBLY__

/**
 * @fn uint32_t gen_rand32(void)
 * @brief get 32-bit random value.
 *
 * @return 32-bit random value.
 */
uint32_t gen_rand32(void);

/**
 * @fn uint64_t gen_rand64(void)
 * @brief get 64-bit random value.
 *
 * @return 64-bit random value.
 */
uint64_t gen_rand64(void);

/**
 * @fn void init_gen_rand(uint32_t seed)
 * @brief initialize random number.
 */
void init_gen_rand(uint32_t seed);

/*------------------------------------------------------
  128-bit SIMD data type for Altivec, SSE2 or standard C
  ------------------------------------------------------*/
/**
 * @struct W128_T
 * @brief 128-bit data structure.
 */
struct W128_T {
  /**
   * 32-bit data structure x 4.
   */
  uint32_t u[4];
};


/**
 * @typedef w128_t
 * @brief Typedef of @struct W128_T.
 */
typedef struct W128_T w128_t;

/**
 * @def MEXP
 * @brief Definition of MEXP.
 */
#define MEXP 19937

#if MEXP == 19937

/*-----------------
  BASIC DEFINITIONS
  -----------------*/
/*
 * Mersenne Exponent.
 * The period of the sequence is a multiple of 2^MEXP-1.
 */

/**
 * @def SFMT_ARRAY_SIZE
 * @brief have an internal state array of 128-bit integers, and N is its size.
 */
#define SFMT_ARRAY_SIZE (MEXP / 128 + 1)

/**
 * @def SFMT_ARRAY_SIZE32
 * @brief N32 is the size of internal state array when regarded as an array
 * of 32-bit integers.
 */
#define SFMT_ARRAY_SIZE32 (SFMT_ARRAY_SIZE * 4)

/**
 * @def SFMT_ARRAY_SIZE64
 * @brief N64 is the size of internal state array when regarded as an array
 * of 64-bit integers.
 */
#define SFMT_ARRAY_SIZE64 (SFMT_ARRAY_SIZE * 2)

/**
 * @def POS1
 * @brief Position 1.
 */
#define POS1 122

/**
 * @def SL1
 * @brief Shift left 1.
 */
#define SL1 18

/**
 * @def SL2
 * @brief Shift left 2.
 */
#define SL2 1

/**
 * @def SR1
 * @brief Shift right 1.
 */
#define SR1 11

/**
 * @def SR2
 * @brief Shift right 2.
 */
#define SR2 1

/**
 * @def MSK1
 * @brief Mask 1.
 */
#define MSK1 0xdfffffef

/**
 * @def MSK2
 * @brief Mask 2.
 */
#define MSK2 0xddfecb7f

/**
 * @def MSK3
 * @brief Mask 3.
 */
#define MSK3 0xbffaffff

/**
 * @def MSK4
 * @brief Mask 4.
 */
#define MSK4 0xbffffff6

/**
 * @def PARITY1
 * @brief Parity 1.
 */
#define PARITY1 0x00000001

/**
 * @def PARITY2
 * @brief Parity 2.
 */
#define PARITY2 0x00000000

/**
 * @def PARITY3
 * @brief Parity 3.
 */
#define PARITY3 0x00000000

/**
 * @def PARITY4
 * @brief Parity 4.
 */
#define PARITY4 0x13c9e684


/* PARAMETERS FOR ALTIVEC */

/**
 * @def ALTI_SL1
 * @brief Alternative shift left 1.
 */
#define ALTI_SL1 {SL1, SL1, SL1, SL1}

/**
 * @def ALTI_SR1
 * @brief Alternative shirt right 1.
 */
#define ALTI_SR1 {SR1, SR1, SR1, SR1}

/**
 * @def ALTI_MSK
 * @brief Alternative mask.
 */
#define ALTI_MSK {MSK1, MSK2, MSK3, MSK4}

/**
 * @def ALTI_MSK64
 * @brief Alternative mask 64.
 */
#define ALTI_MSK64 {MSK2, MSK1, MSK4, MSK3}

/**
 * @def ALTI_SL2_PERM
 * @brief Alternative shift left 2 perm.
 */
#define ALTI_SL2_PERM {1,2,3,23,5,6,7,0,9,10,11,4,13,14,15,8}

/**
 * @def ALTI_SL2_PERM64
 * @brief Alternative shift left 2 perm 64.
 */
#define ALTI_SL2_PERM64 {1,2,3,4,5,6,7,31,9,10,11,12,13,14,15,0}

/**
 * @def ALTI_SR2_PERM
 * @brief Alternative shift right 2 perm.
 */
#define ALTI_SR2_PERM {7,0,1,2,11,4,5,6,15,8,9,10,17,12,13,14}

/**
 * @def ALTI_SR2_PERM64
 * @brief Alternative shift right 2 perm 64.
 */
#define ALTI_SR2_PERM64 {15,0,1,2,3,4,5,6,17,8,9,10,11,12,13,14}

/**
 * @def IDSTR
 * @brief ID string.
 */
#define IDSTR "SFMT-19937:122-18-1-11-1:dfffffef-ddfecb7f-bffaffff-bffffff6"

#else
#error "Invalid MEXP"
#endif

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_MT_H__ */
