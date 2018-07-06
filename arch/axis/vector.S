/**
 * @file arch/axis/vector.S
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

ENTRY(common_interrupt)
  /* save general purpose registers. */
  .set noat
  addi $sp, $sp, -STACK_FRAME_SIZE
  sw $ra, 124($sp)
  sw $fp, 120($sp)
  sw $sp, 116($sp)
  sw $gp, 112($sp)
  sw $k1, 108($sp)
  sw $k0, 104($sp)
  sw $t9, 100($sp)
  sw $t8, 96($sp)
  sw $s7, 92($sp)
  sw $s6, 88($sp)
  sw $s5, 84($sp)
  sw $s4, 80($sp)
  sw $s3, 76($sp)
  sw $s2, 72($sp)
  sw $s1, 68($sp)
  sw $s0, 64($sp)
  sw $t7, 60($sp)
  sw $t6, 56($sp)
  sw $t5, 52($sp)
  sw $t4, 48($sp)
  sw $t3, 44($sp)
  sw $t2, 40($sp)
  sw $t1, 36($sp)
  sw $t0, 32($sp)
  sw $a3, 28($sp)
  sw $a2, 24($sp)
  sw $a1, 20($sp)
  sw $a0, 16($sp)
  sw $v1, 12($sp)
  sw $v0, 8($sp)
  sw $at, 4($sp)
  .set at
  jal do_IRQ
  /* restore general purpose registers. */
  .set noat
  lw $at, 4($sp)
  lw $v0, 8($sp)
  lw $v1, 12($sp)
  lw $a0, 16($sp)
  lw $a1, 20($sp)
  lw $a2, 24($sp)
  lw $a3, 28($sp)
  lw $t0, 32($sp)
  lw $t1, 36($sp)
  lw $t2, 40($sp)
  lw $t3, 44($sp)
  lw $t4, 48($sp)
  lw $t5, 52($sp)
  lw $t6, 56($sp)
  lw $t7, 60($sp)
  lw $s0, 64($sp)
  lw $s1, 68($sp)
  lw $s2, 72($sp)
  lw $s3, 76($sp)
  lw $s4, 80($sp)
  lw $s5, 84($sp)
  lw $s6, 88($sp)
  lw $s7, 92($sp)
  lw $t8, 96($sp)
  lw $t9, 100($sp)
  lw $k0, 104($sp)
  lw $k1, 108($sp)
  lw $gp, 112($sp)
  lw $sp, 116($sp)
  lw $fp, 120($sp)
  lw $ra, 124($sp)
  addi   $sp, $sp, STACK_FRAME_SIZE
  .set at
  ert
  
