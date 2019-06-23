/**
 * @file include/mcube/stdio.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_STDIO_H__
#define __MCUBE_MCUBE_STDIO_H__

#ifndef __ASSEMBLY__

/**
 * @def KBUF_SIZE
 * @brief Kernel buffer size.
 */
#define KBUF_SIZE 1024

/**
 * @def MAX_DIGIT
 * @brief Maximum digit.
 */
#define MAX_DIGIT 64

/**
 * @enum format_arglen
 * @brief Format argument's length.
 *
 * Definitions for parsing format arguments. Each argument
 * is described by its descriptor (argdesc) structure.
 */
enum format_arglen {
  INT = 0,
  LONG,
};

/**
 * @enum format_argtype
 * @brief Format argument's type.
 */
enum format_argtype {
  NONE = 0,
  SIGNED,
  UNSIGNED,
  FLOAT,
  STRING,
  CHAR,
  PERCENT,
};

/**
 * @enum format_pad
 * @brief Format padding.
 */
enum format_pad {
  PAD_NO = 0,
  PAD_BLANK,
  PAD_ZERO,
};

/**
 * @struct format_argdesc
 * @brief Format argument's descriptor.
 */
struct format_argdesc {
  /**
   * Radix.
   */
  int radix;

  /**
   * Length.
   */
  enum format_arglen len;

  /**
   * Argument type.
   */
  enum format_argtype type;

  /**
   * Pad.
   */
  enum format_pad pad;

  /**
   * Digit.
   */
  int digit;

  /**
   * Float digit.
   */
  int float_digit;
};

/**
 * @fn void printk_bust_all_locks(void)
 * @brief unlock all locks if printk is bust.
 */
void printk_bust_all_locks(void);

/**
 * @fn int printk(const char *fmt, ...)
 * @brief print characters for kernel mode.
 *
 * @param fmt Format.
 * @return Number of characters printed.
 */
int printk(const char *fmt, ...);

/**
 * @fn int sscan(const char *str, const char *fmt, ...)
 * @brief scans the input from @a str.
 *
 * @param str Input string.
 * @param fmt Format.
 * @return On success, these functions return the number of input items successfully
 * matched and assigned; this can be fewer than provided for, or even zero,
 * in the event of an early matching failure.
 */
int sscan(const char *str, const char *fmt, ...);

/**
 * @fn int vsnscan(const char *str, const char *fmt, va_list args)
 * @brief scans the input from @a str.
 *
 * @param str Input string.
 * @param fmt Format.
 * @param args Arguments.
 */
int vsnscan(const char *str, const char *fmt, va_list args);


/**
 * @fn int print(const char *fmt, ...)
 * @brief print characters for both kernel and user modes.
 *
 * @param fmt Format.
 * @return Number of characters printed.
 */
int print(const char *fmt, ...);

/**
 * @fn int vsnprint(char *str, int size, const char *fmt, va_list args)
 * @brief print characters.
 *
 * @param str Input string.
 * @param size Size.
 * @param fmt Format.
 * @param args Arguments.
 * @return Number of characters printed.
 */
int vsnprint(char *str, int size, const char *fmt, va_list args);

/**
 * @fn int print_uart(const char *fmt, ...)
 * @brief print for UART.
 *
 * @param fmt Format.
 * @return Number of characters printed.
 */
int print_uart(const char *fmt, ...);

/**
 * @fn const char *parse_arg(const char *fmt, struct format_argdesc *desc)
 * @brief parse given print argument expression (@a fmt) and save
 * the results to argument descriptor @a desc.
 *
 * @param fmt Format.
 * @param desc Descriptor.
 *
 * @return @a fmt after bypassing the '%' expression.
 */
const char *parse_arg(const char *fmt, struct format_argdesc *desc);

/**
 * @def format_assert(condition)
 * @brief format assert.
 * We cannot use assert() for below printk() code as
 * the assert code istelf internally calls printk().
 *
 * @param condition Condition.
 */
#define format_assert(condition)                \
  do {                                          \
    if (__unlikely(!(condition))) {             \
      format_panic("!(" #condition ")");        \
    }                                           \
  } while (0);

/**
 * @fn __noreturn void format_panic(const char *str)
 * @brief format panic.
 */
__noreturn void format_panic(const char *str);


/**
 * @def INIT_DIGIT
 * @brief Initial digit.
 */
#define INIT_DIGIT -1

/**
 * @def INIT_FLOAT_DIGIT
 * @brief Initial float digit.
 */
#define INIT_FLOAT_DIGIT 6

#if !CONFIG_ARCH_SIM

/**
 * @fn int sscanf(const char *str, const char *fmt, ...)
 * @brief scans the input from @a str.
 *
 * @param str Input string.
 * @param fmt Format.
 * @return On success, these functions return the number of input items successfully
 * matched and assigned; this can be fewer than provided for, or even zero,
 * in the event of an early matching failure.
 */
int sscanf(const char *str, const char *fmt, ...);

/**
 * @fn int printf(const char *fmt, ...)
 * @brief print characters for both kernel and user modes.
 *
 * @param fmt Format.
 * @return Number of characters printed.
 */
int printf(const char *fmt, ...);

/**
 * @fn int sprintf(char *str, const char *fmt, ...)
 * @brief write to the character to @a str.
 *
 * @param str String.
 * @param fmt Format.
 * @return Number of characters printed.
 */
int sprintf(char *str, const char *fmt, ...);

/**
 * @fn void perror(const char *str)
 * @brief print error.
 *
 * @param str string.
 */
void perror(const char *str);

/**
 * @fn int putchar(int c)
 * @brief put character.
 *
 * @param c Character.
 * @return Character written as an unsigned char or EOF on error.
 */
int putchar(int c);

/**
 * @fn int puts(const char *s)
 * @brief put string.
 *
 * @param s string.
 * @return Nonnegative number on success, or EOF on error.
 */
int puts(const char *s);


#if CONFIG_ARCH_X86_64
/**
 * @fn void putchar_colored(char c, int color)
 * @brief put colored character.
 *
 * @param c Character.
 * @param color Color
 */
void putchar_colored(char c, int color);
#endif /* CONFIG_ARCH_X86_64 */


#endif /* !CONFIG_ARCH_SIM */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_STDIO_H__ */
