/**
 * @file arch/arm/boot.S
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
  .section ".text.boot"
ENTRY(_start)
  // Setup the stack with CPU ID.
initialize_stack:  
  get_cpu_id
  mov x1, #STACK_SIZE
  mul x0, x0, x1
  ldr x1, =idle_stack
  add x2, x0, x1
  mov sp, x2
  
initialize_vector_addresses_el3_el2_el1:
  ldr x0, =vector
  msr vbar_el3, x0
  msr vbar_el2, x0
  msr vbar_el1, x0
  
initialize_system_conrol_registers:
  /*
   * hcr_el2, sctlr_el2, and sctlr_el1 do not have architectural reset values.
   * NOTE: there may be other system control registers, which should be initiailized.
   */  
  msr hcr_el2, xzr
  ldr x1, =0x30c50838
  msr sctlr_el2, x1
  msr sctlr_el1, x1

initialize_mmu_and_cache: 
  bl configure_mmu_and_cache
  
  
enable_neon_and_fp:
  /* disable trapping of accessing in EL3 and EL2. */
  msr cptr_el3, xzr
  msr cptr_el3, xzr
  /* disable access trapping in EL1 and EL0. */
  mov x1, #(0x3 << 20) // FPEN disables trapping to EL1.
  msr cpacr_el1, x1
  isb

change_el3_to_el2:  
  // Initialize SCTLR_EL2 and HCR_EL2 to save values before entering EL2.
  msr sctlr_el2, xzr
  msr hcr_el2, xzr
  // Determine the EL2 Execution state.
  mrs x0, scr_el3
  orr x0, x0, #(1<<10) // RW EL2 Execution state is AArch64.
  orr x0, x0, #(1<<0) // NS EL1 is Non-secure world.
  msr scr_el3, x0
  mov x0, #0b01001 // DAIF=0000
  msr spsr_el3, x0 // M[4:0]=01001 EL2h must match SCR_EL3.RW

  // Determine EL2 entry.
  adr x0, change_el2_to_el1 // el2_entry points to the first instruction of
  msr elr_el3, x0 // EL2 code.
  eret
  
change_el2_to_el1:
  // Initialize the SCTLR_EL1 register before entering EL1.
  msr sctlr_el1, xzr
  // Determine the EL1 Execution state.
  mrs x0, hcr_el2
  orr x0, x0, #(1<<31) // RW=1 EL1 Execution state is AArch64.
  msr hcr_el2, x0
  mov x0, #0b00101 // DAIF=0000
  msr spsr_el2, x0 // M[4:0]=00101 EL1h must match HCR_EL2.RW.
  adr x0, start_el1 // el1_entry points to the first instruction of
  msr elr_el2, x0 // EL1 code.
  eret

#if 0
change_el1_to_el0:
  // Determine the EL0 Execution state.
  mov x0, #0b00000 // DAIF=0000 M[4:0]=00000 EL0t.
  msr spsr_el1, x0
  adr x0, el0_entry // el1_entry points to the first instruction of
  msr elr_el1, x0 // EL0 code.
  eret
el0_entry:  
#endif
  
start_el1:
  // set up stack in el1
  get_cpu_id
  mov x1, #STACK_SIZE
  mul x0, x0, x1
  ldr x1, =idle_stack
  add x2, x0, x1
  mov sp, x2
  
clear_bss:  
  cbnz x0, begin_main
  adr     x0, __bss_begin
  adr     x1, __bss_end
  sub     x1, x1, x0
  bl      memzero
begin_main: 
  bl      main
  b       hang               // should never come here

ENTRY(hang)
  disable_irq
  wfi
  b       hang
  
