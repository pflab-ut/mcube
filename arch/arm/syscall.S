/**
 * @file arch/arm/vector.S
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

ENTRY(call_sys_sched)
  mov w8, #SYS_sched
  svc #0
  ret

ENTRY(call_sys_end_job)
  mov w8, #SYS_end_job
  svc #0
  ret
  
ENTRY(call_sys_get_exec_time)
  mov w8, #SYS_get_exec_time
  svc #0
  ret
  
ENTRY(call_sys_write)
  mov w8, #SYS_write
  svc #0
  ret

ENTRY(call_sys_get_cpu_id)
  mov w8, #SYS_get_cpu_id
  svc #0
  ret
  
ENTRY(call_sys_get_mode_level)
  mov w8, #SYS_get_mode_level
  svc #0
  ret
  
