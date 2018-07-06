/**
 * @file arch/arm/boot.S
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
  .section ".text.boot"
ENTRY(_start)
  // Setup the stack with CPU ID.
  // Stack Value is 0x00800000 - CPU_ID * STACK_SIZE
initialize_stack:  
  get_cpu_id
  mov x1, #STACK_SIZE
  mul x0, x0, x1
  mov sp, #STACK_ADDR
  sub sp, sp, x0
initialize_vector_addresses_el3_el2_el1:
  ldr x0, =vector
  msr vbar_el3, x0
  msr vbar_el2, x0
  msr vbar_el1, x0
  
initialize_system_conrol_registers:
  /* hcr_el2, sctlr_el2, and sctlr_el1 do not have architectural reset values. */
  msr hcr_el2, xzr
  ldr x1, =0x30c50838
  msr sctlr_el2, x1
  msr sctlr_el1, x1
enable_neon_and_fp:
  /* disable trapping of accessing in EL3 and EL2. */
  msr cptr_el3, xzr
  msr cptr_el3, xzr
  /* disable access trapping in EL1 and EL0. */
  mov x1, #(0x3 << 20) // FPEN disables trapping to EL1.
  msr cpacr_el1, x1
  isb
change_el3_to_el2:
  // EL3 to EL2.
  mov   x2, #0x5b1    // RW=1, HCE=1, SMD=1, RES=1, NS=1
  msr   scr_el3, x2
  mov   x2, #0x3c9    // D=1, A=1, I=1, F=1 M=EL2h
  msr   spsr_el3, x2
  adr   x2, change_el2_to_el1
  msr   elr_el3, x2
  eret

change_el2_to_el1:
  // set sp in EL1.
  msr   sp_el1, x1
  // enable AArch64 in EL1.
  mov   x0, #(1 << 31)      // AArch64
  orr   x0, x0, #(1 << 1)   // SWIO hardwired on Pi3
  msr   hcr_el2, x0
  mrs   x0, hcr_el2
  // set vector address in EL1.
//  ldr x0, =vector
//  msr vbar_el1, x0
  // change execution level to EL1.
  mov   x2, #0x3c4         // D=1, A=1, I=1, F=1 M=EL1t
  msr   spsr_el2, x2
  adr   x2, start_el1
  msr   elr_el2, x2
  eret
start_el1:  
  get_cpu_id
  mov x1, #STACK_SIZE
  mul x0, x0, x1
  mov sp, #STACK_ADDR
  sub sp, sp, x0
clear_bss:  
  cbnz x0, begin_main
  adr     x0, __bss_begin
  adr     x1, __bss_end
  sub     x1, x1, x0
  bl      memzero
begin_main: 
  bl      kernel_main
  b       hang               // should never come here

ENTRY(hang)
  disable_interrupt
  wfi
  b       hang
  
