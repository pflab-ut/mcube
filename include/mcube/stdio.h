/**
 * @file include/mcube/stdio.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_STDIO_H__
#define	__MCUBE_MCUBE_STDIO_H__

#ifndef __ASSEMBLY__

#define FOUT_SZ 0x200
#define MAX_DIGIT 32


#if CONFIG_ARCH_SIM

#include <stdio.h>
#include <unistd.h>

#if CONFIG_PRINTK2CONSOLE || CONFIG_PRINTK2UART || CONFIG_PRINTK2PRINTF
int printk(const char *format, ...) __attribute__ ((format (printf, 1, 2)));
#else
#error "Unknown Printk to Output"
#endif /* CONFIG_PRINTK2NCURSES */

#else

int getc(void);
int putchar(int c);
int puts(const char *s);

int printk(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

ssize_t console_write(const void *input_data, size_t length, void *devdata);
  
#endif /* CONFIG_ARCH_SIM */

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_STDIO_H__ */
