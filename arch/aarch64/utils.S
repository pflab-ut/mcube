/**
 * @file arch/aarch64/utils.S
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

ENTRY(memzero)
  str xzr, [x0], #8
  subs x1, x1, #8
  b.gt memzero
  ret

  
ENTRY(arch_switch_to)
  mov     x10, #OFFSET_OF_CPU_CONTEXT
  /* set first argument with offset */
  add     x8, x0, x10
  /* save stack pointer */
  mov     x9, sp
  /* store callee-saved registers */
  stp     x19, x20, [x8], #16
  stp     x21, x22, [x8], #16
  stp     x23, x24, [x8], #16
  stp     x25, x26, [x8], #16
  stp     x27, x28, [x8], #16
  stp     x29, x9, [x8], #16
  str     x30, [x8]
  /* set second argument with offset */
  add     x8, x1, x10
  /* restore callee-saved registers */
  ldp     x19, x20, [x8], #16
  ldp     x21, x22, [x8], #16
  ldp     x23, x24, [x8], #16
  ldp     x25, x26, [x8], #16
  ldp     x27, x28, [x8], #16
  ldp     x29, x9, [x8], #16
  ldr     x30, [x8]
  /* resume stack pointer */
  mov     sp, x9
  ret

#define STACK_OFFSET 48
  
ENTRY(move_to_user_level)
  sub sp, sp, #STACK_OFFSET
  stp x25, x26, [sp, #16 * 0]
  stp x27, x28, [sp, #16 * 1]
  stp x29, x30, [sp, #16 * 2]
  mov x26, x30 // save ret addr
  mov x25, sp // save sp

  // NOTE: add stack pointer of EL1 to change eret to ret
  add sp, sp, #STACK_OFFSET

  msr sp_el0, x25
  
  get_cpu_id x28, x27 // save CPU ID to x28
  // Determine the EL0 Execution state.
  mov x29, #0b00000 // DAIF=0000 M[4:0]=00000 EL0t.
  msr spsr_el1, x29
  adr x29, el0_entry // el1_entry points to the first instruction of
  msr elr_el1, x29 // EL0 code.
  eret
el0_entry:
  mov x30, x26 // resume ret addr
  mov sp, x25 // resume sp
  ldp x25, x26, [sp, #16 * 0]
  ldp x27, x28, [sp, #16 * 1]
  ldp x29, x30, [sp, #16 * 2]
  add sp, sp, #STACK_OFFSET
  
  ret
