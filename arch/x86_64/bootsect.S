/**
 * @file arch/x86_64/bootsect.S
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * Real-mode memory map:
 *
 *          segment:offset                physical
 *  0x0000:0x0000 -> 0x0030:0x00ff  (0x00000 -> 0x003ff) real mode interrupt vector table
 *  0x0040:0x0000 -> 0x0040:0x00ff  (0x00400 -> 0x004ff) bios data area, 256 bytes
 *  0x0050:0x0000 -> 0x8000:0xffff  (0x00500 -> 0x8ffff) free memory
 *  0x9000:0x0000 -> 0x9000:0xffff  (0x90000 -> 0x9ffff) usually Extended BIOS Data Area
 *  0xa000:0x0000 -> 0xb000:0xffff  (0xa0000 -> 0xbffff) VGA frame buffer
 *  0xc000:0x0000 -> 0xc000:0x7fff  (0xc0000 -> 0xc7fff) video bios, 32K
 *  0xc000:0x8000 -> 0xe000:0xffff  (0xc8000 -> 0xeffff) empty
 *  0xf000:0x0000 -> 0xf000:0xffff  (0xf0000 -> 0xfffff) motherboard bios, 64K
 *
 * The bootsector is loaded inside the 630K free memory region:
 *
 *  0x07c0:0x0000 -> 0x07e0:0x0000  (0x07c00 -> 0x07e00) bootsector, 512 bytes
 */

/* The 'valid boot sector' flag; it must be put at the end of
 * the mbr, or otherwise the bios will happily ignore us */
#define BOOT_SIGNATURE  0xaa55

/* Print messages to screen */
#define MSG(msg)    \
  movw   $msg, %si;    \
  call   print_string

#include "rmcommon.h"

.code16

/* The text section is relocated by the linker to 0x0000, thus
 * when we send an address of a string, we're actually sending
 * its offset from whatever segment currently setup in the
 * code segment register */
.text
.global _start
_start:
  /* All BIOSes load the mbr to the same 0x07c00 physical
   * address. The difference is that some load it using the
   * 0x07c0:0x00 scheme while others load it as 0x00:0x7c00.
   * For certainity, we force our own realmode code segment
   * value using a far jump */
  .equ   MBR_SEG, 0x07c0
  ljmp   $MBR_SEG, $real_start

real_start:
  movw   %cs, %ax
  movw   %ax, %es
  movw   %ax, %ds
  movw   %ax, %es
  movw   %ax, %ss
  movw   $STACK_OFFSET, %sp

  /* Permanently save the BIOS-passed EDD Drive Number */
  .equ   driveno, RCODE_DRIVE_NUMBER - MBR_SEG << 4
  movb   %dl, (driveno)

  MSG    (welcome)

  /* Check for enhanced bios services to move beyond the 8GB
   * limit with LBA, or the 504MB limit with no LBA (CHS).
   * If no enhanced services exist (a pre-1995 bios), we'll
   * gracefully exit. */
  EDD_CHECK_EXTENSIONS_PRESENT(driveno, no_ebios)

  /* Extended BIOS Data Area grows down the top of low memory
   * at 0x9ffff and usually spans to 0x9a000 these days. We load
   * our kernel up to 0x8ffff, so we won't run on any BIOS that
   * expands its data area lower than 0x90000. INT 0x12 reports
   * the size of low memory beginning from 0x00000 in kilobytes */
  .equ   $MIN_MEMSIZE, 0x90000
  .equ   $KILOBYTE, 0x400
  xor    %ax, %ax
  int    $0x12
  cmpw   $($MIN_MEMSIZE / $KILOBYTE), %ax
  jl     huge_ebda

  /* Copy the kernel from disk to its destined starting address
   * at 0x10000. For simplicity, we'll copy the disk's first
   * 640 sectors post the mbr to ram regardless of the real
   * kernel size; we may optimize later. The kernel will be in
   * the range [0x10000-0x50000); sector size = 512 bytes.
   *
   * INT 0x13, function 0x42 - extended read sectors
   * input  %dl     - drive number (provided by bios in %dl)
   * input  %ds:si  - pointer to the Disk Address Packet
   * output %ah     - error code (if any)
   * output CF      - error flag (true = error) */
  .equ   KERNEL_SEGMENT, 0x1000
  .equ   KERNEL_SEGMENT_END, 0x9000
  movw   $10, %cx      # 10 * 64 sector (32K) = 320K
  movb   (driveno), %dl
  movw   $disk_address_packet, %si
  movw   $KERNEL_SEGMENT, buffer_seg
  movw   $1, sector    # mbr is sector 0
next_sectors:
  movb   $64, sectors_cnt
  movb   $0x42, %ah
  int    $0x13
  jc     disk_error
  addw   $64, sector
  addw   $0x8000, buffer_offset   # 32K copied; 0x8000 bytes
  jnc    same_segment
  addw   $0x1000, buffer_seg  # 64K copied, move to next segment
  movw   $0x0000, buffer_offset
same_segment:
  loop   next_sectors

  /* Now we prepare moving to protected mode
   *
   * we disable interrupts cause the interrupt table format
   * for protected mode and real-mode differs. It is inevitable
   * that if IDTR selects an interrupt table, it  will have the
   * wrong format at some time which can lead to unpredictable
   * behvaior (horrors)
   *
   * The processor will still attempt to access the interrupt
   * table if an exception or nonmaskable interrupt (NMI) occurs;
   * change the limit value in the IDTR to zero, which will force
   * a shutdown if an exception or a malice NMI occurs: already
   * unrecoverable events at this stage. */
  cli
  lidt   idtdesc      # null
  lgdt   gdt      # pmode cs and ds descriptors

  /* Enable the A20. The story goes that IBM, to achive backward
   * compatibility with the 8086s, used a three-state multiplexer
   * to gate the A20 line output from its 80286 CPUs before it
   * reached the rest of the machine. A tri-state mux can either
   * produce 0, efficively killing the a20 line, or go to a high
   * impedence state where it just acts as a wire. They used an
   * unused pin in the keyboard controller to control that mux */
  call   enable_a20

  /* Set the protected mode PE flag */
  movl   %cr0, %eax
  orl    $0x1, %eax
  movl   %eax, %cr0

  /* Flush the processor's instruction prefetch queue by
   * executing a jump instruction. The 80386 fetches and decodes
   * instructions and addresses before they are used; however,
   * after a change into protected mode, the prefetched
   * instruction information is no longer valid. A JMP forces
   * the processor to discard the invalid information.
   *
   * Note that a short jump to a code32 block won't work. We
   * either continue running in 16-bit potected mode by a short
   * jump to a code16 block, or move to 32-bit pmode by a long
   * jump to a code32 block where %cs segment cache is updated */
  jmp    flush_prefetch

flush_prefetch:
  /* Setup the data segments and the stack pointer. Note that
   * the segment descriptors index is 8bytes aligned, thus
   * Intel uses the least significant 3 bits in the segment
   * selector to decode the DPL and the TI bits */
  movw   $(gdt_ds - gdt), %ax
  movw   %ax, %ds
  movw   %ax, %es
  movw   %ax, %fs
  movw   %ax, %gs
  movw   %ax, %ss
  movl   $((MBR_SEG << 4) + STACK_OFFSET), %esp

  /* Setup %cs: long jump to 32-bit protected mode. GAS should
   * have known that $(gdt_cs - gdt), equalling 0x08, *is* an
   * absoulte code segment value, but it doesn't.
   *
   * Far pointer offset is the destination's full flat address
   * since now setup %cs base = 0 */
  ljmp   $0x08, $((MBR_SEG << 4) + reloc)

.code32

reloc:
  /* Goodbye the 80's; welcome the 90's!
   * Relocate the kernel above the one mege-byte point at
   * 0x100000. We should have directly done so using multiple
   * stages, but we're still very simple (read: braindead) */
  .equ   KERNEL_RELOC, 0x100000
  .equ   KERNEL_SIZE, 0x80000
  movl   $(KERNEL_SEGMENT << 4), %esi
  movl   $KERNEL_RELOC, %edi
  movl   $(KERNEL_SIZE / 4), %ecx
  rep    movsl

  /* Run our kernel with the currently setup code segment:
   * perform an absolute near jump */
  movl   $KERNEL_RELOC, %eax
  jmpl   *%eax

.code16

/*
 * A20 line methods
 *
 * There's usually a controller in the keyboard itself and
 * a motherboard keyboard controller responsible for buffering,
 * data validity checking, and translation form Kscan codes
 * provided by the in-keyboard controller to scan codes.
 *
 * On AT+ machines, ports 0x60 and 0x64 communicate with the
 * motherboard keyboard controller. What's relative to us is
 * port 0x64 which is used to read status from the
 * motherboard keyboard controller and to send commands to it
 *
 * Port 0x64 input:
 *
 * bit1: If clear, the motherboard controller's input buffer
 * is empty. A write can be made to port 0x60 or port 0x64.
 * If set, the motherboard controller's input buffer is full.
 * Until the motherboard controller has emptied the buffer,
 * no writes should occur to ports 0x60/0x64 or the command
 * will be lost.
 *
 * bit0: If clear, the motherboard controller's output buffer
 * is empty. A read from port 0x60 will not be valid.
 * If set, the motherboard controller's output buffer has
 * a byte available. Use port 0x60 to read that byte.
 *
 * Port 0x64 output:
 * (send commands to motherboard's keyboard controller)
 *
 * 0xd0: Read the controller's state/output-port. The byte can
 * be read by reading from 0x60 afterwards.
 * bit1: if set, A20 line is on. The opposite if clear.
 *
 * 0xD1: The next byte written to 0x60 is transfered to the
 * controller's state/output-port
 * same bit values semantics as the read state command
 */

  .equ   KCOMM, 0x64    # controller command io port
  .equ   KDATA, 0x60    # controller data io port
  .equ   KCOMM_GETSTATE, 0xd0
  .equ   KCOMM_SETSTATE, 0xd1

enable_a20:
  /* First we read the controller state and save it. To avoid
   * affecting other system functions, we want to keep the
   * other state bits intact while setting the A20 bit */
  call   kpoll_empty
  movb   $KCOMM_GETSTATE, %al
  outb   $KCOMM
  call   kpoll_output    # wait for output
  xorw   %ax, %ax
  inb    $KDATA
  pushw  %ax
  /* Then we write the new state with the A20 bit set */
  call   kpoll_empty
  movb   $KCOMM_SETSTATE, %al
  outb   $KCOMM
  call   kpoll_empty
  popw   %ax
  orw    $0x02, %ax
  outb   $KDATA
  ret        # done (hopefully)

/* loop till the motherboard controller's input buffer
 * is empty. You can write to 0x64 or 0x60 afterwards */
kpoll_empty:
  xorw   %ax, %ax
  inb    $KCOMM
  btw    $1, %ax
  jc     kpoll_empty
  ret

/* loop till the motherboard controller's output buffer
 * has an available byte to read from 0x60 */
kpoll_output:
  xorw   %ax, %ax
  inb    $KCOMM
  btw    $0, %ax
  jnc    kpoll_output
  ret

  PUT_PRINT_METHODS()

/*
 * Error conditions
 */

huge_ebda:
  pushw  %ax
  MSG    (ebda)
  popw   %dx
  call   print_hex
  jmp    halt
disk_error:
  pushw  %ax
  MSG    (disk)
  popw   %dx
  call   print_hex
  jmp    halt
no_ebios:
  MSG    (old_bios)
halt:
  MSG    (error)
  jmp    .

/*
 * Disk Address Packet
 */

disk_address_packet:
  .byte  0x10    # Packet size in bytes
  .byte  0x00    # Unused
sectors_cnt:
  .byte  0x00    # Number of blocks to transfer [1 - 127]
  .byte  0x00    # Unused
buffer_offset:
  .word  0x00    # Read/Write buffer address
buffer_seg:
  .word  0x00
sector:
  .quad  0x00    # Logical sector number

/*
 * Protected mode GDT
 *
 * Refer to attached Intel i686 documentation, vol. 3:
 * 2.4 Memory-management registers
 * 3.4.3 Segment descriptors
 * 3.4.3.1 Code and data segment descriptor types
 */

/* As said by linux x86_64 boot code, there are machines which are
 * known not to boot with the GDT being 8-byte unaligned. Intel also
 * recommends 16 byte alignment, so let it be.
 *
 * "The limit value is expressed in bytes and is added to the base
 * address to get the address of the last valid byte.  A limit value
 * of 0 results in exactly 1 valid byte. Because IDT/GDT entries are
 * always eight bytes long, the limit should always be one less than
 * an integral multiple of eight (that is, 8N - 1)."  --Intel */
.align 16
/*
 * 31                                                             0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | Base address  |G|D|X|A| Limit |P|DPL|S| Type  | Base address  | +8
 * | 31-24         | |B| | | 19-16 | |   | |       | 23-16         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * | Base address                  | Segment limit                 | +0
 * | 15-0                          | 15-0                          |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 63                                                            32
 *
 * G: Granularity bit. If 0 the limit is in 1 B blocks (byte granularity),
 *    if 1 the limit is in 4 KiB blocks (page granularity).
 * DB: Size bit. If 0 the selector defines 16 bit protected mode. If 1 it
 *     defines 32 bit protected mode. You can have both 16 bit and 32 bit
 *     selectors at once.
 * X: L bit in x86_64
 * P: Present bit. This must be 1 for all valid selectors.
 * DPL: Privilege, 2 bits. Contains the ring level, 0 = highest (kernel),
 *      3 = lowest (user applications).
 * A: 0
 * S: 1
 * Type: Ex DC RW Ac
 * Ex: Executable bit. If 1 code in this segment can be executed, ie. a
 *     code selector. If 0 it is a data selector.
 * DC: Direction bit/Conforming bit.
 * RW: Readable bit/Writable bit.
 * Ac: Accessed bit. Just set to 0. The CPU sets this to 1 when the segment
 *     is accessed.
 */
gdt:
  /* since the null descriptor values are not used, we
   * save the gdt table descriptor here at its place */
  .word  gdt_end - gdt -1 # limit
  .long  MBR_SEG << 4 + gdt  # NOTE! protected mode flat address
  .word  0x0000    # padding
gdt_cs:
  /* code segment descriptor */
  /* 0x00cf9a000000ffff */ 
  .word  0xffff      # limit
  .word  0x0000      # base
  .word  0x9a00      # P=1, DPL=00, type=0xa (execute/read)
  .word  0x00cf      # G=1 (4K), D=1, limit[16:19]=0xf
gdt_ds:
 /* data segment descriptor */
 /* 0x00cf92000000ffff */
  .word  0xffff      # limit
  .word  0x0000      # base
  .word  0x9200      # P=1, DPL=00, type=0x2 (read/write)
  .word  0x00cf      # G=1 (4K), B=1, limit[16:19]=0xf
gdt_end:

idtdesc:
  .word  0
  .long  0

/*
 * Strings
 */

ebda:
  .asciz "Huge EBDA = "
disk:
  .asciz "Disk "
old_bios:
  .asciz "Old BIOS"
error:
  .asciz " error; EXITING"
welcome:
  .asciz "Loading..\r\n"

.org 510
boot_flag:
  .word  BOOT_SIGNATURE
