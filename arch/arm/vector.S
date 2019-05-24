/**
 * @file arch/arm/vector.S
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
  
.macro invalid_entry el, type
  kernel_entry \el
  mov x0, #\type
  mrs x1, esr_el1
  mrs x2, elr_el1
  mov x3, sp
  bl show_invalid_entry_message
  b err_hang
.endm
  
.macro vector_entry label
  b \label
  .balign 128
.endm

.macro kernel_entry, el
  disable_local_irq
  sub sp, sp, #S_FRAME_SIZE
  stp x0, x1, [sp, #16 * 0]
  stp x2, x3, [sp, #16 * 1]
  stp x4, x5, [sp, #16 * 2]
  stp x6, x7, [sp, #16 * 3]
  stp x8, x9, [sp, #16 * 4]
  stp x10, x11, [sp, #16 * 5]
  stp x12, x13, [sp, #16 * 6]
  stp x14, x15, [sp, #16 * 7]
  stp x16, x17, [sp, #16 * 8]
  stp x18, x19, [sp, #16 * 9]
  stp x20, x21, [sp, #16 * 10]
  stp x22, x23, [sp, #16 * 11]
  stp x24, x25, [sp, #16 * 12]
  stp x26, x27, [sp, #16 * 13]
  stp x28, x29, [sp, #16 * 14]
//  str x30, [sp, #16 * 15]

#if 1
  .if     \el == 0
  mrs     x21, sp_el0
  .else
  add     x21, sp, #S_FRAME_SIZE
  .endif /* \el == 0 */
#endif
  
  mrs     x22, elr_el1 // save exception link register
  mrs     x23, spsr_el1 // save saved program status register

  stp     x30, x21, [sp, #16 * 15]
  stp     x22, x23, [sp, #16 * 16]
  /* set the first argument to sp for accessing full registers */
.endm

.macro kernel_exit el, type
  ldp     x22, x23, [sp, #16 * 16]
  ldp     x30, x21, [sp, #16 * 15]

#if 1
  .if \el == 0
  msr     sp_el0, x21
  .endif /* \el == 0 */
#endif
//  mov x21, sp
//  msr sp_el0, x21
  
  msr     elr_el1, x22 // restore exception link register
  msr     spsr_el1, x23 // restore saved program status register
  
  ldp x0, x1, [sp, #16 * 0]
  ldp x2, x3, [sp, #16 * 1]
  ldp x4, x5, [sp, #16 * 2]
  ldp x6, x7, [sp, #16 * 3]
  ldp x8, x9, [sp, #16 * 4]
  ldp x10, x11, [sp, #16 * 5]
  ldp x12, x13, [sp, #16 * 6]
  ldp x14, x15, [sp, #16 * 7]
  ldp x16, x17, [sp, #16 * 8]
  ldp x18, x19, [sp, #16 * 9]
  ldp x20, x21, [sp, #16 * 10]
  ldp x22, x23, [sp, #16 * 11]
  ldp x24, x25, [sp, #16 * 12]
  ldp x26, x27, [sp, #16 * 13]
  ldp x28, x29, [sp, #16 * 14]
//  ldr x30, [sp, #16 * 15]
  add sp, sp, #S_FRAME_SIZE
  enable_local_irq
  .if \type == 0
  eret
  .else
//  invalid_entry 1, 1
//  eret
  // NOTE: 112 means margin of ret against eret
  //  add sp, sp, 112
//  add sp, sp, -32
  ret
  .endif
.endm
    
.balign 2048
ENTRY(vector)
  /* 0x000 Synchronous EL1t */
  vector_entry vector_sync_el1t
  /* 0x080 IRQ/vIRQ EL1t */
  vector_entry vector_irq_el1t
  /* 0x100 FIQ/vFIQ EL1t */
  vector_entry vector_fiq_el1t
  /* 0x180 SError/vSError EL1t */
  vector_entry vector_error_el1t
  /* 0x200 Synchronous EL1h */
  vector_entry vector_sync_el1h
  /* 0x280 IRQ/vIRQ EL1h */
  vector_entry vector_irq_el1h
  /* 0x300 FIQ/vFIQ EL1h */
  vector_entry vector_fiq_el1h
  /* 0x380 SError/vSError EL1h */
  vector_entry vector_error_el1h
  /* 0x400 Synchronous EL0 64-bit */
  vector_entry vector_sync_el0_64
  /* 0x480 IRQ/vIRQ EL0 64-bit */
  vector_entry vector_irq_el0_64
  /* 0x500 FIQ/vFIQ EL0 64-bit */
  vector_entry vector_fiq_el0_64
  /* 0x580 SError/vSError EL0 64-bit */
  vector_entry vector_error_el0_64
  /* 0x600 Synchronous EL0 32-bit */
  vector_entry vector_sync_el0_32
  /* 0x680 IRQ/vIRQ EL0 32-bit */
  vector_entry vector_irq_el0_32
  /* 0x700 FIQ/vFIQ EL0 32-bit */
  vector_entry vector_fiq_el0_32
  /* 0x780 SError/vSError EL0 32-bit */
  vector_entry vector_error_el0_32

vector_sync_el1t:
  kernel_entry 1
  invalid_entry 1, SYNC_INVALID_EL1t
  
vector_irq_el1t:
  kernel_entry 1
  mov     x0, sp
  bl do_irq
  kernel_exit 1, 0
  
vector_fiq_el1t:
  kernel_entry 1
  invalid_entry 1, FIQ_INVALID_EL1t
  
vector_error_el1t:
  kernel_entry 1
  invalid_entry 1, ERROR_INVALID_EL1t
  
vector_sync_el1h:
//  invalid_entry 0, SYNC_INVALID_EL0_64
  kernel_entry 0
  mrs     x25, esr_el1                            // read the syndrome register
  lsr     x24, x25, #ESR_ELx_EC_SHIFT             // exception class
  cmp     x24, #ESR_ELx_EC_SVC_INSTRUCTION_EXCEPTION64  // SVC in 64-bit state
  b.eq    el1h_svc
  invalid_entry 0, SYNC_ERROR

  sc_nr   .req    x25                                     // number of system calls
  scno    .req    x26                                     // syscall number
  stbl    .req    x27                                     // syscall table pointer

el1h_svc:
  adr     stbl, syscall_table                    // load syscall table pointer
  uxtw    scno, w8                                // syscall number in w8
  mov     sc_nr, #NR_SYSCALLS
//  enable_local_irq
  cmp     scno, sc_nr                             // check upper syscall limit
  b.hs    el1h_ni_sys

  ldr     x16, [stbl, scno, lsl #3]               // address in the syscall table
  blr     x16                                     // call sys_* routine
  b       el1h_ret_from_syscall
el1h_ni_sys:
  invalid_entry 0, SYSCALL_ERROR
el1h_ret_from_syscall:
//  disable_local_irq
  str     x0, [sp, #S_X0]                         // returned x0
  kernel_exit 0, 0
  
vector_irq_el1h:
  kernel_entry 1
  mov     x0, sp
  bl do_irq
  kernel_exit 1, 0

vector_fiq_el1h:
  invalid_entry 1, FIQ_INVALID_EL1h
  
vector_error_el1h:
  invalid_entry 1, ERROR_INVALID_EL1h
  
vector_sync_el0_64:
  kernel_entry 0
  mrs     x25, esr_el1                            // read the syndrome register
  lsr     x24, x25, #ESR_ELx_EC_SHIFT             // exception class
  cmp     x24, #ESR_ELx_EC_SVC_INSTRUCTION_EXCEPTION64  // SVC in 64-bit state
  b.eq    el0_svc
  invalid_entry 0, SYNC_ERROR

  sc_nr   .req    x25                                     // number of system calls
  scno    .req    x26                                     // syscall number
  stbl    .req    x27                                     // syscall table pointer

el0_svc:  
  adr     stbl, syscall_table                    // load syscall table pointer
  uxtw    scno, w8                                // syscall number in w8
  mov     sc_nr, #NR_SYSCALLS
//  enable_local_irq
  cmp     scno, sc_nr                             // check upper syscall limit
  b.hs    el0_ni_sys

  ldr     x16, [stbl, scno, lsl #3]               // address in the syscall table
  blr     x16                                     // call sys_* routine
  b       el0_ret_from_syscall
el0_ni_sys: 
  invalid_entry 0, SYSCALL_ERROR
el0_ret_from_syscall:
//  disable_local_irq
  str     x0, [sp, #S_X0]                         // returned x0
  cmp scno, #SYS_move_to_kernel_level
  b.eq el0_ret
  kernel_exit 0, 0
el0_ret:
  kernel_exit 0, 1
    
vector_irq_el0_64:
  kernel_entry 0
  invalid_entry 0, IRQ_INVALID_EL0_64

vector_fiq_el0_64:
  kernel_entry 0
  invalid_entry 0, FIQ_INVALID_EL0_64
  
vector_error_el0_64:
  kernel_entry 0
  invalid_entry 0, ERROR_INVALID_EL0_64
  
vector_sync_el0_32:
  kernel_entry 0
  invalid_entry 0, SYNC_INVALID_EL0_32
  
vector_irq_el0_32:
  kernel_entry 0
  invalid_entry 0, IRQ_INVALID_EL0_32

vector_fiq_el0_32:
  kernel_entry 0
  invalid_entry 0, FIQ_INVALID_EL0_32
  
vector_error_el0_32:
  kernel_entry 0
  invalid_entry 0, ERROR_INVALID_EL0_32

ENTRY(ret_from_fork)
  bl schedule_tail
  mov x0, x20
  blr x19 // should never return

ENTRY(err_hang)
  b err_hang

