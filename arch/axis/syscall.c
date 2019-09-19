/**
 * @file arch/axis/syscall.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

/*
 * NOTE: system call is not implemented in AXIS architecture because it has only kernel level.
 * This implementation exists for compatability for other architectures.
 */

int call_sys_get_mode_level(void)
{
  return KERNEL_LEVEL;
}

int call_sys_write(char *buf)
{
  return puts(buf);
}
