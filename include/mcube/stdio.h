/**
 * @file include/mcube/stdio.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_STDIO_H__
#define __MCUBE_MCUBE_STDIO_H__

#ifndef __ASSEMBLY__

#define FOUT_SIZE 0x200
#define MAX_DIGIT 64

/* print for only kernel mode. */
//int printk(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
int printk(const char *fmt, ...);

/* print for both kernel and user modes. */
int print(const char *fmt, ...);

#if CONFIG_ARCH_SIM

#include <stdio.h>
#include <unistd.h>

#else

int getc(void);
int putchar(int c);
int puts(const char *s);

#if CONFIG_ARCH_X86
void putchar_colored(char c, int color);
#endif /* CONFIG_ARCH_X86 */

/* print for user mode. */
int printf(const char *fmt, ...);



#endif /* CONFIG_ARCH_SIM */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_STDIO_H__ */
