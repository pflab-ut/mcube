/**
 * @file arch/axis/utils.S
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

  
ENTRY(arch_switch_to)
  addi $t0, $0, OFFSET_OF_CPU_CONTEXT
  /* set first argument with offset */
  add $t1, $a0, $t0
  /* save stack pointer */
  move $t2, $sp
  /* store callee-saved registers */
  sw $ra, 52($t1)
  sw $fp, 48($t1)
  sw $sp, 44($t1)
  sw $gp, 40($t1)
  sw $k1, 36($t1)
  sw $k0, 32($t1)
  sw $s7, 28($t1)
  sw $s6, 24($t1)
  sw $s5, 20($t1)
  sw $s4, 16($t1)
  sw $s3, 12($t1)
  sw $s2, 8($t1)
  sw $s1, 4($t1)
  sw $s0, 0($t1)
  /* set second argument with offset */
  add $t1, $a1, $t0
  /* restore callee-saved registers */
  lw $s0, 0($t1)
  lw $s1, 4($t1)
  lw $s2, 8($t1)
  lw $s3, 12($t1)
  lw $s4, 16($t1)
  lw $s5, 20($t1)
  lw $s6, 24($t1)
  lw $s7, 28($t1)
  lw $k0, 32($t1)
  lw $k1, 36($t1)
  lw $gp, 40($t1)
  lw $sp, 44($t1)
  lw $fp, 48($t1)
  lw $ra, 52($t1)
  move $sp, $t2
  jr $ra
