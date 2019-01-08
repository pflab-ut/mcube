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

/* Page table */
#define KERNEL_PGT      0x00079000

	.globl	entry32

	/* Code segment */
	.text
	.code32

/* Entry point for 32-bit protected mode */
entry32:
	cli

	/* Mask all interrupts (i8259) */
	movb	$0xff,%al
	outb	%al,$0x21
	movb	$0xff,%al
	outb	%al,$0xa1

	/* Set data segment registers to data descriptors */
	movl	$0x20,%eax
	movl	%eax,%ss
	movl	%eax,%ds
	movl	%eax,%es
	movl	%eax,%fs
	movl	%eax,%gs

	/* Setup stack */
	movl	$0x7c00,%eax
	movl	%eax,%esp

	/* Enable Page Address Extension (PAE) */
	movl	$0x20,%eax		/* CR4[bit 5] = PAE */
	movl	%eax,%cr4

/* Create 64bit page table */
pg_setup:
	movl	$KERNEL_PGT,%ebx	/* Low 12 bit must be zero */
	movl	%ebx,%edi
	xorl	%eax,%eax
	movl	$(512*8*6/4),%ecx
	rep	stosl			/* Initialize %ecx*4 bytes from %edi */
					/*  with %eax */
	/* Level 4 page map */
	leal	0x1007(%ebx),%eax
	movl	%eax,(%ebx)
	/* Page directory pointers (PDPE) */
	leal	0x1000(%ebx),%edi
	leal	0x2007(%ebx),%eax
	movl	$4,%ecx
pg_setup.1:
	movl	%eax,(%edi)
	addl	$8,%edi
	addl	$0x1000,%eax
	loop	pg_setup.1
	/* Page directories (PDE) */
	leal	0x2000(%ebx),%edi
	movl	$0x083,%eax
	movl	$(512*4),%ecx
pg_setup.2:
	movl	%eax,(%edi)
	addl	$8,%edi
	addl	$0x00200000,%eax
	loop	pg_setup.2

	/* Modify kernel page table */
	movl	%ebx,%edi
	addl	$(0x1000*5),%edi
	movl	$0x083,%eax
	movl	%eax,(%edi)

	/* Set page table register */
	movl	%ebx,%cr3

	/* Enable long mode */
	movl	$0xc0000080,%ecx	/* EFER MSR number */
	rdmsr				/* Read from 64 bit-specific register */
	btsl	$8,%eax			/* LME bit = 1 */
	wrmsr

	/* Activate page translation and long mode */
	movl	$0x80000001,%eax
	movl	%eax,%cr0

	/* Load code64 descriptor */
	pushl	$0x08
	pushl	$entry64
	lret
