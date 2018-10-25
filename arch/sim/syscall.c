/**
 * @file arch/sim/syscall.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

int call_sys_write(char *buf)
{
  return fputs(buf, stderr);
}
