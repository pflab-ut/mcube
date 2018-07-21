/**
 * @file kernel/printk.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


static inline int iout(int d, int base, char *dst, int n, struct conv_flag *cf)
{
	int i;
	char tmp[MAX_DIGIT];
	unsigned int ud;
	char pad = ' ';
	if (base == 10 && d < 0) {
		dst[n++] = '-';
		ud = d * -1;
	} else {
		ud = d;
	}

	for (i = 1; i < MAX_DIGIT; i++) {
		tmp[i] = ud % base;
		if (tmp[i] < 10) {
			tmp[i] += '0';
		} else {
			tmp[i] += 'a' - 10;
		}

		if ((ud /= base) == 0) {

			if (cf->pad) {
				pad = '0';
			}
			if (FOUT_SZ < n + cf->digit) {
				return -1;
			}
			while (i < cf->digit) {
				dst[n++] = pad;
				cf->digit--;
			}

			if (FOUT_SZ < n + i) {
				return -1;
			}
			while (i) {
				dst[n++] = tmp[i--];
			}
			break;
		}
	}

	return n;
}



static inline int ldout(long ld, int base, char *dst, int n, struct conv_flag *cf)
{
	int i;
	char tmp[MAX_DIGIT];
	unsigned long ul;
	char pad = ' ';

	if (base == 10 && ld < 0) {
		dst[n++] = '-';
		ul = ld * -1;
	} else {
		ul = ld;
	}

	for (i = 1; i < MAX_DIGIT; i++) {
		tmp[i] = ul % base;
		if (tmp[i] < 10) {
			tmp[i] += '0';
		} else {
			tmp[i] += 'a' - 10;
		}

		if ((ul /= base) == 0) {

			if (cf->pad) {
				pad = '0';
			}
			if (FOUT_SZ < n + cf->digit) {
				return -1;
			}
			while (i < cf->digit) {
				dst[n++] = pad;
				cf->digit--;
			}

			if (FOUT_SZ < n + i) {
				return -1;
			}
			while (i) {
				dst[n++] = tmp[i--];
			}
			break;
		}
	}

	return n;
}

static inline unsigned int uout(unsigned int u, int base, char *dst, int n, struct conv_flag *cf)
{
	int i;
	char tmp[MAX_DIGIT];
	char pad = ' ';

	for (i = 1; i < MAX_DIGIT; i++) {
		tmp[i] = u % base;
		if (tmp[i] < 10) {
			tmp[i] += '0';
		} else {
			tmp[i] += 'a' - 10;
		}

		if ((u /= base) == 0) {

			if (cf->pad) {
				pad = '0';
			}
			if (FOUT_SZ < n + cf->digit) {
				return -1;
			}
			while (i < cf->digit) {
				dst[n++] = pad;
				cf->digit--;
			}

			if (FOUT_SZ < n + i) {
				return -1;
			}
			while (i) {
				dst[n++] = tmp[i--];
			}
			break;
		}
	}

	return n;
}


static inline unsigned int luout(unsigned long lu, int base, char *dst, int n, struct conv_flag *cf)
{
	int i;
	char tmp[MAX_DIGIT];
	char pad = ' ';

	for (i = 1; i < MAX_DIGIT; i++) {
		tmp[i] = lu % base;
		if (tmp[i] < 10) {
			tmp[i] += '0';
		} else {
			tmp[i] += 'a' - 10;
		}
    
		if ((lu /= base) == 0) {

			if (cf->pad) {
				pad = '0';
			}
			if (FOUT_SZ < n + cf->digit) {
				return -1;
			}
			while (i < cf->digit) {
				dst[n++] = pad;
				cf->digit--;
			}

			if (FOUT_SZ < n + i) {
				return -1;
			}
			while (i) {
				dst[n++] = tmp[i--];
			}
			break;
		}
	}

	return n;
}


#if !CONFIG_ARCH_AXIS
static inline int fout(float f, int base, char *dst, int n, struct conv_flag *cf)
{
	int i;
	char tmp[MAX_DIGIT];
	char pad = ' ';
	float uf;
	uint32_t u32;
	if (base == 10 && f < 0) {
		dst[n++] = '-';
		uf = f * -1;
	} else {
		uf = f;
	}

	u32 = uf;
	for (i = 1; i < MAX_DIGIT; i++) {
		tmp[i] = u32 % base;
		if (tmp[i] < 10) {
			tmp[i] += '0';
		} else {
			tmp[i] += 'a' - 10;
		}

		if ((u32 /= base) == 0) {

			if (cf->pad) {
				pad = '0';
			}
			if (FOUT_SZ < n + cf->digit) {
				return -1;
			}
			while (i < cf->digit) {
				dst[n++] = pad;
				cf->digit--;
			}

			if (FOUT_SZ < n + i) {
				return -1;
			}
			while (i) {
				dst[n++] = tmp[i--];
			}
			break;
		}
	}
	dst[n++] = '.';

	for (i = 0, uf = (uf - (uint32_t) uf) * 10; i < 6; i++) {
    dst[n++] = (uint32_t) uf % 10 + '0';
    uf = uf * 10 - (uint32_t) uf * 10;
  }

	return n;
}

static inline int lfout(double lf, int base, char *dst, int n, struct conv_flag *cf)
{
	int i;
	char tmp[MAX_DIGIT];
	char pad = ' ';
	double ulf;
	uint64_t u64;
	if (base == 10 && lf < 0) {
		dst[n++] = '-';
		ulf = lf * -1;
	} else {
		ulf = lf;
	}
	u64 = ulf;
	for (i = 1; i < MAX_DIGIT; i++) {
		tmp[i] = u64 % base;
		if (tmp[i] < 10) {
			tmp[i] += '0';
		} else {
			tmp[i] += 'a' - 10;
		}

		if ((u64 /= base) == 0) {

			if (cf->pad) {
				pad = '0';
			}
			if (FOUT_SZ < n + cf->digit) {
				return -1;
			}
			while (i < cf->digit) {
				dst[n++] = pad;
				cf->digit--;
			}

			if (FOUT_SZ < n + i) {
				return -1;
			}
			while (i) {
				dst[n++] = tmp[i--];
			}
			break;
		}
	}
	dst[n++] = '.';

	for (i = 0, ulf = (ulf - (uint64_t) ulf) * 10; i < 6; i++) {
    dst[n++] = (uint64_t) ulf % 10 + '0';
    ulf = ulf * 10 - (uint64_t) ulf * 10;
  }

	return n;
}
#endif /* !CONFIG_ARCH_AXIS */



/**
 * The printk() function produces output according to @b CONSOLE or @b UART.
 * @param fmt specifies how subsequent arguments.
 * @return Number of characters printed.
 */
int printk(const char *fmt, ...)
{
	int n = 0;
  int ret;
	char buf[FOUT_SZ];
	va_list ap;
	int d;
	unsigned int u;
	char *p, *s;
	struct conv_flag cf;
	unsigned long lu;
	long ld;
#if !CONFIG_ARCH_AXIS
  float f;
  double lf;
#endif /* !CONFIG_ARCH_AXIS */
  
	va_start(ap, fmt);

	while (*fmt) {
		if (*fmt != '%') {
			p = (char *) fmt + 1;
			while (*p && *p != '%') {
				p++;
			}
			if (FOUT_SZ < n + (int) (p - fmt)) {
				goto out;
			}
			while (fmt != p) {
				buf[n++] = *fmt++;
			}
			if (!*p) {
				break;
			}
		}
		fmt++;

		/* at least one */
		if (FOUT_SZ < n + 1) {
			goto out;
		}
		cf = (struct conv_flag) {.pad = FALSE, .digit = 0};
skip:
		switch (*fmt++) {
		case 'l':
			switch (*fmt++) {
			case 'd': /* dec long */
				ld = va_arg(ap, long);
				ret = ldout(ld, 10, buf, n, &cf);
				if (ret == -1) {
					goto out;
				}
				n = ret;
				break;
			case 'u': /* dec unsigned long */
				lu = va_arg(ap, unsigned long);
				ret = luout(lu, 10, buf, n, &cf);
				if (ret == -1) {
					goto out;
				}
				n = ret;
				break;
			case 'b':	/* bin unsigned long */
				lu = va_arg(ap, unsigned long);
				ret = luout(lu, 2, buf, n, &cf);
				if (ret == -1) {
					goto out;
				}
				n = ret;
				break;
#if !CONFIG_ARCH_AXIS
      case 'f': /* double */
        lf = va_arg(ap, double);
        ret = lfout(lf, 10, buf, n, &cf);
        if (ret == -1) {
          goto out;
        }
        n = ret;
        break;
#endif /* !CONFIG_ARCH_AXIS */
			case 'x':	/* hex unsigned long */
				lu = va_arg(ap, unsigned long);
				ret = luout(lu, 16, buf, n, &cf);
				if (ret == -1) {
					goto out;
				}
				n = ret;
				break;
			}
			break;
		case 's':	/* string */
			s = va_arg(ap, char *);
			while (*s) {
				if (FOUT_SZ < n) {
					goto out;
				}
				buf[n++] = *s++;
			}
			break;
		case 'd':	/* dec int */
			d = va_arg(ap, int);
			ret = iout(d, 10, buf, n, &cf);
			if (ret == -1) {
				goto out;
			}
			n = ret;
			break;
		case 'u':	/* dec unsigned int */
			u = va_arg(ap, unsigned int);
			ret = uout(u, 10, buf, n, &cf);
			if (ret == -1) {
				goto out;
			}
			n = ret;
			break;
		case 'b':	/* bin unsigned int */
			u = va_arg(ap, unsigned int);
			ret = uout(u, 2, buf, n, &cf);
			if (ret == -1) {
				goto out;
			}
			n = ret;
			break;
#if !CONFIG_ARCH_AXIS
    case 'f': /* float */
      f = (float) va_arg(ap, double);
      ret = fout(f, 10, buf, n, &cf);
      if (ret == -1) {
        goto out;
      }
      n = ret;
      break;
#endif /* !CONFIG_ARCH_AXIS */
		case 'x':	/* hex unsigned int */
			u = va_arg(ap, unsigned int);
			ret = uout(u, 16, buf, n, &cf);
			if (ret == -1) {
				goto out;
			}
			n = ret;
			break;
		case 'c':	/* char */
			buf[n++] = (char) va_arg(ap, int);
			break;
		case '0':
			cf.pad = TRUE;
			fmt++;
      FALLTHROUGH;
		case '1' ... '8':
			cf.digit = *(fmt - 1) - '0';
		default:
			goto skip;
		}
	}

out:
	va_end(ap);
  buf[n] = '\0';
  
#if CONFIG_PRINTK2CONSOLE
  console_write(buf, n, NULL);
#elif CONFIG_PRINTK2PRINTF
  fprintf(stderr, "%s", buf);
#elif CONFIG_PRINTK2UART
	uart_write(buf, n, NULL);
#else
#error "Unknown Printk to Output"
#endif
	return n;
}

