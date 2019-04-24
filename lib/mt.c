/**
 * @file lib/mt.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

/*--------------------------------------
  FILE GLOBAL VARIABLES
  internal state, index counter and flag
  --------------------------------------*/
/** the 128-bit internal state array */
static w128_t sfmt[SFMT_ARRAY_SIZE];
/** the 32bit integer pointer to the 128-bit internal state array */
static uint32_t *psfmt32 = &sfmt[0].u[0];
/** the 64bit integer pointer to the 128-bit internal state array */
static uint64_t *psfmt64 = (uint64_t *) &sfmt[0].u[0];
/** index counter to the 32-bit internal state array */
static int idx;
/** a flag: it is 0 if and only if the internal state is not yet
 * initialized. */
static int initialized = 0;
/** a parity check vector which certificate the period of 2^{MEXP} */
static uint32_t parity[4] = {PARITY1, PARITY2, PARITY3, PARITY4};



static inline int idxof(int i)
{
  return i;
}

static inline void rshift128(w128_t *out, w128_t const *in, int shift)
{
  uint64_t th, tl, oh, ol;

  th = ((uint64_t) in->u[3] << 32) | ((uint64_t) in->u[2]);
  tl = ((uint64_t) in->u[1] << 32) | ((uint64_t) in->u[0]);

  oh = th >> (shift * 8);
  ol = tl >> (shift * 8);
  ol |= th << (64 - shift * 8);
  out->u[1] = (uint32_t)(ol >> 32);
  out->u[0] = (uint32_t) ol;
  out->u[3] = (uint32_t)(oh >> 32);
  out->u[2] = (uint32_t) oh;
}

static inline void lshift128(w128_t *out, w128_t const *in, int shift)
{
  uint64_t th, tl, oh, ol;

  th = ((uint64_t) in->u[3] << 32) | ((uint64_t) in->u[2]);
  tl = ((uint64_t) in->u[1] << 32) | ((uint64_t) in->u[0]);

  oh = th << (shift * 8);
  ol = tl << (shift * 8);
  oh |= tl >> (64 - shift * 8);
  out->u[1] = (uint32_t)(ol >> 32);
  out->u[0] = (uint32_t) ol;
  out->u[3] = (uint32_t)(oh >> 32);
  out->u[2] = (uint32_t) oh;
}

#if (!defined(HAVE_ALTIVEC)) && (!defined(HAVE_SSE2))
static inline void do_recursion(w128_t *r, w128_t *a, w128_t *b, w128_t *c,
                                w128_t *d)
{
  w128_t x;
  w128_t y;

  lshift128(&x, a, SL2);
  rshift128(&y, c, SR2);
  r->u[0] = a->u[0] ^ x.u[0] ^ ((b->u[0] >> SR1) & MSK1) ^ y.u[0]
            ^ (d->u[0] << SL1);
  r->u[1] = a->u[1] ^ x.u[1] ^ ((b->u[1] >> SR1) & MSK2) ^ y.u[1]
            ^ (d->u[1] << SL1);
  r->u[2] = a->u[2] ^ x.u[2] ^ ((b->u[2] >> SR1) & MSK3) ^ y.u[2]
            ^ (d->u[2] << SL1);
  r->u[3] = a->u[3] ^ x.u[3] ^ ((b->u[3] >> SR1) & MSK4) ^ y.u[3]
            ^ (d->u[3] << SL1);
}
#endif

static inline void gen_rand_all(void)
{
  int i;
  w128_t *r1, *r2;

  r1 = &sfmt[SFMT_ARRAY_SIZE - 2];
  r2 = &sfmt[SFMT_ARRAY_SIZE - 1];

  for (i = 0; i < SFMT_ARRAY_SIZE - POS1; i++) {
    do_recursion(&sfmt[i], &sfmt[i], &sfmt[i + POS1], r1, r2);
    r1 = r2;
    r2 = &sfmt[i];
  }

  for (; i < SFMT_ARRAY_SIZE; i++) {
    do_recursion(&sfmt[i], &sfmt[i], &sfmt[i + POS1 - SFMT_ARRAY_SIZE], r1, r2);
    r1 = r2;
    r2 = &sfmt[i];
  }
}


static void period_certification(void)
{
  int inner = 0;
  int i, j;
  uint32_t work;

  for (i = 0; i < 4; i++) {
    inner ^= psfmt32[idxof(i)] & parity[i];
  }

  for (i = 16; i > 0; i >>= 1) {
    inner ^= inner >> i;
  }

  inner &= 1;

  /* check OK */
  if (inner == 1) {
    return;
  }

  /* check NG, and modification */
  for (i = 0; i < 4; i++) {
    work = 1;

    for (j = 0; j < 32; j++) {
      if ((work & parity[i]) != 0) {
        psfmt32[idxof(i)] ^= work;
        return;
      }

      work = work << 1;
    }
  }
}

uint32_t gen_rand32(void)
{
  uint32_t r;

  //	assert(initialized);
  if (idx >= SFMT_ARRAY_SIZE32) {
    gen_rand_all();
    idx = 0;
  }

  r = psfmt32[idx++];
  return r;
}

uint64_t gen_rand64(void)
{
  uint64_t r;

  //	assert(initialized);
  //	assert((idx % 2) == 0);

  if (idx >= SFMT_ARRAY_SIZE32) {
    gen_rand_all();
    idx = 0;
  }

  r = psfmt64[idx / 2];
  idx += 2;
  return r;
}

void init_gen_rand(uint32_t seed)
{
  int i;

  psfmt32[idxof(0)] = seed;

  for (i = 1; i < SFMT_ARRAY_SIZE32; i++) {
    psfmt32[idxof(i)] = 1812433253UL *
                        (psfmt32[idxof(i - 1)] ^ (psfmt32[idxof(i - 1)] >> 30))
                        + i;
  }

  idx = SFMT_ARRAY_SIZE32;
  period_certification();
  initialized = 1;
}
