/**
 * @file arch/x86/asm.S
 *
 * @author Hiroyuki Chishiro
 */
/*_
 * Copyright (c) 2018 Hirochika Asai <asai@jar.jp>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <mcube/mcube.h>

	.globl kentry
	.globl hlt
	.globl sti
	.globl cli
	.globl pause
	.globl kmemset
	.globl kmemcmp
	.globl kmemcpy
	.globl cpuid
	.globl rdtsc
	.globl rdmsr
	.globl wrmsr
	.globl mfrd32
	.globl mfwr32
	.globl inb
	.globl inw
	.globl inl
	.globl outb
	.globl outw
	.globl outl
	.globl lgdt
	.globl sgdt
	.globl lidt
	.globl sidt
	.globl lldt
	.globl ltr
	.globl chcs
	.globl task_restart
	.globl intr_null
	.globl intr_apic_loc_tmr
	.globl intr_gpf
	.globl intr_irq1
	.globl intr_crash
	.globl asm_ioapic_map_intr

	/* Code segment */
	.text
	.code64

/* Entry point */
kentry:
	cli
	call bsp_start
1:
	hlt
	jmp 1b

/* void hlt(void) */
hlt:
	hlt
	ret

/* void hlt(void) */
sti:
	sti
	ret

/* void cli(void) */
cli:
	cli
	ret

/* void pause(void) */
pause:
	pause
	ret

/* void *kmemset(void *b, int c, size_t len) */
kmemset:
  pushq	%rdi
	movl %esi, %eax /* c */
	movq %rdx, %rcx /* len */
	cld /* Ensure the DF cleared */
	rep stosb /* Set %al to (%rdi)-(%rdi+%rcx) */
	popq %rdi
	movq %rdi, %rax /* Restore for the return value */
	ret

/* int kmemcmp(void *s1, void *s2, size_t n) */
kmemcmp:
	xorq %rax, %rax
	movq %rdx, %rcx /* n */
	cld /* Ensure the DF cleared */
	repe cmpsb /* Compare byte at (%rsi) with byte at (%rdi) */
	jz 1f
	decq %rdi /* rollback one */
	decq %rsi /* rollback one */
	movb (%rdi), %al /* *s1 */
	subb (%rsi), %al /* *s1 - *s2 */
1:
	ret

/* int kmemcpy(void *__restrict dst, void *__restrict src, size_t n) */
kmemcpy:
	movq %rdi, %rax /* Return value */
	movq %rdx, %rcx /* n */
	cld /* Ensure the DF cleared */
	rep movsb /* Copy byte at (%rsi) to (%rdi) */
	ret

/* uint64_t cpuid(uint64_t rax, uint64_t *rbx, uint64_t *rcx, uint64_t *rdx) */
cpuid:
	pushq %rbx
	movq %rdi, %rax
	movq %rdx, %rdi
	movq %rcx, %r8
	cpuid
	movq %rbx, (%rsi)
	movq %rcx, (%rdi)
	movq %rdx, (%r8)
	popq %rbx
	ret
  
/* uint64_t rdtsc(void) */
rdtsc:
	xorq %rax,%rax
	movq %rax,%rdx
	rdtscp
	shlq $32,%rdx
	addq %rdx,%rax
	ret


/* uint64_t rdmsr(uint64_t reg) */
rdmsr:
	movq %rdi,%rcx
	rdmsr
	shlq $32,%rdx
	addq %rdx,%rax
	ret

/* void wrmsr(uint64_t reg, uint64_t data) */
wrmsr:
	movq %rdi,%rcx
	movq %rsi,%rax
	movq %rax,%rdx
	shrq $32,%rdx
	wrmsr
	ret

/* uint32_t mfrd32(uintptr_t addr) */
mfrd32:
	mfence
	movl (%rdi),%eax
	ret

/* void mfwr32(uintptr_t addr, uint32_t data) */
mfwr32:
	mfence
	movl %esi,(%rdi)
	ret

/* uint8_t inb(uint16_t port) */
inb:
	movw %di,%dx
	xorq %rax,%rax
	inb	%dx,%al
	ret

/* uint16_t inw(uint16_t port) */
inw:
	movw %di,%dx
	xorq %rax,%rax
	inw %dx,%ax
	ret

/* uint32_t inl(uint16_t port) */
inl:
	movw %di,%dx
	xorq %rax,%rax
	inl %dx,%eax
	ret

/* void outb(uint16_t port, uint8_t value) */
outb:
	movw %di,%dx
	movb %sil,%al
	outb %al,%dx
	ret

/* void outw(uint16_t port, uint16_t value) */
outw:
	movw %di,%dx
	movw %si,%ax
	outw %ax,%dx
	ret

/* void outl(uint16_t port, uint32_t value) */
outl:
	movw %di,%dx
	movl %esi,%eax
	outl %eax,%dx
	ret

/* void lgdt(void *gdtr, uint64_t selector) */
lgdt:
	lgdt (%rdi)
	/* Reload GDT */
	pushq %rsi
	movabs $1f,%rax
	pushq %rax	/* Just to do ret */
	lretq
1:
	/* Set data selector */
	movq %rsi,%rax
	addq $8,%rax
	movq %rax,%ds
	movq %rax,%es
	movq %rax,%ss
	ret

/* void sgdt(void *gdtr) */
sgdt:
	sgdt (%rdi)
	ret

/* void lidt(void *idtr) */
lidt:
	lidt (%rdi)
	ret

/* void sidt(void *idtr) */
sidt:
	sidt (%rdi)
	ret

/* void lldt(uint16_t) */
lldt:
	lldt %di
	ret

/* void ltr(uint16_t) */
ltr:
	ltr %di
	ret

/* void chcs(uint64_t selector) */
chcs:
	/* Setup SP0 in TSS */
	movq $0x00010000, %rax
	movq $0x00078000, %rdx
	movq %rax, TSS_SP0(%rdx)
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

/* Restart a task */
task_restart:
	/* Task base address (struct arch_task *) */
	movq $0xc0068e00, %rbp
	/* If the next task is not scheduled, immediately restart this task. */
	cmpq $0, 8(%rbp)	/* next_task */
	jz 2f
	movq 8(%rbp), %rax
	/* If the current task is null, then do not need to save anything. */
	cmpq $0, 0(%rbp)	/* cur_task */
	jz 1f
	/* Save the stack pointer (restart point) */
	movq 0(%rbp), %rax
	movq %rsp, 0(%rax)	/* cur_task->rp */
1:
	/* Notify that the current task is switched (to the kernel) */
	movq 0(%rbp), %rdi
	movq 8(%rbp), %rsi
	/* Task switch (set the stack frame of the new task) */
	movq 8(%rbp), %rax	/* next_task */
	movq %rax, 0(%rbp)	/* cur_task */
	movq 0(%rax), %rsp	/* next_task->rp */
	movq $0, 8(%rbp)	/* next_task */
	/* Setup sp0 in TSS */
	movq 0(%rbp), %rax	/* cur_task */
	movq 8(%rax), %rdx	/* cur_task->sp0 */
	movq $0xc0078000, %rax	/* TSS */
	movq %rdx, TSS_SP0(%rax)
2:
	/* Pop all registers from the stackframe */
	popw %gs
	popw %fs
	popq %rbp
	popq %rdi
	popq %rsi
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax
	iretq


/* Null interrupt handler (do nothing) */
intr_null:
	iretq

/* Timer interrupt of Local APIC */
intr_apic_loc_tmr:
	/* Push all registers to the stackframe */
	pushq	%rax
	pushq	%rbx
	pushq	%rcx
	pushq	%rdx
	pushq	%r8
	pushq	%r9
	pushq	%r10
	pushq	%r11
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15
	pushq	%rsi
	pushq	%rdi
	pushq	%rbp
	pushw	%fs
	pushw	%gs
	/* Call a function */
	call ksignal_clock
	/* APIC EOI */
	movq $MSR_APIC_BASE, %rcx
	rdmsr	/* Read APIC info to [%edx:%eax]; N.B., higer */
				/* 32 bits of %rax and %rdx are cleared */
				/* bit [35:12]: APIC Base, [11]: EN */
				/* [10]: EXTD, and [8]:BSP */
	shlq $32, %rdx
	addq %rax, %rdx
	andq $0xfffffffffffff000, %rdx /* APIC Base */
	movl $0, 0x0b0(%rdx) /* EOI */
	jmp	task_restart

/* General protection fault */
intr_gpf:
	/* Save registers*/
	pushq %rbp
	movq %rsp, %rbp
	pushq %rax
	pushq %rdi
	pushq %rsi
	pushq %rdx
	pushq %rcx
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %rbx
	/* Call isr_exception_werror() */
	movq $13, %rdi
	movq 8(%rbp), %rsi	/* error code */
	movq 16(%rbp), %rdx	/* rip */
	movq 24(%rbp), %rcx	/* cs */
	movq 32(%rbp), %r8	/* rflags */
	movq 40(%rbp), %r9	/* rsp */
	/* 48(%rbp): ss */
	call isr_exception_werror
	/* Restore registers */
	popq %rbx
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rcx
	popq %rdx
	popq %rsi
	popq %rdi
	popq %rax
	popq %rbp
	addq $0x8, %rsp
	iretq

/* Interrupt handler for IRQ1 */
intr_irq1:
	pushq %rax
	pushq %rcx
	pushq %rdx
	/* Print the key to the bottom right */
	xorl %eax, %eax
	inb $0x60, %al
	testb $0x80, %al
	jnz 1f /* Key released */
	movl $keymap_base, %edx	/* Use base keymap */
	addl %eax, %edx
	movb (%edx), %al
	movb $0x07, %ah
	movw %ax, (0xb8000+80*25*2-2)
1:
	/* APIC EOI */
	movq $MSR_APIC_BASE, %rcx
	rdmsr /* Read APIC info to [%edx:%eax]; N.B., higer */
				/*  32 bits of %rax and %rdx are cleared */
				/*  bit [35:12]: APIC Base, [11]: EN */
				/*  [10]: EXTD, and [8]:BSP */
	shlq $32, %rdx
	addq %rax, %rdx
	andq $0xfffffffffffff000, %rdx        /* APIC Base */
	movl $0, 0x0b0(%rdx)       /* EOI */
	popq %rdx
	popq %rcx
	popq %rax
	iretq

/* Crash interrupt (hang here) */
intr_crash:
1:
	hlt
	jmp	1b

  /* Data section */
	.data

keymap_base:
	.ascii	"  1234567890-=  qwertyuiop[]  as"
	.ascii	"dfghjkl;'` \\zxcvbnm,./          "
	.ascii	"                                "
	.ascii	"                                "
