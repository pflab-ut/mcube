;
; @file arch/x86/utils.asm
;
; @author Hiroyuki Chishiro
;
;=============================================================================
; @file     utils.asm
; @brief    Fill a region of memory with zeroes.
;
;=============================================================================

bits 64

section .text

  global cpu_switch_to
  
cpu_switch_to:
