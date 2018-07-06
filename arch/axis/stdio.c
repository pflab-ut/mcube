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

ssize_t console_write(const void *input_data, size_t length, void *devdata)
{
  size_t i;
  for (i = 0; i < length; i++) {
    putchar(((char *) input_data)[i]);
  }
  return 0;
}
