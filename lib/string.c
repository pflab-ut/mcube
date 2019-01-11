/**
 * @file lib/string.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM

/**
 * The memcmp() function compares the first @c n bytes (each interpreted as unsigned char)
 * of the memory areas @c s1 and @c s2.
 * @param s1 is a pointer of @c char.
 * @param s2 is a pointer of @c char.
 * @param n is the number of bytes comparing @c s1 to @c s2.
 * @return Integer less than, equal to, or greater than zero if the first @c n bytes of @c s1 is found,
 * respectively, to be less than, to match, or be greater than the first @c n bytes of s2.@n
 * For a nonzero return value, the sign is determined by the sign of the difference between the first
 * pair of bytes (interpreted as @c unsigned @c char) that differ in @c s1 and @c s2.
 */
#if 0
int memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for (su1 = s1, su2 = s2; 0 < n; ++su1, ++su2, n--) {
		if ((res = *su1 - *su2) != 0) {
			break;
		}
	}
	return res;
}
#endif


/**
 * The memmove() function copies @c n bytes from memory area @c src to memory area @c dest.
 * The memory areas may overlap: copying takes place as though the bytes in @c src are first
 * copied into a temporary array that does not overlap @c src or @c dest,
 * and the bytes are then copied from the temporary array to @c dest.
 * @param dest is a pointer of @c void.
 * @param src is a pointer of @c void.
 * @param n is the number fo bytes copying @c src to @c dest.
 * @return Pointer to @c dest.
 */
void *memmove(void *dest, const void *src, size_t n)
{
	char *tmp;
	const char *s;

	if (dest <= src) {
		tmp = dest;
		s = src;
		while (n--) {
			*tmp++ = *s++;
		}
	} else {
		tmp = dest;
		tmp += n;
		s = src;
		s += n;
		while (n--) {
			*--tmp = *--s;
		}
	}
	return dest;
}

/**
 * The strlen() function calculates the length of the string @c s,
 * excluding the terminating null byte('\0').
 * @param s is a pointer of @c char.
 * @return Number of bytes in the string @c s.
 */
#if 0
size_t strlen(const char *s)
{
	size_t num = 0;
	while (*s++) {
		num++;
	}
	return num;
}
#endif

/**
 * The strcmp() function compares the two strings @c s1 and @c s2.
 * @param s1 is a pointer of @c char.
 * @param s2 is a pointer of @c char.
 * @return Integer less than, equal to, or greater than zero
 * if @c s1 (or the first @c n bytes thereof) is found, respectively,
 * to be less than, to match, or be greater than @c s2.
 */
int strcmp(const char *s1, const char *s2)
{
	int res;
	while (1) {
		if ((res = *s1 - *s2++) != 0 || !*s1++) {
			break;
		}
	}
	return res;
}


/**
 * The strncmp() function is similar to the strcmp() function, except
 * it compares the only first (at most) @c n bytes of @c s1 and @c s2.
 * @param s1 is a pointer of @c char.
 * @param s2 is a pointer of @c char.
 * @param n is the number of bytes comparing @a s1 to @a s2.
 * @return Integer less than, equal to, or greater than zero
 * if @c s1 (or the first @c n bytes thereof) is found, respectively,
 * to be less than, to match, or be greater than @c s2.
 */
#if 0
int strncmp(const char *s1, const char *s2, size_t n)
{
	signed char res = 0;

	while (n) {
		if ((res = *s1 - *s2++) != 0 || !*s1++) {
			break;
		}
		n--;
	}
	return res;
}
#endif

/**
 * The strcpy() function copies the string pointed to by @c src, including the terminating null byte
 * ('\0'), to the buffer pointed to by @c dest.
 * The strings may not overlap, and the destination string @c dest must be large enough
 * to receive the copy.
 * @param dest is a pointer of @c char.
 * @param src is a pointer of @c char.
 * @return Pointer to the destination string @c dest.
 */
char *strcpy(char *dest, const char *src)
{
  char *tmp = dest;

  while ((*dest++ = *src++))
		;
  return tmp;
}

/**
 * The strncpy() function is similar to the strcpy() function, except that
 * at most @c n bytes of @c src are copied.
 * @warning If there is no null byte among the first @c n bytes of @c src,
 * the string placed in @c dest will  not  be  null-terminated.
 * @param dest is a pointer of @c char.
 * @param src is a pointer of @c char.
 * @param n is the number of bytes copying @c src to @c dest.
 * @return Pointer to the destination string @c dest.
 */
#if 0
char *strncpy(char *dest, const char *src, size_t n)
{
	char *tmp = dest;

	while (n) {
		if ((*tmp = *src) != 0) {
			src++;
		}
		tmp++;
		n--;
	}
	return dest;
}
#endif


/**
 * The strcat() function appends the @c src string to the @c dest string, overwriting the terminating null
 * byte ('\0') at the end of @c dest, and then adds a terminating null byte.
 * The strings may not overlap, and the @c dest string must have enough space for the result.
 * If @c dest is not large enough, program behavior is unpredictable.
 * @param dest is a pointer of @c char.
 * @param src is a pointer of @c char.
 * @return Pointer to the destination string @c dest.
 */
char *strcat(char *dest, const char *src)
{
  char *tmp = dest;
  
  while (*dest) {
    dest++;
	}
  while ((*dest++ = *src++))
    ;
  return tmp;
}

/**
 * The strncat() function is similar to the strcat() function, except that
 * it will use at most n bytes from src; and
 * src does not need to be null-terminated if it contains @c n or more bytes.
 * @param dest is a pointer of @c char.
 * @param src is a pointer of @c char.
 * @param n is the number of bytes appending @c src to @c dest.
 * @return Pointer to the destination string @c dest.
 */
char *strncat(char *dest, const char *src, size_t n)
{
	char *tmp = dest;

	if (n) {
		while (*dest)
			dest++;
		while ((*dest++ = *src++) != 0) {
			if (--n == 0) {
				*dest = '\0';
				break;
			}
		}
	}
	return tmp;
}


/**
 * The strchr() function returns a pointer to the first occurrence of the character @c c
 * in the string @c s.
 * @param s is a pointer of @c char.
 * @param c is a character where it is located.
 * @return Pointer to the matched characture or NULL if the character is not found.
 */
char *strchr(const char *s, int c)
{
	for (; *s != (char) c; s++) {
		if (*s == '\0') {
			return NULL;
		}
	}
	return (char *) s;
}



/* implicit call by compiler */

/**
 * The memset() function fills the first @c n bytes of the memory area pointed to
 * by @c s with the constant byte @c c (8-bit value).
 * @param s is a pointer of @c void.
 * @param c is an integer to fill the memory area.
 * @param n is the number of bytes filled by @c c.
 * @return Pointer to the memory area @c s.
 */
#if 0
void *memset(void *s, int c, size_t n)
{
  /* NOTE: volatile is required for -O3 option of GCC. */
#if CONFIG_COMPILER_GCC
  volatile uint8_t *xs = s;
#elif CONFIG_COMPILER_CLANG
  uint8_t *xs = s;
#else
#error "Unknown Compiler"
#endif /* CONFIG_COMPILER_GCC */
  while (n--) {
    *xs++ = c;
  }
  return s;
}
#endif

/**
 * The memsetw() function fills the first @c n bytes of the memory area pointed to
 * by @c s with the constant byte @c c (16-bit value).
 * @param s is a pointer of @c void.
 * @param c is an integer to fill the memory area.
 * @param n is the number of bytes filled by @c c.
 * @return Pointer to the memory area @c s.
 */
void *memsetw(void *s, int c, size_t n)
{
  /* NOTE: volatile is required for -O3 option of GCC. */
#if CONFIG_COMPILER_GCC
  volatile uint16_t *xs = s;
#elif CONFIG_COMPILER_CLANG
  uint16_t *xs = s;
#else
#error "Unknown Compiler"
#endif /* CONFIG_COMPILER_GCC */
  /* mask lower 1-bit */
  n &= ~0x1;
  while (n) {
    *xs++ = c;
    n -= 2;
  }
  return s;

}

/**
 * The memsetd() function fills the first @c n bytes of the memory area pointed to
 * by @c s with the constant byte @c c (32-bit value).
 * @param s is a pointer of @c void.
 * @param c is an integer to fill the memory area.
 * @param n is the number of bytes filled by @c c.
 * @return Pointer to the memory area @c s.
 */
void *memsetd(void *s, int c, size_t n)
{
  /* NOTE: volatile is required for -O3 option of GCC. */
#if CONFIG_COMPILER_GCC
  volatile uint32_t *xs = s;
#elif CONFIG_COMPILER_CLANG
  uint32_t *xs = s;
#else
#error "Unknown Compiler"
#endif /* CONFIG_COMPILER_GCC */
  /* mask lower 2-bit */
  n &= ~0x3;
  while (n) {
    *xs++ = c;
    n -= 4;
  }
  return s;
}

/**
 * The memcpy() function copies @c n bytes from memory area @c src to memory area @c dest.
 * The memory areas must not overlap.
 * @param dest is a pointer of @c void.
 * @param src is a pointer of @c void.
 * @param n is the number fo bytes copying @c src to @c dest.
 * @return Pointer to @c dest.
 */
#if 0
void *memcpy(void *dest, const void *src, size_t n)
{
  char *d = (char *) dest;
	char *s = (char *) src;

  while (n--) {
    *d++ = *s++;
  }
  return dest;
}
#endif


/**
 * The strtok() function breaks a string into a sequence of zero or more nonempty tokens.
 * On the first call to strtok() the string to be parsed should be specified in @c str.
 * In each subsequent call that should parse the same string, str must be NULL.
 * @param str is a pointer of @c char.
 * @param delim specifies a set of bytes that delimit the tokens in the parsed string.
 * The caller may specify different strings in delim in successive calls that parse the same string.
 * @return Pointer to @c dest.
 */
char *strtok(char *str, const char *delim)
{
	register char *spanp;
	register int c, sc;
	char *tok;
	static char *last;

	if (!str && !(str = last)) {
		return NULL;
	}
	/*
	 * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
	 */
 cont:
	c = *str++;
	for (spanp = (char *) delim; (sc = *spanp++) != 0;) {
		if (c == sc) {
			goto cont;
		}
	}

	if (c == 0) {/* no non-delimiter characters */
		last = NULL;
		return NULL;
	}
	tok = str - 1;

	/*
	 * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
	 * Note that delim must have one NUL; we stop if we see that, too.
	 */
	for (;;) {
		c = *str++;
		spanp = (char *) delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0) {
					str = NULL;
				} else {
					str[-1] = 0;
				}
				last = str;
				return tok;
			}
		} while (sc != 0);
	}
	/* not reached */
}

#endif /* !CONFIG_ARCH_SIM */
