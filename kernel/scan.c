/**
 * @file kernel/scan.c
 *
 * @author Hiroyuki Chishiro
 */

#include <mcube/mcube.h>


static char sbuf[KBUF_SIZE];

#if CONFIG_ARCH_AXIS
spinlock_t sbuf_lock;
#else
spinlock_t sbuf_lock = INIT_SPINLOCK;
#endif /* CONFIG_ARCH_AXIS */


static int get_val(char *dst, const char *src)
{
  const char *p = src;

  while (*src == '\0' || *src == ' ' || *src == '\t' || *src == '\n') {
    src++;
  }

  while (*src != '\0' && *src != ' ' && *src != '\t' && *src != '\n') {
    *dst++ = *src++;
  }

  *dst++ = '\0';
  return src - p;
}


/*
 * Formt given scan-like string (@fmt) and store the result
 * within at most @size bytes. This version does *NOT* append
 * a NULL to output buffer @buf; it's for internal use only.
 */
int vsnscan(const char *buf, int size, const char *fmt, va_list args)
{
  struct format_argdesc desc = {0};
  char *str;
  int len;

  if (size < 1) {
    return 0;
  }

  str = (char *) buf;

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
      len = get_val(sbuf, str);

      if (desc.len == LONG) {
        *va_arg(args, long *) = strtol(sbuf, NULL, 10);
      } else {
        *va_arg(args, int *) = strtol(sbuf, NULL, 10);
      }

      break;

    case UNSIGNED:
      len = get_val(sbuf, str);

      if (desc.len == LONG) {
        *va_arg(args, unsigned long *) = strtoul(sbuf, NULL, 10);
      } else {
        *va_arg(args, unsigned int *) = strtoul(sbuf, NULL, 10);
      }

      break;
#if defined(ENABLE_FPU)

    case FLOAT:
      len = get_val(sbuf, str);

      if (desc.len == LONG) {
        *va_arg(args, double *) = strtod(sbuf, NULL);
      } else {
        *va_arg(args, float *) = strtod(sbuf, NULL);
      }

      break;
#endif /* ENABLE_FPU */

    case STRING:
      len = get_val(va_arg(args, char *), str);
      break;

    case CHAR:
      get_val(sbuf, str);
      *va_arg(args, char *) = *sbuf;
      len = 1;
      break;

    default:
      // fprintf(stderr, "Error: Unknown scan type %d\n", desc.type);
      break;
      /* No-op */
    }

    str += len;
    size -= len;
  }

  format_assert(str >= buf);
  return str - buf;
}


//#if !CONFIG_ARCH_SIM

int sscan(const char *str, const char *fmt, ...)
{
  va_list args;
  int n;

  spin_lock(&sbuf_lock);
  va_start(args, fmt);
  n = vsnscan(str, sizeof(str), fmt, args);
  va_end(args);
  spin_unlock(&sbuf_lock);

  return n;
}



//#endif /* CONFIG_ARCH_SIM */


