/**
 * @file arch/sim/stdio.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>



ssize_t console_write(const void *input_data, size_t length, void *devdata)
{
  fprintf(stderr, "%s", input_data);
  return length;
}
