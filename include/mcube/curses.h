/**
 * @file include/mcube/curses.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_KCURSES_H__
#define	__MCUBE_MCUBE_KCURSES_H__

#ifndef __ASSEMBLY__

extern int kmove(int y, int x);
extern int kdelch(void);
extern int kaddstr(const char *str);
extern int kclear(void);
extern int kclrtoeol(void);

extern void init_curses(void);
extern void exit_curses(void);
#endif


#endif	/* __MCUBE_MCUBE_KCURSES_H__ */
