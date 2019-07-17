/**
 * @file lib/string.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if !CONFIG_ARCH_SIM


int memcmp(const void *s1, const void *s2, size_t n)
{
  const unsigned char *su1, *su2;
  int res = 0;

  for (su1 = (unsigned char *) s1, su2 = (unsigned char *) s2;
       0 < n;
       ++su1, ++su2, n--) {
    if ((res = *su1 - *su2) != 0) {
      break;
    }
  }

  return res;
}


void *memmove(void *dst, const void *src, size_t n)
{
  char *tmp;
  const char *s;

  if (dst <= src) {
    tmp = (char *) dst;
    s = (char *) src;

    while (n--) {
      *tmp++ = *s++;
    }
  } else {
    tmp = (char *) dst;
    tmp += n;
    s = (char *) src;
    s += n;

    while (n--) {
      *--tmp = *--s;
    }
  }

  return dst;
}

size_t strlen(const char *s)
{
  size_t num = 0;

  while (*s++) {
    num++;
  }

  return num;
}

size_t strnlen(const char *s, size_t n)
{
  const char *tmp;

  tmp = s;

  while (n) {
    if (*tmp == '\0') {
      break;
    }

    tmp++;
    n--;
  }

  return tmp - s;
}


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



char *strcpy(char *dst, const char *src)
{
  char *tmp = dst;

  while ((*dst++ = *src++)) {
  }

  return tmp;
}

char *strncpy(char *dst, const char *src, size_t n)
{
  char *tmp = dst;

  while (n) {
    if ((*tmp = *src) != 0) {
      src++;
    }

    tmp++;
    n--;
  }

  return dst;
}


char *strcat(char *dst, const char *src)
{
  char *tmp = dst;

  while (*dst) {
    dst++;
  }

  while ((*dst++ = *src++)) {
  }

  return tmp;
}

char *strncat(char *dst, const char *src, size_t n)
{
  char *tmp = dst;

  if (n) {
    while (*dst) {
      dst++;
    }

    while ((*dst++ = *src++) != 0) {
      if (--n == 0) {
        *dst = '\0';
        break;
      }
    }
  }

  return tmp;
}


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


/*
 * "Note that a REP STOS instruction is the fastest way to
 * initialize a large block of memory." --Intel, vol. 2B
 *
 * To copy the @a c byte repetitively over an 8-byte block,
 * we multiply its value with (0xffffffffffffffff / 0xff).
 */
#if CONFIG_ARCH_X86_64
void *memset(void *s, int c, size_t n)
{
  uint64_t uch;
  uintptr_t d0;

  uch = c;
  asm volatile(
    "mov  %3, %%rcx;"
    "rep  stosb;"      /* rdi, rcx */
    "mov  %4, %%rcx;"
    "rep  stosq;"      /* ~~~ */
    :"=&D"(d0)
    :"0"(s), "a"(uch * 0x0101010101010101),
    "ir"(n & 7), "ir"(n >> 3)
    :"rcx", "memory");

  return s;
}


/*
 * Fill memory with given 4-byte value @a c. For easy
 * implementation, @a n is vetoed to be a multiple of 8
 */
void *memset32(void *s, uint32_t c, size_t n)
{
  uint32_t uc;
  uintptr_t d0;

  assert((n % 8) == 0);
  n = n / 8;

  uc = ((uint64_t) c << 32) + c;
  asm volatile("rep stosq"      /* rdi, rcx */
               :"=&D"(d0), "+&c"(n)
               :"0"(s), "a"(uc)
               :"memory");

  return s;
}

/*
 * Fill memory with given 8-byte value @a c. For easy
 * implementation, @a n is vetoed to be a multiple of 8
 */
void *memset64(void *s, uint64_t c, size_t n)
{
  uintptr_t d0;

  assert((n % 8) == 0);
  n = n / 8;

  asm volatile("rep stosq"      /* rdi, rcx */
               :"=&D"(d0), "+&c"(n)
               :"0"(s), "a"(c)
               :"memory");

  return s;
}

#else

void *memset(void *s, int c, size_t n)
{
  /* NOTE: volatile is required for -O3 option of GCC. */
#if CONFIG_COMPILER_GCC
  volatile uint8_t *xs = (uint8_t *) s;
#elif CONFIG_COMPILER_CLANG
  uint8_t *xs = (uint8_t *) s;
#else
#error "Unknown Compiler"
#endif /* CONFIG_COMPILER_GCC */

  while (n--) {
    *xs++ = c;
  }

  return s;
}

/*
 * Fill memory with given 4-byte value @a val. For easy
 * implementation, @a n is vetoed to be a multiple of 8
 */
void *memset32(void *s, uint32_t c, size_t n)
{
  /* NOTE: volatile is required for -O3 option of GCC. */
#if CONFIG_COMPILER_GCC
  volatile uint32_t *xs = (uint32_t *) s;
#elif CONFIG_COMPILER_CLANG
  uint32_t *xs = (uint32_t *) s;
#else
#error "Unknown Compiler"
#endif /* CONFIG_COMPILER_GCC */

  while (n--) {
    *xs++ = c;
  }

  return s;
}

/*
 * Fill memory with given 8-byte value @a val. For easy
 * implementation, @a n is vetoed to be a multiple of 8
 */
void *memset64(void *s, uint64_t c, size_t n)
{
  /* NOTE: volatile is required for -O3 option of GCC. */
#if CONFIG_COMPILER_GCC
  volatile uint64_t *xs = (uint64_t *) s;
#elif CONFIG_COMPILER_CLANG
  uint64_t *xs = (uint64_t *) s;
#else
#error "Unknown Compiler"
#endif /* CONFIG_COMPILER_GCC */

  while (n--) {
    *xs++ = c;
  }

  return s;
}

#endif /* CONFIG_ARCH_X86_64 */


#if CONFIG_ARCH_SIM || CONFIG_ARCH_X86_64


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
void *memcpy(void *dst, const void *src, size_t len)
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


void *memcpy_forward_nocheck(void *dst, const void *src, size_t len)
{
  return __memcpy_forward(dst, src, len);
}

void *memcpy_nocheck(void *dst, const void *src, size_t len)
{
  return __memcpy_forward(dst, src, len);
}

#else


void *memcpy(void *dst, const void *src, size_t n)
{
  char *d = (char *) dst;
  char *s = (char *) src;

  while (n--) {
    *d++ = *s++;
  }

  return dst;
}


#endif


char *strtok(char *str, const char *delim)
{
  char *spanp;
  int c, sc;
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

