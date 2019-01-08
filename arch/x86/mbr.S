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

#define VGA_TEXT_COLOR_80x25    0x03
#define NUM_RETRIES     3               /* # of retries for disk read */
#define BOOTMON_SEG             0x0900  /* Memory where to load boot monitor */
#define BOOTMON_OFF             0x0000  /*  segment and offset [0900:0000] */
#define BOOTMON_SIZE            0x0020  /* Boot monitor size in sector */

	.globl	start

	/* Code segment */
	.text
	.code16

start:
	cld		/* Clear direction flag (inc di/si for str ops) */
	cli

	/* Setup the stack (below $start = 0x7c00) */
	xorw	%ax,%ax
	movw	%ax,%ss
	movw	$start,%sp
	/* Reset data segment registers */
	movw	%ax,%ds
	movw	%ax,%es

	sti

	/* Save drive information */
	movb	%dl,drive

	/* Set the video mode to 16-bit color text mode */
	movb	$VGA_TEXT_COLOR_80x25,%al
	movb	$0x00,%ah
	int	$0x10

	/* Get drive parameters */
	xorw	%ax,%ax
	movw	%ax,%es
	movw	%ax,%di
	movb	$0x08,%ah
	int	$0x13
	jc	disk_error

	/* Save the drive information (CHS) */
	incb	%dh		/* Get # of heads (%dh: last index of heads) */
	movb	%dh,heads
	movb	%cl,%al		/* %cl[5:0]: last index of sectors per track */
	andb	$0x3f,%al	/*  N.B., sector is one-based numbering */
	movb	%al,sectors
	movb	%ch,%al         /* %cx[7:6]%cx[15:8]: last index of cylinders */
				/*  then copy %cx[15:8] to %al */
	movb	%cl,%ah		/* Lower byte to higher byte */
	shrb	$6,%ah		/* Pick most significant two bits */
	incw	%ax		/*  N.B., cylinder starting with 0 */
	movw	%ax,cylinders

	/* Load boot monitor */
	movb	drive,%dl
	movw	$BOOTMON_SIZE,%cx/* Specify boot monitor size */
	movw	$1,%ax
	movw	$BOOTMON_SEG,%bx
	movw	%bx,%es		/* Buffer address pointer (Segment) */
	movw	$BOOTMON_OFF,%bx/* Buffer address pointer (Offset) */
	call	read		/* Read %cx sectors starting at LBA %ax on */
				/*  drive %dl into %es:[%bx] */

	/* Jump to boot monitor */
	ljmp	$BOOTMON_SEG,$BOOTMON_OFF

/*
 * Load sectors from the disk
 * Parameters:
 *   %dl: drive
 *   %cx: # of sectors to read
 *   %ax: Position in LBA
 *   %es:(%bx): Buffer
 */
read:
	pushw	%bp
	movw	%sp,%bp
	/* Save registers */
	movw	%ax,-2(%bp)
	movw	%bx,-4(%bp)
	movw	%cx,-6(%bp)
	movw	%dx,-8(%bp)
	/* Prepare space for local variables */
	/* u16 counter -10(%bp) */
	subw	$10,%sp
	/* Reset counter */
	xorw	%ax,%ax
	movw	%ax,-10(%bp)
1:
	movw	-2(%bp),%ax	/* Restore %ax */
	addw	-10(%bp),%ax	/* Current LBA */
	call	lba2chs		/* Convert LBA (%ax) to CHS (%cx,%dh) */
	call	read_sector	/* Read a sector */
	/* Next 512-byte buffer */
	addw	$512,%bx
	/* Increment the counter */
	movw	-10(%bp),%ax
	incw	%ax
	movw	%ax,-10(%bp)
	/* More sectors to read? */
	cmpw	-6(%bp),%ax
	jb	1b		/* Read more sectors */
	/* Restore the saved registers */
	movw	-8(%bp),%dx
	movw	-6(%bp),%cx
	movw	-4(%bp),%bx
	movw	-2(%bp),%ax
	movw	%bp,%sp
	popw	%bp
	ret

/*
 * Load one sector from the disk
 * Parameters:
 *   %dl: drive
 *   %cx, %dh: CHS (%cx[7:6]%cx[15:8] ,%dh, %cx[5:0])
 *   %es:(%bx): Buffer
 */
read_sector:
	pushw	%bp
	movw	%sp,%bp
	/* Save registers */
	movw	%ax,-2(%bp)
	/* Prepare space for local variables */
	/* u16 retries -4(%bp); retry counter  */
	/* u16 error -6(%bp); error code  */
	subw	$6,%sp
	/* Reset retry counter */
	xorw	%ax,%ax
	movw	%ax,-4(%bp)
1:
	/* Read a sector from the drive */
	movb	$0x02,%ah
	movb	$1,%al
	int	$0x13
	jnc	2f		/* Jump if success */
	movw	%ax,-6(%bp)	/* Save the error code */
	movw	-4(%bp),%ax
	incw	%ax
	movw	%ax,-4(%bp)
	cmpw	$NUM_RETRIES,%ax
	movw	-6(%bp),%ax	/* Restore the error code */
	ja	disk_error	/* Exceeded the maximum number of retries */
	jmp	1b
2:
	/* Restore saved registers */
	movw	-2(%bp),%ax
	movw	%bp,%sp
	popw	%bp
	ret

/*
 * LBA to CHS
 * Parameters:
 *   %ax: LBA
 * Return values:
 *   %cx, %dh: CHS (%cx[7:6]%cx[15:8] ,%dh, %cx[5:0])
 */
lba2chs:
	/* Save registers */
	pushw	%ax
	pushw	%bx
	pushw	%dx
	/* Compute sector number */
	xorw	%bx,%bx
	movw	%bx,%dx
	movw	%bx,%cx
	movb	sectors,%bl
	divw	%bx		/* %dx:%ax / %bx; %ax:quotient, %dx:remainder */
	incw	%dx		/* Sector number is one-based numbering */
	movb	%dl,%cl		/* Sector: %cx[5:0] */
	/* Compute head and track (cylinder) numbers */
	xorw	%bx,%bx
	movw	%bx,%dx
	movb	heads,%bl
	divw	%bx		/* %dx:%ax / %bx; %ax:quotient, %dx:remainder */
	movb	%al,%ch		/* Cylinder[7:0]: %cx[7:6]%cx[15:8] */
	shlb	$6,%ah
	orb	%ah,%cl
	movw	%dx,%bx		/* %dl: Head */
	popw	%dx		/* Restore %dx */
	movb	%bl,%dh		/* Head */
	/* Restore registers */
	popw	%bx
	popw	%ax
	ret

/*
 * Display a null-terminated string
 * Parameters:
 *   %ds:(%si): Pointer to the string
 * Unpreserved registers: %ax
 */
putstr:
putstr.load:
	lodsb			/* Load %ds:(%si) to %al, then incl %si */
	testb	%al,%al
	jnz	putstr.putc
	xorw	%ax,%ax
	ret
putstr.putc:
	call	putc
	jmp	putstr
putc:
	pushw	%bx
	movw	$0x7,%bx
	movb	$0xe,%ah
	int	$0x10
	popw	%bx
	ret

/*
 * Display the read error message (%ah = error codee)
 * Parameters:
 *   %ds:(%si): Pointer to the string
 * Unpreserved registers: %es, %ax, %di
 */
disk_error:
	pushw	%bx
	movb	%ah,%al
	movw	$error_code,%di
	xorw	%bx,%bx
	movw	%bx,%es
	call	hex8
	movw	$msg_error,%si
	call	putstr
	popw	%bx
	ret

/*
 * Convert %al to hex characters, and save the result to %es:(%di)
 * Parameters:
 *   %es:(%di): Pointer to the buffer to store the result (2 bytes)
 *   %al: Byte to be converted to hex characters
 * Unpreserved registers: %al, %di
 */
hex8:
	pushw	%ax
	shrb	$4,%al		/* Get the most significant 4 bits in %al */
	call	hex8.allsb
	popw	%ax
hex8.allsb:
	andb	$0xf,%al	/* Get the least significant 4 bits in %al */
	cmpb	$0xa,%al	/* CF=1 if %al < 10 */
	sbbb	$0x69,%al	/* %al <= %al - (0x69 + CF) */
	das			/* Adjust BCD */
	orb	$0x20,%al	/* To lower case */
	stosb			/* Save chars to %es:(%di) and inc %di */
	ret

	/* Data segment */
	.data

drive:
	.byte	0
heads:
	.byte	0
cylinders:
	.word	0
sectors:
	.byte	0
msg_error:
	.ascii	"Disk error: 0x"
error_code:
	.asciz	"00\r"

