/**
 * @file arch/axis/boot.S
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
  
  .section  .text
  .globl  __begin
  .ent    __begin
  .globl kernel_stack
__begin:
ENTRY(_start)
  .set   noreorder
  la    $gp, _gp
  mfs $t0, $1 // get cluster and cpu ids
  and $t0, $t0, 0x7 // get cpu id
//  la    $sp, THREAD_STACK_ADDR(0, 0)
  //  la    $sp, (MEMORY_BASE + MEMORY_SIZE)
  // $sp = kernel_stack + cpu_id($t0) * STACK_SIZE
  la $sp, kernel_stack
  addi $t0, $t0, 1
  muli $t1, $t0, KERNEL_STACK_SIZE
  add    $sp, $sp, $t1

  addi  $t0, $0, 'Z'
  mts   $31, $t0
  
__jump_to_main:
  la    $t0, main
  jalr  $ra, $t0
  nop

__end:
  j    __end
  nop
  .set reorder
  .end  __begin
