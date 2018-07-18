/**
 * @file include/mcube/mt.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_MT_H__
#define __MCUBE_MCUBE_MT_H__

#ifndef __ASSEMBLY__

uint32_t gen_rand32(void);
uint64_t gen_rand64(void);
void init_gen_rand(uint32_t seed);

/*------------------------------------------------------
  128-bit SIMD data type for Altivec, SSE2 or standard C
  ------------------------------------------------------*/
/**
 * @brief 128-bit data structure
 *
 * The W128_T structure has 128-bit data structure.
 */
struct W128_T {
	/** 32-bit data structure x 4. */
	uint32_t u[4];
};


/** 128-bit unsigned integer data type. */
typedef struct W128_T w128_t;

#define MEXP 19937

#if MEXP == 19937

/*-----------------
  BASIC DEFINITIONS
  -----------------*/
/** Mersenne Exponent. The period of the sequence 
 *  is a multiple of 2^MEXP-1.
 * #define MEXP 19937 */
/** SFMT generator has an internal state array of 128-bit integers,
 * and N is its size. */
#define SFMT_ARRAY_SIZE (MEXP / 128 + 1)
/** N32 is the size of internal state array when regarded as an array
 * of 32-bit integers.*/
#define SFMT_ARRAY_SIZE32 (SFMT_ARRAY_SIZE * 4)
/** N64 is the size of internal state array when regarded as an array
 * of 64-bit integers.*/
#define SFMT_ARRAY_SIZE64 (SFMT_ARRAY_SIZE * 2)

#define POS1	122
#define SL1	18
#define SL2	1
#define SR1	11
#define SR2	1
#define MSK1	0xdfffffef
#define MSK2	0xddfecb7f
#define MSK3	0xbffaffff
#define MSK4	0xbffffff6
#define PARITY1	0x00000001
#define PARITY2	0x00000000
#define PARITY3	0x00000000
#define PARITY4	0x13c9e684


/* PARAMETERS FOR ALTIVEC */
#define ALTI_SL1	{SL1, SL1, SL1, SL1}
#define ALTI_SR1	{SR1, SR1, SR1, SR1}
#define ALTI_MSK	{MSK1, MSK2, MSK3, MSK4}
#define ALTI_MSK64	{MSK2, MSK1, MSK4, MSK3}
#define ALTI_SL2_PERM	{1,2,3,23,5,6,7,0,9,10,11,4,13,14,15,8}
#define ALTI_SL2_PERM64	{1,2,3,4,5,6,7,31,9,10,11,12,13,14,15,0}
#define ALTI_SR2_PERM	{7,0,1,2,11,4,5,6,15,8,9,10,17,12,13,14}
#define ALTI_SR2_PERM64	{15,0,1,2,3,4,5,6,17,8,9,10,11,12,13,14}
#define IDSTR	"SFMT-19937:122-18-1-11-1:dfffffef-ddfecb7f-bffaffff-bffffff6"

#else
#error "Invalid MEXP"
#endif

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_MT_H__ */
