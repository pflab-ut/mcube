/*
 * printf()-like methods: vsnprintf(), etc
 *
 * Copyright (C) 2009-2010 Ahmed S. Darwish <darwish.07@gmail.com>
*/

#include <mcube/mcube.h>


static char kbuf[1024];
static char sbuf[1024];

#if CONFIG_ARCH_AXIS
spinlock_t kbuf_lock;
spinlock_t sbuf_lock;
#else
spinlock_t kbuf_lock = INIT_SPINLOCK;
spinlock_t sbuf_lock = INIT_SPINLOCK;
#endif


/*
 * Definitions for parsing printk arguments. Each argument
 * is described by its descriptor (argdesc) structure.
 */
enum printf_arglen {
  INT = 0,
  LONG,
};
enum printf_argtype {
  NONE = 0,
  SIGNED,
  UNSIGNED,
  STRING,
  CHAR,
  PERCENT,
};

enum printf_pad {
  PAD_NO = 0,
  PAD_BLANK,
  PAD_ZERO,
};

struct printf_argdesc {
  int radix;
  enum printf_arglen len;
  enum printf_argtype type;
  enum printf_pad pad;
  int digit;
};


/*
 * We cannot use assert() for below printk() code as
 * the assert code istelf internally calls printk().
 */
#define printk_assert(condition)                \
  do {                                          \
    if (__unlikely(!(condition))) {             \
      printk_panic("!(" #condition ")");        \
    }                                           \
  } while (0);

/*
 * A panic() that can be safely used by printk().
 * We use putchar() as it directly invokes the VGA code.
 * NOTE! Don't use any assert()s in this function!
 */
static char panic_prefix[] = "PANIC: printk: ";
static __no_return void printk_panic(const char *str)
{
  const char *prefix;

  prefix = panic_prefix;
  while (*prefix != 0) {
    putchar(*prefix++);
  }
  
  while (*str != 0) {
    putchar(*str++);
  }
  
  halt();
}

#define PRINTK_MAX_RADIX  16

/*
 * Convert given unsigned long integer (@num) to ascii using
 * desired radix. Return the number of ascii chars printed.
 * @size: output buffer size
 */
static int ulout(unsigned long num, char *buf, int size, struct printf_argdesc *desc)
{
  static char digit[PRINTK_MAX_RADIX + 1] = "0123456789abcdef";
  int ret, digits;
  int i;

  printk_assert(desc->radix > 2 && desc->radix <= PRINTK_MAX_RADIX);

  digits = 0;
  if (num == 0) {
    digits++;
  }
  
  for (typeof(num) c = num; c != 0; c /= desc->radix) {
    digits++;
  }

  printk_assert(digits > 0);
  printk_assert(digits <= size);
  printk_assert(desc->digit <= size);
  if (desc->digit == -1) {
    ret = digits;
    for (; digits != 0; digits--) {
      buf[digits - 1] = digit[num % desc->radix];
      num = num / desc->radix;
    }
  } else {
    printk_assert(digits <= desc->digit);
    ret = desc->digit;
    for (i = 0; i < desc->digit - digits; i++) {
      switch (desc->pad) {
      case PAD_BLANK:
        buf[i] = ' ';
        break;
      case PAD_ZERO:
        buf[i] = '0';
      default:
        break;
      }
    }
    for (i = 0; i < digits; i++) {
      buf[desc->digit - i - 1] = digit[num % desc->radix];
      num = num / desc->radix;
    }
  }

  return ret;
}

/*
 * Convert given signed long integer (@num) to ascii using
 * desired radix. Return the number of ascii chars printed.
 * @size: output buffer size
 */
static int lout(signed long num, char *buf, int size, struct printf_argdesc *desc)
{
  printk_assert(desc->radix > 2 && desc->radix <= PRINTK_MAX_RADIX);

  if (num < 0) {
    /* Make room for the '-' */
    printk_assert(size >= 2);

    num *= -1;
    buf[0] = '-';

    return ulout(num, buf + 1, size - 1, desc) + 1;
  }

  return ulout(num, buf, size, desc);
}

/*
 * Parse given printf argument expression (@fmt) and save
 * the results to argument descriptor @desc.
 *
 * Input is in in the form: %ld, %d, %x, %lx, etc.
 *
 * Return @fmt after bypassing the '%' expression.
 * FIXME: Better only return printf-expression # of chars.
 */
static const char *parse_arg(const char *fmt, struct printf_argdesc *desc)
{
  bool complete;
  unsigned int digit_size;

  printk_assert(*fmt == '%');

  complete = false;
  desc->len = INT;
  desc->type = NONE;
  desc->radix = 10;
  desc->pad = PAD_NO;
  desc->digit = -1;
  
  while (*++fmt) {
    switch (*fmt) {
    case 'l':
      desc->len = LONG;
      break;
    case 'd':
      desc->type = SIGNED;
      complete = true;
      goto out;
    case 'u':
      desc->type = UNSIGNED;
      complete = true;
      goto out;
    case 'x':
      desc->type = UNSIGNED;
      desc->radix = 16;
      complete = true;
      goto out;
    case 's':
      desc->type = STRING;
      complete = true;
      goto out;
    case 'c':
      desc->type = CHAR;
      complete = true;
      goto out;
    case '%':
      desc->type = PERCENT;
      complete = true;
      goto out;
    case '0' ... '9':
      if (*fmt == '0') {
        desc->pad = PAD_ZERO;
        fmt++;
      } else {
        desc->pad = PAD_BLANK;
      }
      digit_size = 0;
      while ((*fmt >= '0') && (*fmt <= '9')) {
        digit_size = digit_size * 10 + (*fmt - '0');
        fmt++;
      }
      fmt--;
      desc->digit = digit_size;
      break;
    default:
      /* Unknown mark: complete by definition */
      //printf("Unknown mark: %d\n", *fmt);
      desc->type = NONE;
      complete = true;
      goto out;
    }
  }

 out:
  if (!complete) {
    printk_panic("Unknown/incomplete expression");
  }
  
  /* Bypass last expression char */
  if (*fmt != '\0') {
    fmt++;
  }
  
  return fmt;
}


/*
 * Formt given printf-like string (@fmt) and store the result
 * within at most @size bytes. This version does *NOT* append
 * a NULL to output buffer @buf; it's for internal use only.
 */
int vsnprint(char *buf, int size, const char *fmt, va_list args)
{
  struct printf_argdesc desc = {0};
  const char *s;
  char *str;
  long num;
  unsigned long unum;
  unsigned char ch;
  int len;

  if (size < 1) {
    return 0;
  }
  
  str = buf;
  while (*fmt) {
    while (*fmt != '\0' && *fmt != '%' && size != 0) {
      *str++ = *fmt++;
      size--;
    }

    /* Mission complete */
    if (*fmt == '\0' || size == 0) {
      break;
    }

    printk_assert(*fmt == '%');
    fmt = parse_arg(fmt, &desc);

    len = 0;
    switch (desc.type) {
    case SIGNED:
      if (desc.len == LONG) {
        num = va_arg(args, long);
      } else {
        num = va_arg(args, int);
      }
      len = lout(num, str, size, &desc);
      break;
    case UNSIGNED:
      if (desc.len == LONG) {
        unum = va_arg(args, unsigned long);
      } else {
        unum = va_arg(args, unsigned int);
      }
      len = ulout(unum, str, size, &desc);
      break;
    case STRING:
      s = va_arg(args, char *);
      if (!s) {
        s = "<*NULL*>";
      }
      len = strlen(s);
      len = min(size, len);
      strncpy(str, s, len);
      break;
    case CHAR:
      ch = (unsigned char) va_arg(args, int);
      *str = ch;
      len = 1;
      break;
    case PERCENT:
      *str = '%';
      len = 1;
      break;
    default:
      // fprintf(stderr, "Error: Unknown print type %d\n", desc.type);
      break;
      /* No-op */
    }
    str += len;
    size -= len;
  }

  printk_assert(str >= buf);
  return str - buf;
}

/*
 * VGA text-mode memory (0xb8000-0xbffff) access
 *
 * @vga_xpos and @vga_ypos forms the current cursor position
 * @vga_buffer: A buffer to access VGA RAM in a write-only mode.
 *
 * For scrolling, we need to copy the last 24 rows up one row, but
 * reading from VGA RAM is pretty darn slow and buggy[1], thus the
 * need for a dedicated buffer. As also once advised by Travis
 * Geiselbrecht, multiple terminals will be much easier to support
 * that way since everything on the screen will be backed up.
 *
 * [1] 20 seconds to write and scroll 53,200 rows on my core2duo
 * laptop.
 *
 * Do not use any assert()s in VGA code! (stack overflow)
 */

#if CONFIG_ARCH_X86

#define VGA_BASE    ((char *) VIRTUAL(0xb8000))
#define VGA_MAXROWS    25
#define VGA_MAXCOLS    80
#define VGA_DEFAULT_COLOR  VGA_COLOR(VGA_BLACK, VGA_WHITE)
#define VGA_AREA    (VGA_MAXROWS * VGA_MAXCOLS * 2)

static spinlock_t vga_lock = INIT_SPINLOCK;
static int vga_xpos, vga_ypos;
static char vga_buffer[VGA_AREA];

/*
 * Scroll the screen up by one row.
 * NOTE! only call while the vga lock is held
 */
static void vga_scrollup(int color) {
  char *src, *dst;
  int rows_24;
  uint16_t *vgap;

  src = vga_buffer + 2 * VGA_MAXCOLS;
  dst = vga_buffer;
  rows_24 = 2 * ((VGA_MAXROWS - 1) * VGA_MAXCOLS);

  memcpy_forward_nocheck(dst, src, rows_24);

  vgap = (uint16_t *)(vga_buffer + rows_24);
  for (int i = 0; i < VGA_MAXCOLS; i++) {
    *vgap++ = (color << 8) + ' ';
  }

  memcpy_nocheck(VGA_BASE, vga_buffer, VGA_AREA);
  vga_xpos = 0;
  vga_ypos--;
}

/*
 * Write given buffer to VGA ram and scroll the screen
 * up as necessary.
 */
static void vga_write(char *buf, int n, int color)
{
  int max_xpos = VGA_MAXCOLS;
  int max_ypos = VGA_MAXROWS;
  int old_xpos, old_ypos;
  int area, offset;

  /* NOTE! This will deadlock if the code enclosed
   * by this lock triggered exceptions: the default
   * exception handlers implicitly call vga_write() */
  spin_lock(&vga_lock);

  old_xpos = vga_xpos;
  old_ypos = vga_ypos;

  while (*buf && n--) {
    if (vga_ypos == max_ypos) {
      vga_scrollup(color);
      old_ypos = vga_ypos;
      old_xpos = vga_xpos;
    }

    if (*buf != '\n') {
      writew((color << 8) + *buf,
             vga_buffer + 2 * (vga_xpos + vga_ypos * max_xpos));
      vga_xpos++;
    }

    if (vga_xpos == max_xpos || *buf == '\n') {
      vga_xpos = 0;
      vga_ypos++;
    }

    buf++;
  }

  offset = 2 * (old_ypos * max_xpos + old_xpos);
  area = 2 * ((vga_ypos - old_ypos) * max_xpos + vga_xpos);
  memcpy_nocheck(VGA_BASE + offset, vga_buffer + offset, area);

  spin_unlock(&vga_lock);
}

/*
 * Without any formatting overhead, write a single
 * charactor to screen.
 *
 * @color: VGA_COLOR(bg, fg); check vga.h
 */

void putchar_colored(char c, int color)
{
  vga_write(&c, 1, color);
}

int putchar(int c)
{
  putchar_colored(c, VGA_DEFAULT_COLOR);
  return c;
}


/*
 * Do not permit any access to screen state after calling
 * this method.  This is for panic(), which is important
 * not to scroll away its critical messages afterwards.
 */
void printk_bust_all_locks(void)
{
  spin_lock(&vga_lock);
  spin_lock(&kbuf_lock);
}



#endif

int puts(const char *s)
{
  int i;
  int n = strlen(s);
  for (i = 0; i < n; i++) {
    putchar(s[i]);
  }
  return n;
}

int printf(const char *fmt, ...)
{
  va_list args;
  int n;

  /* NOTE! This will deadlock if the code enclosed
   * by this lock triggered exceptions: the default
   * exception handlers already call printk() */
  spin_lock(&kbuf_lock);

  va_start(args, fmt);
  n = vsnprint(kbuf, sizeof(kbuf), fmt, args);
  va_end(args);
  kbuf[n] = '\0';

  /* TODO: call_sys_write() */
#if CONFIG_ARCH_X86
  vga_write(kbuf, n, VGA_DEFAULT_COLOR);
#else
  puts(kbuf);
#endif
  
  spin_unlock(&kbuf_lock);
  return n;
}


/*
 * Kernel print, for VGA and serial outputs
 */
#if 1
int printk(const char *fmt, ...)
{
  va_list args;
  int n;

  /* NOTE! This will deadlock if the code enclosed
   * by this lock triggered exceptions: the default
   * exception handlers already call printk() */
  spin_lock(&kbuf_lock);

  va_start(args, fmt);
  n = vsnprint(kbuf, sizeof(kbuf), fmt, args);
  va_end(args);
  kbuf[n] = '\0';

#if CONFIG_ARCH_X86
  vga_write(kbuf, n, VGA_DEFAULT_COLOR);
#else
  puts(kbuf);
#endif
  
  spin_unlock(&kbuf_lock);
  return n;
}
#endif

int print_uart(const char *fmt, ...)
{
  va_list args;
  int n;

  spin_lock(&sbuf_lock);

  va_start(args, fmt);
  n = vsnprint(sbuf, sizeof(sbuf), fmt, args);
  va_end(args);
  sbuf[n] = '\0';

#if CONFIG_ARCH_X86
  serial_write(sbuf, n);
#elif CONFIG_ARCH_SIM || CONFIG_ARCH_AXIS
  puts(sbuf);
#elif CONFIG_ARCH_ARM
  uart_write(sbuf, n, NULL);
#endif
  
  spin_unlock(&sbuf_lock);
  return n;
}

