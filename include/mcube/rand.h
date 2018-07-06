/**
 * @file include/mcube/rand.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_RAND_H__
#define __MCUBE_MCUBE_RAND_H__

#ifndef __ASSEMBLY__

#define INIT_GEN_RAND(x) init_gen_rand(x)
#define GEN_RAND() ((double) gen_rand32() / UINT_MAX)
#define GEN_RAND_UINT() gen_rand32()
//#define GEN_RAND() gen_rand32()


//#define GET_ACET() (GEN_RAND() * (1 - (double) emin / 100) + (double) emin / 100)
//#define GET_ACET() ((emin * UINT_MAX) / 100 + ((100 - emin) * GEN_RAND()) / UINT_MAX)
#if CONFIG_ENABLE_FPU
#define GET_ACET() (GEN_RAND() * (double) (1 - arg_param.emin / 100.0) + (double) arg_param.emin / 100.0)
#else
#define GET_ACET() ((GEN_RAND_UINT() * (100 - arg_param.emin) + arg_param.emin) / 100)
#endif /* CONFIG_ENABLE_FPU */

static inline uint64_t get_acet(uint64_t x)
{
	uint64_t tmp;

	tmp = x * GET_ACET();
	//	tmp = (uint64_t) (x * GET_ACET());
	while (tmp < (x * arg_param.emin) / 100 || tmp == 0) {
		tmp++;
	}
	return tmp;
}

#endif /* !__ASSEMBLY__ */

#endif  /* __MCUBE_MCUBE_RAND_H__ */
