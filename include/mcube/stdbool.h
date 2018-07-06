/**
 * @file include/mcube/stdbool.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_BOOLEAN_H__
#define __MCUBE_MCUBE_BOOLEAN_H__

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#ifdef __ASSEMBLY__

#define FALSE 0
#define	TRUE 1

#else

enum boolean {
	FALSE = 0,
	TRUE
};

typedef enum boolean boolean;

#endif /* !__ASSEMBLY__ */


#define SUCCESS 1
#define FAILURE 0


#endif  /* __MCUBE_MCUBE_BOOLEAN_H__ */
