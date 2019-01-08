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

#define MSR_APIC_BASE           0x1b
#define APIC_LAPIC_ID           0x020

	.globl	ap_entry32

	.text
	.code32

/*
 * Entry point to the 32-bit protected mode for application processors
 */
ap_entry32:
	cli

	/* %cs is automatically set after the long jump operation */
	/* Setup other segment registers */
	movl	$AP_GDT_DATA64_SEL,%eax
	movl	%eax,%ss
	movl	%eax,%ds
	movl	%eax,%es
	movl	%eax,%fs
	movl	%eax,%gs

	/* Get the local APIC ID */
	movl    $MSR_APIC_BASE,%ecx
	rdmsr
	andl	$0xfffffffffffff000,%eax        /* APIC Base */
	movl	APIC_LAPIC_ID(%eax),%eax
	shrl	$24,%eax

	movl	$0xb8000,%ebx
	movl	$160,%ecx
	mull	%ecx
	subl	$2,%eax
	movl	$0x0721,%edx
	addl	%eax,%ebx
	movl	%edx,(%ebx)

1:
	hlt
	jmp	1b

