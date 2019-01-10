/**
 * @file arch/x86/bootmon.S
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


	.globl bootmon

	/* Code segment */
	.text
	.code16

/*
 * Boot monitor (from MBR)
 *   %cs:%ip=0x0900:0x0000 (=0x9000)
 *   %dl: drive
 *   %ds: 0x0000
 */
bootmon:
	/* Save %dl parameter to the .data section */
	movb %dl, drive

	/* Set video mode to 16bit color text mode (redundant) */
	movb $VGA_TEXT_COLOR_80x25, %al
	movb $0x00, %ah
	int	$0x10

	/* Get drive parameters */
	xorw %ax, %ax
	movw %ax, %es
	movw %ax, %di
	movb $0x08, %ah
	int $0x13
	jc disk_error

	/* Save the drive information (CHS) */
	incb %dh /* Get # of heads (%dh: last index of heads) */
	movb %dh, heads
	movb %cl, %al /* %cl[5:0]: last index of sectors per track */
	andb $0x3f, %al /*  N.B., sector is one-based numbering */
	movb %al, sectors
	movb %ch, %al /* %cx[7:6]%cx[15:8]: last index of cylinders */
	              /* then copy %cx[15:8] to %al */
	movb %cl, %ah	/* Lower byte to higher byte */
	shrb $6, %ah /* Pick most significant two bits */
	incw %ax /*  N.B., cylinder starting with 0 */
	movw %ax, cylinders

	/* Enable A20 */
	call enable_a20

	/* Reset the boot information region */
	xorw %ax, %ax
	movl $BI_START_ADDR, %edi
	movl $(BI_SIZE/2), %ecx
	rep	stosw
	/* Save the information on BIOS */
	movb drive, %al
	movb %al, (BI_DRIVE_ADDR)

	/* Load memory map */
	movw $BI_MM_TABLE_ADDR, %di
	movw $(BI_MM_SIZE / MME_SIZE), %dx
	call load_mm
	jc 1f
	movw %ax, (BI_MM_NENT_ADDR)
	jmp	2f
1:
	movw $msg_error_mm, %ax
	movw %ax, %si
	call putstr
	jmp	halt
2:

	/* Display boot option message */
	movw $msg_bootopt, %ax
	movw %ax, %si
	call putstr

	/* Setup programmable interrupt controller (PIC) */
	xorw %ax, %ax
	movw %ax, %es
	movw $intr_irq0, %ax
	movw $(IVT_IRQ0 + 0), %bx
	call setup_intvec

	xorw %ax, %ax
	movw %ax, %es
	movw $intr_irq1, %ax
	movw $(IVT_IRQ0 + 1), %bx
	call setup_intvec

	movw $BOOT_TIMEOUT * 100, boottimer	/* in centisecond */

	/* Initialize programmable interval timer (PIT) */
	call init_pit

	/* Wait until the timer reached zero or keyboard input */
1:
	sti
	hlt
	cli
	movb bootmode, %al
	cmpb $'1', %al
	je boot
	cmpb $'2', %al
	je poweroff
	cmpw $0, boottimer
	je boot
	jmp 1b

/*
 * Boot sequence
 */
boot:
	call load_kernel
	jmp entry16

halt:
	hlt
	jmp halt

/* Enable A20 address line */
enable_a20:
	cli
	pushw %ax
	pushw %cx
	xorw %cx, %cx
1:
	incw %cx /* Try until %cx overflows (2^16 times) */
	jz 3f /*  Failed to enable a20 */
	inb $0x64, %al /* Get status from the keyboard controller */
	testb $0x2, %al /* Busy? */
	jnz 1b /* Yes, busy.  Then try again */
	movb $0xd1, %al	/* Command: Write output port (0x60 to P2) */
	outb %al, $0x64	/* Write the command to the command register */
2:
	inb $0x64, %al /* Get status from the keyboard controller */
	testb $0x2, %al /* Busy? */
	jnz 2b /* Yes, busy.  Then try again */
	movb $0xdf, %al	/* Command: Enable A20 */
	outb %al, $0x60	/* Write to P2 via 0x60 output port */
3:
	popw %cx
	popw %ax
	sti
	ret

/*
 * Timer interrupt handler
 */
intr_irq0:
	pushw %ax
	pushw %bx
	pushw %cx
	pushw %dx
	pushw %si

	movw boottimer, %ax
	testw %ax, %ax
	jz 1f
	decw %ax
	movw %ax, boottimer
1:
	movb $100, %dl /* Convert centisecond to second */
	divb %dl /* Q=%al, R=%ah */
	xorb %ah, %ah
	movb $10, %dl
	divb %dl /* Q(%al) = tens digit, R(%ah) = unit digit */
	addb $'0', %al
	addb $'0', %ah
	movw %ax, msg_count
	movw $msg_countdown, %si
	call putbstr

	movb $0x20, %al /* Notify */
	outb %al, $0x20 /*  End-Of-Interrupt (EOI) */

	popw %si
	popw %dx
	popw %cx
	popw %bx
	popw %ax
	iret

/*
 * Keyboard interrupt handler
 */
intr_irq1:
	pushw %ax
	pushw %bx
	xorw %ax, %ax
	inb $0x60, %al /* Scan code from the keyboard controller */
1:
	movb %al, %bl /* Ignore the flag */
	and $0x7f, %bl /*  indicating released in %bl */
	cmpb $KBD_LSHIFT, %bl /* Left shift */
	je 4f /* Jump if left shift */
	cmpb $KBD_RSHIFT, %bl /* Right shift */
	je 4f /* Jump if right shift */
	/* Otherwise */
	testb $0x80, %al /* Released? */
	jnz 6f /* Yes, then ignore the key */
	cmpb $0, (keyboard_shift)	/* Shift key is released? */
	je 2f /* Yes, then use base keymap */
	movw $keymap_shift, %bx /*  Otherwise, use shifted keymap */
	jmp 3f
2:
	movw $keymap_base, %bx /* Use base keymap */
3:
	addw %ax, %bx
	movb (%bx), %al /* Get ascii code from the keyboard code */
	movb %al, bootmode
	call putc /* Print the character */
	movb $0x08, %al /* Print backspace */
	call putc /* for the next input */
	jmp 6f
4:
	testb $0x80, %al /* Released? */
	jnz	5f /* Yes, then clear shift key */
	movb $1, (keyboard_shift) /* Set shift key */
	jmp 6f
5:
	movb $0, (keyboard_shift)	/* Clear shift key */
6:
	movb $0x20, %al /* Notify */
	outb %al, $0x20 /* End-Of-Interrupt (EOI) */
	popw %bx
	popw %ax
	iret

/*
 * Setup interrupt vector
 *   %es: code segment
 *   %ax: instruction pointer
 *   %bx: interrupt vector number
 */
setup_intvec:
	pushw %bx
	shlw $2, %bx
	movw %ax, (%bx)
	addw $2, %bx
	movw %es, (%bx)
	popw %bx
	ret

/*
 * Disable i8259 PIC
 */
disable_pic:
	pushw %ax
	movb $0xff, %al
	outb %al, $0xa1
	movb $0xff, %al
	outb %al, $0x21
	popw %ax
	ret

/*
 * Initialize programmable interval timer (PIT)
 */
init_pit:
	pushw %ax
	movb $(PIT_BINARY | PIT_MODE3 | PIT_HILO | PIT_CHANNEL0), %al
	outb %al, $0x43
	movw $0x2e9b, %ax /* Frequency=100Hz: 1193181.67/100 */
	outb %al, $0x40 /* Counter 0 (least significant 4 bits) */
	movb %ah, %al /* Get most significant 4 bits */
	outb %al, $0x40 /* Counter 0 (most significant 4 bits) */
	popw %ax
	ret

/*
 * Power off via APM
 */
poweroff:
	/* Disable PIC */
	call disable_pic

	/* Power off with APM */
	movw $0x5301, %ax /* Connect APM interface */
	movw $0x0, %bx /* Specify system BIOS */
	int $0x15 /* Return error code in %ah with CF */
	jc 1f /* Error */

	movw $0x530e, %ax /* Set APM version */
	movw $0x0, %bx /* Specify system BIOS */
	movw $0x102, %cx /* Version 1.2 */
	int $0x15 /* Return error code in %ah with CF */
	jc 1f /* Error */

	movw $0x5308, %ax /* Enable power management */
	movw $0x1, %bx /* All devices managed by the system BIOS */
	movw $0x1, %cx /* Enable */
	int $0x15 /* Ignore errors */

	movw $0x5307, %ax /* Set power state */
	movw $0x1, %bx /* All devices managed by the system BIOS */
	movw $0x3, %cx /* Off */
	int	$0x15
1:
	ret /* Return on error */

/*
 * Load the kernel (at most 128KB)
 */
load_kernel:
	pushw	%ax
	pushw	%bx
	pushw	%cx
	pushw	%dx
	pushw	%es

	/* Load the kernel (in 33rd - 160th sectors) to 0x1000:0x0000 [0x00010000, 0x00020000) */
	movb drive, %dl
	movw $TRANSFER_SIZE ,%cx
	movw $KERNEL_LBA, %ax
	movw $KERNEL_SEG, %bx
	movw %bx, %es
	movw $KERNEL_OFFSET, %bx
	call read

	/* Load the kernel (at 161th - 288th sectors) to 0x2000:0x0000 [0x00020000, 0x00030000) */
	movb drive, %dl
	movw $TRANSFER_SIZE, %cx
	movw $KERNEL_LBA + (65536 / 512), %ax
	movw $KERNEL_SEG + 0x1000, %bx
	movw %bx, %es
	movw $KERNEL_OFFSET, %bx
	call read

	popw %es
	popw %dx
	popw %cx
	popw %bx
	popw %ax
	ret

/*
 * Load sectors from the disk
 * Parameters:
 *   %dl: drive
 *   %cx: # of sectors to read
 *   %ax: Position in LBA
 *   %es:(%bx): Buffer
 */
read:
	pushw %bp
	movw %sp, %bp
	/* Save registers */
	movw %ax, -2(%bp)
	movw %bx, -4(%bp)
	movw %cx, -6(%bp)
	movw %dx, -8(%bp)
	/* Prepare space for local variables */
	/* u16 counter -10(%bp) */
	subw $10, %sp
	/* Reset counter */
	xorw %ax, %ax
	movw %ax, -10(%bp)
1:
	movw -2(%bp), %ax /* Restore %ax */
	addw -10(%bp), %ax /* Current LBA */
	call lba2chs /* Convert LBA (%ax) to CHS (%cx,%dh) */
	call read_sector /* Read a sector */
	/* Next 512-byte buffer */
	addw $512, %bx
	/* Increment the counter */
	movw -10(%bp), %ax
	incw %ax
	movw %ax, -10(%bp)
	/* More sectors to read? */
	cmpw -6(%bp), %ax
	jb 1b /* Read more sectors */
	/* Restore the saved registers */
	movw -8(%bp), %dx
	movw -6(%bp), %cx
	movw -4(%bp), %bx
	movw -2(%bp), %ax
	movw %bp, %sp
	popw %bp
	ret

/*
 * Load one sector from the disk
 * Parameters:
 *   %dl: drive
 *   %cx, %dh: CHS (%cx[7:6]%cx[15:8] ,%dh, %cx[5:0])
 *   %es:(%bx): Buffer
 */
read_sector:
	pushw %bp
	movw %sp, %bp
	/* Save registers */
	movw %ax, -2(%bp)
	/* Prepare space for local variables */
	/* u16 retries -4(%bp); retry counter  */
	/* u16 error -6(%bp); error code  */
	subw $6, %sp
	/* Reset retry counter */
	xorw %ax, %ax
	movw %ax, -4(%bp)
1:
	/* Read a sector from the drive */
	movb $0x02, %ah
	movb $1, %al
	int $0x13
	jnc 2f /* Jump if success */
	movw %ax, -6(%bp)	/* Save the error code */
	movw -4(%bp), %ax
	incw %ax
	movw %ax, -4(%bp)
	cmpw $NUM_RETRIES, %ax
	movw -6(%bp), %ax /* Restore the error code */
	ja disk_error /* Exceeded the maximum number of retries */
	jmp 1b
2:
	/* Restore saved registers */
	movw -2(%bp), %ax
	movw %bp, %sp
	popw %bp
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
	pushw %ax
	pushw %bx
	pushw %dx
	/* Compute sector number */
	xorw %bx, %bx
	movw %bx, %dx
	movw %bx, %cx
	movb sectors, %bl
	divw %bx /* %dx:%ax / %bx; %ax:quotient, %dx:remainder */
	incw %dx /* Sector number is one-based numbering */
	movb %dl, %cl /* Sector: %cx[5:0] */
	/* Compute head and track (cylinder) numbers */
	xorw %bx, %bx
	movw %bx, %dx
	movb heads, %bl
	divw %bx /* %dx:%ax / %bx; %ax:quotient, %dx:remainder */
	movb %al, %ch /* Cylinder[7:0]: %cx[7:6]%cx[15:8] */
	shlb $6,%ah
	orb %ah, %cl
	movw %dx, %bx /* %dl: Head */
	popw %dx /* Restore %dx */
	movb %bl, %dh /* Head */
	/* Restore registers */
	popw %bx
	popw %ax
	ret

/*
 * Display a null-terminated string
 * Parameters:
 *   %ds:(%si): Pointer to the string
 * Unpreserved registers: %ax
 */
putstr:
putstr.load:
	lodsb /* Load %ds:(%si) to %al, then incl %si */
	testb %al, %al
	jnz putstr.putc
	xorw %ax, %ax
	ret
putstr.putc:
	call putc
	jmp putstr
putc:
	pushw %bx
	movw $0x7, %bx
	movb $0xe, %ah
	int $0x10
	popw %bx
	ret

/*
 * Display a null-terminated string at the bottom-line
 * Parameters:
 *   %ds:%si --> 0xb800:**
 */
putbstr:
	pushw %ax
	pushw %es
	pushw %di
	movw $0xb800, %ax	/* Memory 0xb8000 */
	movw %ax, %es
	movw $(80*24*2) ,%di  /* 24th (zero-numbering) line */
putbstr.load:
	lodsb /* Load %al from %ds:(%si), then incl %si */
	testb %al, %al /* Stop at null */
	jnz	putbstr.putc /* Call the function to output %al */
	popw %di
	popw %es
	popw %ax
	ret
putbstr.putc:
	movb $0x7, %ah
	stosw /* Write %ax to [%di], then add 2 to %di */
	jmp	putbstr.load


/*
 * Display the read error message (%ah = error codee)
 * Parameters:
 *   %ds:(%si): Pointer to the string
 * Unpreserved registers: %es, %ax, %di
 */
disk_error:
	pushw	%bx
	movb %ah, %al
	movw $error_code, %di
	xorw %bx, %bx
	movw %bx, %es
	call hex8
	movw $msg_error, %si
	call putstr
	popw %bx
	ret

/*
 * Convert %al to hex characters, and save the result to %es:(%di)
 * Parameters:
 *   %es:(%di): Pointer to the buffer to store the result (2 bytes)
 *   %al: Byte to be converted to hex characters
 * Unpreserved registers: %al, %di
 */
hex8:
	pushw %ax
	shrb $4, %al /* Get the most significant 4 bits in %al */
	call hex8.allsb
	popw %ax
hex8.allsb:
	andb $0xf, %al /* Get the least significant 4 bits in %al */
	cmpb $0xa, %al /* CF=1 if %al < 10 */
	sbbb $0x69, %al /* %al <= %al - (0x69 + CF) */
	das /* Adjust BCD */
	orb	$0x20, %al /* To lower case */
	stosb /* Save chars to %es:(%di) and inc %di */
	ret

/*
 * Load memory map entries from BIOS
 * Parameters:
 *   %es:%di: destination
 *   %dx: the maximum number of entries
 * Return values:
 *   %ax: the number of entries
 *   CF: set if an error occurs
 */
load_mm:
	/* Save registers */
	pushl %ebx
	pushl %ecx
	pushw %di
	pushw %bp

	xorl %ebx, %ebx /* Continuation value for int 0x15 */
	xorw %bp, %bp /* Counter */
load_mm.1:
	movl $0x1, %ecx /* Write 1 once */
	movl %ecx, %es:20(%di) /* to check support ACPI >=3.x? */
	/* Read the system address map */
	movl $0xe820, %eax
	movl $MME_SIGN, %edx /* Set the signature */
	movl $MME_SIZE, %ecx /* Set the buffer size */
	int $0x15 /* Query system address map */
	jc load_mm.error /* Error */
	cmpl $MME_SIGN, %eax /* Check the signature SMAP */
	jne	load_mm.error

	cmpl $24, %ecx /* Check the read buffer size */
	je load_mm.2	/* %ecx==24 */
	cmpl $20, %ecx
	je load_mm.3	/* %ecx==20 */
	jmp	load_mm.error	/* Error otherwise */
load_mm.2:
	/* 24-byte entry */
	testl $0x1, %es:20(%di)	/* 1 must be present in the attribute */
	jz load_mm.error	/* error if it's overwritten */
load_mm.3:
	/* 20-byte entry or 24-byte entry coming from above */
	incw %bp /* Increment the number of entries */
	testl %ebx, %ebx /* %ebx=0: No remaining info */
	jz load_mm.done /* jz/je */
	cmpw %bp, %dx
	je load_mm.error /* Exceeded the maximum number to read */
load_mm.4:
	addw $MME_SIZE, %di /* Next entry */
	jmp	load_mm.1	/* Load remaining entries */
load_mm.error:
	stc /* Set CF */
load_mm.done:
	movw %bp, %ax /* Return value */
	popw %bp
	popw %di
	popl %ecx
	popl %ebx
	ret

	/* Data segment */
	.data

drive:
	.byte 0
heads:
	.byte 0
cylinders:
	.word 0
sectors:
	.byte 0

boottimer:
	.word 0
bootmode:
	.byte 0

msg_bootopt:
	.ascii "Welcome to Mcube!\r\n\n"
	.ascii "Select one:\r\n"
	.ascii "    1: Boot (64 bit mode)\r\n"
	.ascii "    2: Power off\r\n"
	.asciz "Press key:[ ]\x08\x08"
msg_countdown:
	.ascii "Mcube will boot in "
msg_count:
	.asciz "00 sec."
msg_error:
	.ascii "Disk error: 0x"
error_code:
	.asciz "00\r"

msg_error_mm:
	.asciz "Error on loading the system memory map."

/* Keymap (US) */
keymap_base:
	.ascii "  1234567890-=  qwertyuiop[]  as"
	.ascii "dfghjkl;'` \\zxcvbnm,./          "
	.ascii "                                "
	.ascii "                                "
keymap_shift:
	.ascii "  !@#$%^&*()_+  QWERTYUIOP{}  AS"
	.ascii "DFGHJKL:\"~ |ZXCVBNM<>?          "
	.ascii "                                "
	.ascii "                                "

/* Keybaord status */
keyboard_shift:
  .byte 0
