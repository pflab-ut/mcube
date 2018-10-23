/**
 * @file arch/arm/vector.S
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

.globl call_sys_write
call_sys_write:
  mov w8, #SYS_write
  svc #0
  ret
