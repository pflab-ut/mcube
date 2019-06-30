/**
 * @file arch/x86_64/rmcommon.h
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Commonly used x86_64 real-mode patterns
 *
 * Copyright (C) 2009-2012 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * Below "patterns" are shared between the bootsector and the real-mode
 * code that loads the ramdisk!
 */

/**
 * @def STACK_OFFSET
 * @brief Arbitrary stack offset for kernel real-mode code and bootsector.
 * It will hopefully be large enough for the called BIOS services.
 */
#define STACK_OFFSET 0x3000

/*
 * The low memory region [0x9000->0xe000] is reserved for passed
 * real-mode method and pmode->rmode->pmode switching code.
 */

/**
 * @def PMODE16_START
 * @brief Where the 16-bit protected and real mode code is going to be copied to
 * and executed from.
 */
#define PMODE16_START 0x9000

/**
 * @def RCODE_STACK
 * @brief Stack top address, 12-KB in size.
 */
#define RCODE_STACK (PMODE16_START + STACK_OFFSET)

/**
 * @def RCODE_PARAMS_BASE
 * @brief 4-KB area for passing parameters.
 */
#define RCODE_PARAMS_BASE 0xd000

/**
 * @def RCODE_PARAMS_END
 * @brief Parameters region end.
 */
#define RCODE_PARAMS_END 0xe000

/*
 * Parameters sent to the real-mode functions:
 */

/**
 * @def RCODE_SECTOR_LBA
 * @brief Disk logical block address to read from.
 */
#define RCODE_SECTOR_LBA (RCODE_PARAMS_BASE)  /* long */

/**
 * @def RCODE_FIRST_CALL
 * @brief Mark the first call (to print a message).
 */
#define RCODE_FIRST_CALL (RCODE_PARAMS_BASE + 4)  /* long */

/**
 * @def RCODE_DRIVE_NUMBER
 * @brief The BIOS-passed EDD drive number.
 */
#define RCODE_DRIVE_NUMBER (RCODE_PARAMS_BASE + 8)  /* long */

/**
 * @def RCODE_BUFFER
 * @brief Temporary buffer to hold the just-read sector.
 */
#define RCODE_BUFFER (RCODE_PARAMS_BASE +12)  /* 512-bytes */

/**
 * @def EDD_CHECK_EXTENSIONS_PRESENT(driveno, fail_label)
 * @brief Check for Enhanced Disk Drive (EDD) BIOS support.
 * Jump to @a fail_label if no such extension exist.
 *
 * INT 0x13, function 0x41
 * input  @c bx     - 0x55aa (defined by ebios standard)
 * input  @c dl     - drive number (provided by bios in @c dl)
 * output success - carry = 0 && bx = 0xaa55 &&
 *                  cx bit 0 = 1 (enhanced drive access)
 * output failure - carry = 1 || bx != 0xaa55
 *
 * @param driveno Bios drive number.
 * @param fail_label Jump there if no enhanced bios exist.
 */
#define EDD_CHECK_EXTENSIONS_PRESENT(driveno, fail_label) \
  movb   (driveno), %dl;                                  \
  movb   $0x41, %ah;                                      \
  movw   $0x55aa, %bx;                                    \
  int    $0x13;                                           \
  jc     fail_label;                                      \
  cmpw   $0xaa55, %bx;                                    \
  jne    fail_label;                                      \
  shrw   $1, %cx;                                         \
  jnc    fail_label;

/**
 * @def PUT_PRINT_METHODS()
 * @brief Print methods.
 *
 * INT 0x10, function 0x0e - write teletype to active page
 * input  @c al    - character to write
 * input  @c bh    - page number
 * input  @c bl    - characters' attribute
 *
 * Character attribute is an 8 bit value, low 4 bits sets
 * foreground color, while the high 4 sets the background.
 * Check vga.h for the actual values.
 */
#define PUT_PRINT_METHODS()                               \
                                                          \
  /* print %dx value in hexadecimal */                    \
  print_hex:                                              \
  xorb   %bh, %bh;                                        \
  movw   $4, %cx;    /*4 hex digits (2-bytes)*/           \
.next:  rolw   $4, %dx;    /*Next digit*/                 \
  movw   $0x0e0f, %ax;                                    \
  andb   %dl, %al;  /*Save leftmost digit in %al..*/      \
  cmpb   $0x0a, %al;                                      \
  jl     1f;                                              \
  addb   $0x07, %al;                                      \
1:  addb   $0x30, %al;  /*.. and transform it to ascii*/  \
  int    $0x10;                                           \
  loop   .next;                                           \
  ret;                                                    \
                                                          \
  /* For debugging: Print %eax in hex */                  \
print_eax:                                                \
 pushl  %eax;                                             \
 sarl   $16, %eax;                                        \
 movw   %ax, %dx;                                         \
 call   print_hex;                                        \
 popl   %eax;                                             \
 movw   %ax, %dx;                                         \
 call   print_hex;                                        \
 ret;                                                     \
                                                          \
 /* print string pointed by %ds:si */                     \
print_string:                                             \
 xorb   %bh, %bh;                                         \
 movb   $0x0e, %ah;                                       \
 lodsb;                                                   \
 cmpb   $0, %al;    /*NULL mark reached?*/                \
 je     1f;                                               \
 int    $0x10;                                            \
 jmp    print_string;                                     \
1:  ret;

