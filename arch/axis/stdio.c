/**
 * @file arch/axis/stdio.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

int putchar(int c)
{
  asm volatile("mts $31, %0" :: "r"(c));
  return c;
}

int puts(const char *s)
{
  const char *t = s;
  while (*t) {
    putchar(*t++);
  }
  return t - s;
}
