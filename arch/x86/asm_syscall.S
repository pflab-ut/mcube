/**
 * @file arch/x86/asm_syscall.S
 *
 * @author Hiroyuki Chishiro
 */

#include <mcube/mcube.h>

.code64
.text

 /* Entry point to the syscall */
ENTRY(syscall_entry)
 /* N.B., rip and rflags are stored in rcx and r11, respectively. */
 pushq   %rbp
 movq    %rsp, %rbp
 pushq   %rcx
 pushq   %r11
 pushq   %rbx

 /* Check the max number of the syscall table */
 movabs  $NR_SYSCALLS, %rbx
 cmpq    %rbx, %rax
 jge     1f

 /* Lookup the system call table and call the corresponding to %rax */
 movabs  $syscall_table, %rcx
 movq    (%rcx), %rbx
 shlq    $3, %rax         /* 8-byte per pointer */
 addq    %rax, %rbx
 cmpq    $0, (%rbx)
 je      1f
 movq    %r10, %rcx       /* Replace the 4th argument with %r10 */
// callq   *(%rbx)
 callq   %rbx
1:
 popq    %rbx
 popq    %r11
 popq    %rcx
 popq    %rbp
_hlt: 
 hlt
 jmp _hlt
 sysretq

 /* void syscall_setup(void *, uint64_t) */
ENTRY(syscall_setup)
 pushq   %rbx
 movabs  $syscall_table, %rbx
 movq    %rdi,(%rbx)
 movabs  $NR_SYSCALLS, %rbx
 movq    %rsi,(%rbx)
 popq    %rbx
 ret
 
 /* int syscall(arg0, ..., arg5) */
ENTRY(syscall)
 pushq   %rbp
 movq    %rdi, %rax
 movq    %rsi, %rdi
 movq    %rdx, %rsi
 movq    %rcx, %rdx
 movq    %r8, %r10
 movq    %r9, %r8
 movq    -8(%rsp), %r9
 syscall
 popq    %rbp
 ret
 
