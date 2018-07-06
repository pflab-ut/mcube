/**
 * @file include/mcube/symbol.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_SYMBOL_H__
#define	__MCUBE_MCUBE_SYMBOL_H__


/* These look damn.  But just in case some other binary formats requests
 * leading _ or whatsoever in their C symbols */

/* Don't put parentheses around x */
#define	LINK_SYMBOL(x)	x
#define	ASMLINK_SYMBOL(x)	stringify(x)

#endif	/* __MCUBE_MCUBE_SYMBOL_H__ */
