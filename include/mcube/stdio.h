/**
 * @file include/mcube/stdio.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_STDIO_H__
#define __MCUBE_MCUBE_STDIO_H__

#ifndef __ASSEMBLY__

#define KBUF_SIZE 1024
#define MAX_DIGIT 64

void printk_bust_all_locks(void);
/* print for only kernel mode. */
//int printk(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
int printk(const char *fmt, ...);


/* print for both kernel and user modes. */
int print(const char *fmt, ...);

int vsnprint(char *buf, int size, const char *fmt, va_list args);

void serial_putc(char c);
int print_uart(const char *fmt, ...);


void __noreturn loop_print(char ch, int color);


#if !CONFIG_ARCH_SIM

int printf(const char *fmt, ...);
int sprintf(char *str, const char *format, ...);
int getc(void);
int putchar(int c);
int puts(const char *s);


#if CONFIG_ARCH_X86
void putchar_colored(char c, int color);
#endif /* CONFIG_ARCH_X86 */


#endif /* !CONFIG_ARCH_SIM */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_STDIO_H__ */
