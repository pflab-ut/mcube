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

int isalnum(int c);
int isalpha(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);

int isascii(int c);
int toascii(int c);
int tolower(int c);
int toupper(int c);

#endif /* !__ASSEMBLY__ */


#endif	/* __MCUBE_MCUBE_CTYPE_H__ */
