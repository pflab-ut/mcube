/**
 * @file kernel/print.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * printf()-like methods: vsnprintf(), etc
 *
 * Copyright (C) 2009-2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#include <mcube/mcube.h>


#define PRINTK_MAX_RADIX 16

static char digit[PRINTK_MAX_RADIX + 1] = "0123456789abcdef";


static char pbuf[KBUF_SIZE];

#if CONFIG_ARCH_AXIS
spinlock_t pbuf_lock;
#else
spinlock_t pbuf_lock = INIT_SPINLOCK;
#endif /* CONFIG_ARCH_AXIS */

#define INT_BUFSIZE 32


#if CONFIG_ARCH_X86_64

static spinlock_t panic_lock = INIT_SPINLOCK;

/*
 * Quickly disable system interrupts upon entrance! Now the
 * kernel is in an inconsistent state, just gracefully stop
 * the machine and halt :-(
 *
 * An interesting case faced from not disabling interrupts
 * early on (disabling them at the function end instead) was
 * having other threads getting scheduled between printk()
 * and disabling interrupts, scrolling-away the caller panic
 * message and losing information FOREVER.
 */
void __noreturn panic(const char *fmt, ...)
{
  va_list args;
  int n;

  /* NOTE! Do not put anything above this */
  disable_local_irq();

  /*
   * NOTE! Manually assure that all the functions called
   * below are void of any asserts or panics.
   */

  /* Avoid concurrent panic()s: first call holds the most
   * important facts; the rest are usually side-effects. */
  if (!spin_trylock(&panic_lock)) {
    goto halt;
  }

  /* If other cores are alive, send them a fixed IPI, which
   * intentionally avoids interrupting cores with IF=0 till
   * they re-accept interrupts. Why?
   *
   * An interrupted critical region may  deadlock our panic
   * code if we tried to  acquire the same lock.  The other
   * cores may not also be in  long-mode before they enable
   * interrupts (e.g. in the 16-bit SMP trampoline step.)
   *
   * IPIs are sent only if more than one core is alive:  we
   * might be on so early a stage that our APIC registers
   * are not yet memory mapped, leading to memory faults if
   * locally accessed!
   *
   * If destination CPUs were alive but have not yet inited
   * their local APICs, they will not be able to catch this
   * IPI and will normally continue execution.  Beware.
   */
  if (smpboot_get_nr_alive_cpus() > 1) {
    apic_broadcast_ipi(APIC_DELMOD_FIXED, HALT_CPU_IPI_VECTOR);
  }

  va_start(args, fmt);
  n = vsnprint(pbuf, sizeof(pbuf) - 1, fmt, args);
  va_end(args);

  pbuf[n] = 0;
  printk("\nCPU#%d-PANIC: %s", percpu_get(apic_id), pbuf);

  /* Since the  other cores are stopped only after they re-
   * accept interrupts, they may print on-screen and scroll
   * away our message.  Acquire all screen locks, forever. */
  printk_bust_all_locks();

halt:
  halt();
}

#else

void __noreturn panic(const char *fmt, ...)
{
  char buf[KBUF_SIZE];
  va_list ap;
  va_start(ap, fmt);
  vsnprint(buf, sizeof(buf), fmt, ap);
  call_sys_write(buf);
  va_end(ap);

  for (;;)
    ;
}

#endif /* CONFIG_ARCH_X86_64 */




/*
 * Convert given unsigned long integer (@num) to ascii using
 * desired radix. Return the number of ascii chars printed.
 * @size: output buffer size
 */
static int luout(unsigned long num, char *buf, int size,
                 struct format_argdesc *desc)
{
  int ret, digits;
  int i;

  format_assert(desc->radix > 2 && desc->radix <= PRINTK_MAX_RADIX);

  digits = 0;

  if (num == 0) {
    digits++;
  } else {
    digits = get_digit(num, desc->radix);
  }

  format_assert(digits > 0);
  format_assert(digits <= size);
  format_assert(desc->digit <= size);

  if (desc->digit == INIT_DIGIT) {
    ret = digits;

    for (; digits != 0; digits--) {
      buf[digits - 1] = digit[num % desc->radix];
      num = num / desc->radix;
    }
  } else {
    format_assert(digits <= desc->digit);
    ret = desc->digit;

    for (i = 0; i < desc->digit - digits; i++) {
      switch (desc->pad) {
      case PAD_BLANK:
        buf[i] = ' ';
        break;

      case PAD_ZERO:
        buf[i] = '0';

      case PAD_NO:
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
static int lout(signed long num, char *buf, int size,
                struct format_argdesc *desc)
{
  format_assert(desc->radix > 2 && desc->radix <= PRINTK_MAX_RADIX);

  if (num < 0) {
    /* Make room for the '-' */
    format_assert(size >= 2);

    num *= -1;
    buf[0] = '-';

    return luout(num, buf + 1, size - 1, desc) + 1;
  }

  return luout(num, buf, size, desc);
}


#if defined(ENABLE_FPU)

static int lfout(double num, char *buf, int size,
                 struct format_argdesc *desc)
{
  bool sign = num < 0.0;
  int ret = 0;
  static char inf[] = "inf";
  static char nan[] = "NaN";
  uint64_t ulpart, dpart;
  char str[INT_BUFSIZE];
  int int_begin;
  int len;
  int dlen;
  int digit = desc->digit;
  int float_digit = desc->float_digit;

  if (isinf(num)) {
    if (sign) {
      *buf++ = '-';
      ret++;
    }

    strcpy(buf, inf);
    ret += strlen(inf);
  } else if (isnan(num)) {
    strcpy(buf, nan);
    ret = strlen(nan);
  } else {
    if (sign) {
      num = -num;
      buf[ret++] = '-';
    }

    int_begin = ret;
    ulpart = (uint64_t) num;
    /* round off to @float_digit decimal places */
    dpart = (num - ulpart) * lpow(10, desc->float_digit) + 0.5;

    if (float_digit == INIT_FLOAT_DIGIT) {
      desc->digit = INIT_FLOAT_DIGIT;
    } else {
      desc->digit = desc->float_digit;
    }

    dlen = luout(dpart, &buf[ret], size, desc);


    if (digit == INIT_DIGIT || digit < float_digit ||
        digit - float_digit - 1 < (int) get_digit(ulpart, 10)) {
      desc->digit = INIT_DIGIT;
    } else {
      desc->digit = digit - float_digit - 1;
    }

    len = luout(ulpart, str, size, desc);
    str[len++] = '.';
    /* move float part to right place */
    memmove(&buf[int_begin + len], &buf[int_begin], dlen);
    /* move integer part from str to right place */
    memmove(&buf[int_begin], str, len);

    ret += dlen + len;
  }

  return ret;
}


#endif /* ENABLE_FPU */



/*
 * Formt given print-like string (@fmt) and store the result
 * within at most @size bytes. This version does *NOT* append
 * a NULL to output buffer @buf; it's for internal use only.
 */
int vsnprint(char *buf, int size, const char *fmt, va_list args)
{
  struct format_argdesc desc = {0};
  const char *s;
  char *str;
  long num;
  unsigned long unum;
  int len;
#if defined(ENABLE_FPU)
  double dnum;
#endif /* ENABLE_FPU */

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

    format_assert(*fmt == '%');
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

      len = luout(unum, str, size, &desc);
      break;
#if defined(ENABLE_FPU)

    case FLOAT:
      if (desc.len == LONG) {
        dnum = va_arg(args, double);
      } else {
        dnum = (float) va_arg(args, double);
      }

      len = lfout(dnum, str, size, &desc);
      break;
#endif /* ENABLE_FPU */

    case STRING:
      s = va_arg(args, char *);

      if (!s) {
        s = "<*NULL*>";
      }

      len = strlen(s);
      len = MIN(size, len);
      strncpy(str, s, len);
      break;

    case CHAR:
      *str = (unsigned char) va_arg(args, int);
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

  format_assert(str >= buf);
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

#if CONFIG_ARCH_X86_64

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
static void vga_scrollup(int color)
{
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
      mmio_out32(vga_buffer + 2 * (vga_xpos + vga_ypos * max_xpos),
                 (color << 8) + *buf);
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
  spin_lock(&pbuf_lock);
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

#if !CONFIG_ARCH_SIM

int printf(const char *fmt, ...)
{
  va_list args;
  int n;

  /* NOTE! This will deadlock if the code enclosed
   * by this lock triggered exceptions: the default
   * exception handlers already call printk() */
  spin_lock(&pbuf_lock);

  va_start(args, fmt);
  n = vsnprint(pbuf, sizeof(pbuf), fmt, args);
  va_end(args);
  pbuf[n] = '\0';

  /* TODO: call_sys_write() */
#if CONFIG_ARCH_X86_64
  vga_write(pbuf, n, VGA_DEFAULT_COLOR);
#else
  puts(pbuf);
#endif

  spin_unlock(&pbuf_lock);
  return n;
}

int sprintf(char *str, const char *fmt, ...)
{
  va_list args;
  int n;

  /* NOTE! This will deadlock if the code enclosed
   * by this lock triggered exceptions: the default
   * exception handlers already call printk() */
  spin_lock(&pbuf_lock);

  va_start(args, fmt);
  n = vsnprint(str, sizeof(str), fmt, args);
  va_end(args);
  str[n] = '\0';

  spin_unlock(&pbuf_lock);
  return n;
}

void perror(const char *string)
{
  int len = strlen(string);
  delay(1000);
  strcpy(pbuf, string);
  pbuf[len++] = ':';
  pbuf[len++] = ' ';
  sprintf(&pbuf[len], "%d\n", errno);
#if CONFIG_ARCH_X86_64
#if CONFIG_PRINT2CONSOLE
  vga_write(pbuf, len, VGA_DEFAULT_COLOR);
#elif CONFIG_PRINT2UART
  uart_write(pbuf, len, 0);
#else
#error "Unknown Printk to Output"
#endif
#else
  puts(pbuf);
#endif
}


#endif /* CONFIG_ARCH_SIM */



int print(const char *fmt, ...)
{
  va_list args;
  int n;

  /* NOTE! This will deadlock if the code enclosed
   * by this lock triggered exceptions: the default
   * exception handlers already call printk() */
  spin_lock(&pbuf_lock);

  va_start(args, fmt);
  n = vsnprint(pbuf, sizeof(pbuf), fmt, args);
  va_end(args);
  pbuf[n] = '\0';

  /* TODO: support both kernel and user levels */
#if CONFIG_ARCH_X86_64
  vga_write(pbuf, n, VGA_DEFAULT_COLOR);
#else
  puts(pbuf);
#endif

  spin_unlock(&pbuf_lock);
  return n;
}


/*
 * Kernel print, for VGA and serial outputs
 */
int printk(const char *fmt, ...)
{
  va_list args;
  int n;

  /* NOTE! This will deadlock if the code enclosed
   * by this lock triggered exceptions: the default
   * exception handlers already call printk() */
  spin_lock(&pbuf_lock);

  va_start(args, fmt);
  n = vsnprint(pbuf, sizeof(pbuf), fmt, args);
  va_end(args);
  pbuf[n] = '\0';

#if CONFIG_ARCH_X86_64
#if CONFIG_PRINT2CONSOLE
  vga_write(pbuf, n, VGA_DEFAULT_COLOR);
#elif CONFIG_PRINT2UART
  uart_write(pbuf, n, 0);
#else
#error "Unknown Printk to Output"
#endif
#else
  puts(pbuf);
#endif

  spin_unlock(&pbuf_lock);
  return n;
}


int print_uart(const char *fmt, ...)
{
  va_list args;
  int n;

  spin_lock(&pbuf_lock);

  va_start(args, fmt);
  n = vsnprint(pbuf, sizeof(pbuf), fmt, args);
  va_end(args);
  pbuf[n] = '\0';

#if CONFIG_ARCH_X86_64 || CONFIG_ARCH_AARCH64_RASPI3 || CONFIG_ARCH_AARCH64_SYNQUACER
  uart_write(pbuf, n, 0);
#elif CONFIG_ARCH_SIM || CONFIG_ARCH_AXIS
  puts(pbuf);
#else
#error "Unknown Architecture"
#endif

  spin_unlock(&pbuf_lock);
  return n;
}

