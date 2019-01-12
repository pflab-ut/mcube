/**
 * @file include/mcube/cache.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_CACHE_H__
#define __MCUBE_MCUBE_CACHE_H__

#ifndef __ASSEMBLY__

#define __DCACHE_ALIGNED__

void enable_icache(void);
void enable_dcache(void);
void enable_cache(void);

void disable_icache(void);
void disable_dcache(void);
void disable_cache(void);

void reset_icache(void);
void reset_dcache(void);
void reset_cache(void);

void init_cache(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_CACHE_H__ */
