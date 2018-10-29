;
; @file arch/x86/utils.asm
;
; @author Hiroyuki Chishiro
;
;=============================================================================
; @file     memzero.asm
; @brief    Fill a region of memory with zeroes.
;
; Copyright 2016 Brett Vickers.
; Use of this source code is governed by a BSD-style license that can
; be found in the MonkOS LICENSE file.
;=============================================================================

bits 64

section .text

  global memzero
  global syscall0
  global syscall1
  global syscall2
  global move_to_user_level

;-----------------------------------------------------------------------------
; @function     memzero
; @brief        Fill a region of memory with zeroes.
; @reg[in]      rdi     Address of the destination memory area.
; @reg[in]      rsi     Number of bytes to set to zero.
; @reg[out]     rax     Destination address.
; @killedregs   r8, rcx
;-----------------------------------------------------------------------------
memzero:

    ; Preserve the original destination address.
    mov     r8,     rdi

    ; The value to store is zero.
    xor     rax,    rax

    ; Do a byte-by-byte store.
    mov     rcx,    rsi
    rep     stosb

    ; Return the original destination address.
    mov     rax,    r8
    ret


syscall0:
  int 0x40 ; SYSCALL_IRQ
  ret

syscall1:
  int 0x40 ; SYSCALL_IRQ
  ret

syscall2:
  int 0x40 ; SYSCALL_IRQ
  ret

  
move_to_user_level:
  ; TODO: implement
  ret
