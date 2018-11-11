/**
 * @file arch/arm/utils.S
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
ENTRY(memzero)
	str xzr, [x0], #8
	subs x1, x1, #8
	b.gt memzero
	ret

ENTRY(get_el)
  mrs x0, CurrentEL
  lsr x0, x0, #2
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

ENTRY(move_to_user_level)
  mov x29, x0 // save first argument (x0) to unused register (x29)
  get_cpu_id x28, x27 // save CPU ID to x28
  // Determine the EL0 Execution state.
  mov x0, #0b00000 // DAIF=0000 M[4:0]=00000 EL0t.
  msr spsr_el1, x0
  adr x0, el0_entry // el1_entry points to the first instruction of
  msr elr_el1, x0 // EL0 code.
  eret
el0_entry:
  mov x0, x28 // move CPU ID (x28) to x0
  mov x1, #STACK_SIZE
  mul x0, x0, x1
  ldr x1, =idle_stack
  add x2, x0, x1
  mov sp, x2
  blr x29
//  bl user_mode_main
  b . // infinite loop

