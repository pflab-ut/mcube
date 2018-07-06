/**
 * @file include/mcube/stdalgo.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_STDALGO_H__
#define __MCUBE_MCUBE_STDALGO_H__

#ifndef __ASSEMBLY__


extern uint64_t gcd(uint64_t m, uint64_t n);
extern uint64_t lcm(uint64_t m, uint64_t n);

extern int binsearch(char *word, args arg[], int n);

#endif /* !__ASSEMBLY__ */

#endif  /* __MCUBE_MCUBE_STDALGO_H__ */
