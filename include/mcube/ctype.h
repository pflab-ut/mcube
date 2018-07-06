/**
 * @file include/mcube/ctype.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_CTYPE_H__
#define	__MCUBE_MCUBE_CTYPE_H__

#ifndef __ASSEMBLY__

/** Upper. */
#define _U (0x1 << 0)
/** Lower. */
#define _L (0x1 << 1)
/** Digit. */
#define _D (0x1 << 2)
/** Cntrl. */
#define _C (0x1 << 3)
/** Punct. */
#define _P (0x1 << 4)
/** White space (space/lf/tab). */
#define _S (0x1 << 5)
/** Hex digit */
#define _X (0x1 << 6)
/* Hard space (0x20) */
#define _SP (0x1 << 7)

extern int _ctype[];

#define __ismask(x) (_ctype[(x)])

extern int isalnum(int c);
extern int isalpha(int c);
extern int iscntrl(int c);
extern int isdigit(int c);
extern int isgraph(int c);
extern int islower(int c);
extern int isprint(int c);
extern int ispunct(int c);
extern int isspace(int c);
extern int isupper(int c);
extern int isxdigit(int c);

extern int isascii(int c);
extern int toascii(int c);
extern int tolower(int c);
extern int toupper(int c);

#endif /* !__ASSEMBLY__ */


#endif	/* __MCUBE_MCUBE_CTYPE_H__ */
