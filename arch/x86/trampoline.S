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

/* Note that .data section cannot be used in the trampoline file because we
   need to determine the contiguous space of the trampoline code within a
   section from two labels; between _trampoline and _trampoline_end. */

	.globl	trampoline
	.globl	trampoline_end

	.text
	.code16

/*
 * Trampoline code starts.  Note that the trampoline code is loaded into a
 * 4 KiB (aligned) page in the lower 1 MiB of memory.  The %cs is automatically
 * set after the SIPI.  %ip is expected to be zero but not sure.  So, we first
 * calculate the offsets of idtr and gdtr.
 */
trampoline:
	cli

	/* Calculate the base address */
	xorl	%eax,%eax
	movw	$(TRAMPOLINE_VEC << 8),%ax
	movw	%ax,%ds

	/* Setup GDT and IDT */
	lidt	%ds:(idtr - trampoline)
	lgdt	%ds:(gdtr - trampoline)

	/* Turn on protected mode */
	movl	%cr0,%eax
	orl	$0x1,%eax
	movl	%eax,%cr0
	ljmpl	$AP_GDT_CODE32_SEL,$(ap_entry32 - KERNEL_RELOCBASE)

	/* Data section but trampoline code cannot be in separated sections */
	.align	16

/* Pseudo interrupt descriptor table */
idtr:
	.word	0x0		/* Limit */
	.long	0x0		/* Base address */
/* Temporary global descriptor table */
gdt:
	.word	0x0,0x0,0x0,0x0		/* Null descriptor */
	.word	0xffff,0x0,0x9a00,0xaf	/* Code 64 */
	.word	0xffff,0x0,0x9200,0xaf	/* Data 64 */
	.word	0xffff,0x0,0x9a00,0xcf	/* Code 32 */
	.word	0xffff,0x0,0x9200,0xcf	/* Data 32 */
	.word	0xffff,0x0,0x9a00,0x8f	/* Code 16 */
	.word	0xffff,0x0,0x9200,0x8f	/* Data 16 */
gdt.1:
gdtr:
	.word	gdt.1 - gdt - 1	/* Limit */
	.long	gdt		/* Base address */

trampoline_end:
