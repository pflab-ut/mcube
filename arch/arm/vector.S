/**
 * @file arch/arm/vector.S
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

.macro invalid_entry type
  kernel_entry
  mov x0, #\type
  mrs x1, esr_el1
  mrs x2, elr_el1
  bl show_invalid_entry_message
  b err_hang
.endm
  
.macro vector_entry label
  b \label
  .balign 128
.endm

.macro kernel_entry
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
  str x30, [sp, #16 * 15]
.endm

.macro kernel_exit
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
  ldr x30, [sp, #16 * 15]
  add sp, sp, #S_FRAME_SIZE
  eret
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
  invalid_entry SYNC_INVALID_EL1t
  
vector_irq_el1t:
  kernel_entry
  bl do_IRQ
  kernel_exit
  
vector_fiq_el1t:
  invalid_entry FIQ_INVALID_EL1t
  
vector_error_el1t:
  invalid_entry ERROR_INVALID_EL1t
  
vector_sync_el1h:
  invalid_entry SYNC_INVALID_EL1h
  
vector_irq_el1h:
  kernel_entry
  bl do_IRQ
  kernel_exit

vector_fiq_el1h:
  invalid_entry FIQ_INVALID_EL1h
  
vector_error_el1h:
  invalid_entry ERROR_INVALID_EL1h
  
vector_sync_el0_64:
  invalid_entry SYNC_INVALID_EL0_64
  
vector_irq_el0_64:
  invalid_entry IRQ_INVALID_EL0_64

vector_fiq_el0_64:
  invalid_entry FIQ_INVALID_EL0_64
  
vector_error_el0_64:
  invalid_entry ERROR_INVALID_EL0_64
  
vector_sync_el0_32:
  invalid_entry SYNC_INVALID_EL0_32
  
vector_irq_el0_32:
  invalid_entry IRQ_INVALID_EL0_32

vector_fiq_el0_32:
  invalid_entry FIQ_INVALID_EL0_32
  
vector_error_el0_32:
  invalid_entry ERROR_INVALID_EL0_32

ENTRY(ret_from_fork)
  bl schedule_tail
  mov x0, x20
  blr x19 // should never return

ENTRY(err_hang)
  b err_hang
