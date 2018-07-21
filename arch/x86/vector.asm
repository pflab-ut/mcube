;
; @file arch/x86/vector.asm
;
; @author Hiroyuki Chishiro
;
;#include <mcube/mcube.h>

bits 64

section .text
  extern handle_pit_timer_tick
  global common_interrupt
common_interrupt:
;  jmp common_interrupt
    ; Push a dummy error code.
    push    0

    ; Preserve the first two general-purpose registers.
    push    r15
    push    r14


        ; Preserve the rest of the general-purpose registers.
        push    r13
        push    r12
        push    r11
        push    r10
        push    r9
        push    r8
        push    rbp
        push    rdi
        push    rsi
        push    rdx
        push    rcx
        push    rbx
        push    rax

        ; Preserve the MXCSR register.
;        sub     rsp,    8
;        stmxcsr [rsp]
    .lookup:

        call    handle_pit_timer_tick

    .done:

        ; Restore the MXCSR register.
;        ldmxcsr [rsp]
;        add     rsp,    8

        ; Restore general-purpose registers.
        pop     rax
        pop     rbx
        pop     rcx
        pop     rdx
        pop     rsi
        pop     rdi
        pop     rbp
        pop     r8
        pop     r9
        pop     r10
        pop     r11
        pop     r12
        pop     r13
        pop     r14
        pop     r15
        add     rsp,    16      ; Chop error code and interrupt #

        iretq
