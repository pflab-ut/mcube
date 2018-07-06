/**
 * @file include/mcube/cache.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_CACHE_H__
#define __MCUBE_MCUBE_CACHE_H__

#ifndef __ASSEMBLY__

#define	__DCACHE_ALIGNED__

extern void enable_icache(void);
extern void enable_dcache(void);
extern void enable_cache(void);

extern void disable_icache(void);
extern void disable_dcache(void);
extern void disable_cache(void);

extern void reset_icache(void);
extern void reset_dcache(void);
extern void reset_cache(void);

extern void init_cache(void);

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_CACHE_H__ */
