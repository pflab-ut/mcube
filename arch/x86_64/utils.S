/**
 * @file arch/x86_64/utils.S
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
 movabs  $syscall_table, %rbx
// movq    %rcx, %rbx
 shlq    $3, %rax         /* 8-byte per pointer */
 addq    %rax, %rbx
 cmpq    $0, (%rbx)
 je      1f
 movq    %r10, %rcx       /* Replace the 4th argument with %r10 */
 callq   *(%rbx)
1:
 popq    %rbx
 popq    %r11
 popq    %rcx
 popq    %rbp
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

//.extern CPU_DATA_BASE
 
ENTRY(change_ring0_to_ring3)
 /* Setup SP0 in TSS */
 movq $0xffffffff80280000UL, %rax
// movq $CPU_DATA_BASE, %rdx
 movq %rax, 4(%rdx)
 /* Prepare stack for iretq */
 movq %rsp, %rdx
 movq %rdi, %rax
 addq $8, %rax
 pushq %rax
 pushq %rdx
 movq $0x202, %rax
 pushq %rax
 pushq %rdi
 movabs $1f, %rax
 pushq %rax
 iretq
1:
 ret


 /* void lgdt(void *gdtr, uint64_t selector) */
ENTRY(lgdt)
 lgdt    (%rdi)
 /* Reload GDT */
 pushq   %rsi
 movabs  $1f,%rax
 pushq   %rax    /* Just to do ret */
 lretq
1:
 /* Set data selector */
 movq    %rsi,%rax
 addq    $8,%rax
 movq    %rax,%ds
 movq    %rax,%es
 movq    %rax,%ss
 ret
 
