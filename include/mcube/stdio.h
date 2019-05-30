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

/*
 * Definitions for parsing format arguments. Each argument
 * is described by its descriptor (argdesc) structure.
 */
enum format_arglen {
  INT = 0,
  LONG,
};
enum format_argtype {
  NONE = 0,
  SIGNED,
  UNSIGNED,
  FLOAT,
  STRING,
  CHAR,
  PERCENT,
};

enum format_pad {
  PAD_NO = 0,
  PAD_BLANK,
  PAD_ZERO,
};

struct format_argdesc {
  int radix;
  enum format_arglen len;
  enum format_argtype type;
  enum format_pad pad;
  int digit;
  int float_digit;
};

void printk_bust_all_locks(void);
/* print for only kernel mode. */
//int printk(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
int printk(const char *fmt, ...);

int sscan(const char *str, const char *fmt, ...);
int vsnscan(const char *buf, const char *fmt, va_list args);



/* print for both kernel and user modes. */
int print(const char *fmt, ...);

int vsnprint(char *buf, int size, const char *fmt, va_list args);

void serial_putc(char c);
int print_uart(const char *fmt, ...);

const char *parse_arg(const char *fmt, struct format_argdesc *desc);

/*
 * We cannot use assert() for below printk() code as
 * the assert code istelf internally calls printk().
 */
#define format_assert(condition)                \
  do {                                          \
    if (__unlikely(!(condition))) {             \
      format_panic("!(" #condition ")");        \
    }                                           \
  } while (0);

__noreturn void format_panic(const char *str);


__noreturn void loop_print(char ch, int color);


#define INIT_DIGIT -1
#define INIT_FLOAT_DIGIT 6

#if !CONFIG_ARCH_SIM

int sscanf(const char *str, const char *fmt, ...);
int printf(const char *fmt, ...);
int sprintf(char *str, const char *format, ...);
void perror(const char *string);
int getc(void);
int putchar(int c);
int puts(const char *s);


#if CONFIG_ARCH_X86_64
void putchar_colored(char c, int color);
#endif /* CONFIG_ARCH_X86_64 */


#endif /* !CONFIG_ARCH_SIM */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_STDIO_H__ */
