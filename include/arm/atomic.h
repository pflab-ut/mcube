/**
 * @file include/arm/atomic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_ARM_ATOMIC_H__
#define __MCUBE_ARM_ATOMIC_H__

#ifndef __ASSEMBLY__

#define LOCK_PREFIX_HERE

#define LOCK_PREFIX LOCK_PREFIX_HERE

static inline void atomic_inc(atomic *v)
{
	/* TODO: use atomic instruction */
	v->counter++;
}

static inline void atomic_dec(atomic *v)
{
	/* TODO: use atomic instruction */
	v->counter--;
}

static inline int fetch_and_inc(int *i)
{
	/* TODO: use atomic instruction */
	return *++i;
}

static inline int fetch_and_dec(int *i)
{
	/* TODO: use atomic instruction */
	return *--i;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_ARM_ATOMIC_H__ */
