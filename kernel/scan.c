/**
 * @file kernel/scan.c
 *
 * @author Hiroyuki Chishiro
 */

#include <mcube/mcube.h>

/**
 * @var sbuf[KBUF_SIZE]
 * @brief Scan buffer.
 */
static char sbuf[KBUF_SIZE];

#if CONFIG_ARCH_AXIS
spinlock_t sbuf_lock;
#else
spinlock_t sbuf_lock = INIT_SPINLOCK;
#endif /* CONFIG_ARCH_AXIS */

/**
 * @fn static int get_val(char *dst, const char *src, int n)
 * @brief get value.
 *
 * @param dst Destination.
 * @param src Source string.
 * @param n Number.
 * @return Value.
 */
static int get_val(char *dst, const char *src, int n)
{
  int i = 0;

  while (*src == '\0' || *src == ' ' || *src == '\t' || *src == '\n') {
    src++;
  }

  while (*src != '\0' && *src != ' ' && *src != '\t' && *src != '\n') {
    if (n == INIT_DIGIT || (n > 0 && i < n)) {
      dst[i++] = *src++;
    } else {
      break;
    }
  }

  dst[i] = '\0';
  return i;
}


int vsnscan(const char *str, const char *fmt, va_list args)
{
  struct format_argdesc desc = {0};
  char *p;
  int num = 0;
  int len;

  p = (char *) str;

  while (*fmt) {
    while (*fmt != '\0' && *fmt != '%') {
      p++;
      fmt++;
    }

    /* Mission complete */
    if (*fmt == '\0') {
      break;
    }

    len = 0;
    format_assert(*fmt == '%');
    fmt = parse_arg(fmt, &desc);

    switch (desc.type) {
    case SIGNED:
      len += get_val(sbuf, p, desc.digit);
      num++;

      if (desc.len == LONG) {
        *va_arg(args, long *) = strtol(sbuf, NULL, 10);
      } else {
        *va_arg(args, int *) = strtol(sbuf, NULL, 10);
      }

      break;

    case UNSIGNED:
      len += get_val(sbuf, p, desc.digit);
      num++;

      if (desc.len == LONG) {
        *va_arg(args, unsigned long *) = strtoul(sbuf, NULL, 10);
      } else {
        *va_arg(args, unsigned int *) = strtoul(sbuf, NULL, 10);
      }

      break;
#if defined(ENABLE_FPU)

    case FLOAT:
      len += get_val(sbuf, p, desc.digit);
      num++;

      if (desc.len == LONG) {
        *va_arg(args, double *) = strtod(sbuf, NULL);
      } else {
        *va_arg(args, float *) = strtod(sbuf, NULL);
      }

      break;
#endif /* ENABLE_FPU */

    case STRING:
      len += get_val(va_arg(args, char *), p, desc.digit);
      num++;
      break;

    case CHAR:
      len += get_val(sbuf, p, 1);
      num++;
      *va_arg(args, char *) = *sbuf;
      break;

    default:
      // fprintf(stderr, "Error: Unknown scan type %d\n", desc.type);
      break;
      /* No-op */
    }

    p += len;
  }

  format_assert(p >= str);
  return num;
}


int sscan(const char *str, const char *fmt, ...)
{
  va_list args;
  int n;

  spin_lock(&sbuf_lock);
  va_start(args, fmt);
  n = vsnscan(str, fmt, args);
  va_end(args);
  spin_unlock(&sbuf_lock);

  return n;
}

#if !CONFIG_ARCH_SIM

int sscanf(const char *str, const char *fmt, ...)
{
  va_list args;
  int n;

  spin_lock(&sbuf_lock);
  va_start(args, fmt);
  n = vsnscan(str, fmt, args);
  va_end(args);
  spin_unlock(&sbuf_lock);

  return n;
}

#endif /* !CONFIG_ARCH_SIM */
