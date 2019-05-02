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

size_t strlen(const char *s)
{
  size_t num = 0;

  while (*s++) {
    num++;
  }

  return num;
}

size_t strnlen(const char *str, int n)
{
  const char *tmp;

  tmp = str;

  while (n) {
    if (*tmp == '\0') {
      break;
    }

    tmp++;
    n--;
  }

  return tmp - str;
}


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

  while (true) {
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

  while ((*dest++ = *src++)) {
  }

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

  while ((*dest++ = *src++)) {
  }

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
    while (*dest) {
      dest++;
    }

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

/*
 * "Note that a REP STOS instruction is the fastest way to
 * initialize a large block of memory." --Intel, vol. 2B
 *
 * To copy the @ch byte repetitively over an 8-byte block,
 * we multiply its value with (0xffffffffffffffff / 0xff).
 */
#if CONFIG_ARCH_X86
void *memset(void *dst, int ch, size_t len)
{
  uint64_t uch;
  uintptr_t d0;

  uch = ch;
  asm volatile(
    "mov  %3, %%rcx;"
    "rep  stosb;"      /* rdi, rcx */
    "mov  %4, %%rcx;"
    "rep  stosq;"      /* ~~~ */
    :"=&D"(d0)
    :"0"(dst), "a"(uch * 0x0101010101010101),
    "ir"(len & 7), "ir"(len >> 3)
    :"rcx", "memory");

  return dst;
}


/*
 * Fill memory with given 4-byte value @val. For easy
 * implementation, @len is vetoed to be a multiple of 8
 */
void *memset32(void *dst, uint32_t val, uint64_t len)
{
  uint64_t uval;
  uintptr_t d0;

  assert((len % 8) == 0);
  len = len / 8;

  uval = ((uint64_t)val << 32) + val;
  asm volatile("rep stosq"      /* rdi, rcx */
               :"=&D"(d0), "+&c"(len)
               :"0"(dst), "a"(uval)
               :"memory");

  return dst;
}

/*
 * Fill memory with given 8-byte value @val. For easy
 * implementation, @len is vetoed to be a multiple of 8
 */
void *memset64(void *dst, uint64_t val, uint64_t len)
{
  uintptr_t d0;

  assert((len % 8) == 0);
  len = len / 8;

  asm volatile("rep stosq"      /* rdi, rcx */
               :"=&D"(d0), "+&c"(len)
               :"0"(dst), "a"(val)
               :"memory");

  return dst;
}

#else

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

/*
 * Fill memory with given 4-byte value @val. For easy
 * implementation, @len is vetoed to be a multiple of 8
 */
void *memset32(void *s, uint32_t c, uint64_t n)
{
  /* NOTE: volatile is required for -O3 option of GCC. */
#if CONFIG_COMPILER_GCC
  volatile uint32_t *xs = s;
#elif CONFIG_COMPILER_CLANG
  uint32_t *xs = s;
#else
#error "Unknown Compiler"
#endif /* CONFIG_COMPILER_GCC */

  while (n--) {
    *xs++ = c;
  }

  return s;
}

/*
 * Fill memory with given 8-byte value @val. For easy
 * implementation, @len is vetoed to be a multiple of 8
 */
void *memset64(void *s, uint64_t c, uint64_t n)
{
  /* NOTE: volatile is required for -O3 option of GCC. */
#if CONFIG_COMPILER_GCC
  volatile uint64_t *xs = s;
#elif CONFIG_COMPILER_CLANG
  uint64_t *xs = s;
#else
#error "Unknown Compiler"
#endif /* CONFIG_COMPILER_GCC */

  while (n--) {
    *xs++ = c;
  }

  return s;
}

#endif /* CONFIG_ARCH_X86 */


#if CONFIG_ARCH_SIM || CONFIG_ARCH_X86


/*
 * NOTE: Always put as much logic as possibe out of the inlined assembly
 * code to the asm-constraints C expressions. This gives the optimizer
 * way more freedom, especially regarding constant propagation.
 */

/*
 * GCC "does not parse the assembler instruction template and does not
 * know what it means or even whether it is valid assembler input." Thus,
 * it needs to be told the list of registers __implicitly__ clobbered by
 * such inlined assembly snippets.
 *
 * A good way to stress-test GCC extended assembler constraints is to
 * always inline the assembler snippets (C99 'static inline'), compile
 * the kernel at -O3 or -Os, and roll!
 *
 * Output registers are (explicitly) clobbered by definition. 'CCR' is
 * the x86's condition code register %rflags.
 */


/*
 * The AMD64 ABI guarantees a DF=0 upon function entry.
 */
static void *__memcpy_forward(void *dst, const void *src, size_t len)
{
  uintptr_t d0;

  asm volatile("mov %3, %%rcx;"
               "rep movsb;"      /* rdi, rsi, rcx */
               "mov %4, %%rcx;"
               "rep movsq;"      /* ~~~ */
               :"=&D"(d0), "+&S"(src)
               :"0"(dst), "ir"(len & 7), "ir"(len >> 3)
               :"rcx", "memory");

  return dst;
}

/*
 * We do tolerate overlapping regions here if src > dst. In
 * such case, (src - dst) must be bigger than movsq's block
 * copy factor: 8 bytes.
 */
void *memcpy_forward(void *dst, const void *src, size_t len)
{
  uintptr_t udst, usrc;
  bool bad_overlap;

  udst = (uintptr_t) dst;
  usrc = (uintptr_t) src;

  bad_overlap = (udst + 8 > usrc) && (usrc + len > udst);

  if (__unlikely(bad_overlap)) {
    panic("%s: badly-overlapped regions, src=0x%lx, dst"
          "=0x%lx, len=0x%lx", __func__, src, dst, len);
  }

  return __memcpy_forward(dst, src, len);
}



/*
 * C99-compliant, with extra sanity checks.
 */
void *memcpy(void *restrict dst, const void *restrict src, size_t len)
{
  uintptr_t udst, usrc;
  bool overlap;

  udst = (uintptr_t) dst;
  usrc = (uintptr_t) src;

  overlap = (udst + len > usrc) && (usrc + len > udst);

  if (__unlikely(overlap)) {
    panic("%s: overlapped regions, src=0x%lx, dst=0x%lx, len=0x%lx",
          __func__, src, dst, len);
  }

  return __memcpy_forward(dst, src, len);
}


/*
 * memcpy(), minus the checks
 *
 * Sanity checks overhead cannot be tolerated for HOT copying
 * paths like screen scrolling.
 *
 * This is also useful for code implicitly called by panic():
 * a sanity check failure there will lead to a stack overflow.
 */

void *memcpy_forward_nocheck(void *dst, const void *src, size_t len)
{
  return __memcpy_forward(dst, src, len);
}

void *memcpy_nocheck(void *restrict dst, const void *restrict src, size_t len)
{
  return __memcpy_forward(dst, src, len);
}

#else


/**
 * The memcpy() function copies @c n bytes from memory area @c src to memory area @c dest.
 * The memory areas must not overlap.
 * @param dest is a pointer of @c void.
 * @param src is a pointer of @c void.
 * @param n is the number fo bytes copying @c src to @c dest.
 * @return Pointer to @c dest.
 */
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
