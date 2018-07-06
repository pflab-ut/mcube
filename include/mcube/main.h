/**
 * @file include/mcube/main.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_MAIN_H__
#define	__MCUBE_MCUBE_MAIN_H__

#ifndef __ASSEMBLY__

extern uint32_t Debug;

extern int main(int argc, char *argv[]);

extern void init_arch(void);
extern void exit_arch(void);


#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_MAIN_H__ */
