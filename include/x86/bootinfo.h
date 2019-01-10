/**
 * @file include/x86/bootinfo.h
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

#ifndef	__MCUBE_X86_BOOTINFO_H__
#define	__MCUBE_X86_BOOTINFO_H__

/* Definition of the boot information region */
#define BI_START_ADDR           0x8000
#define BI_SIZE                 0x1000

/* Address to store drive information (1 byte) */
#define BI_DRIVE_ADDR           0x8000

/* Address to store # of entries of the memory map (2 bytes) */
#define BI_MM_NENT_ADDR         0x87f0
/* Address to store entries of the memory map (24-byte entries) */
#define BI_MM_TABLE_ADDR        0x8800
/* Maximum allowed size of the memory map region */
#define BI_MM_SIZE              0x0800


#define BOOTMON_SEG             0x0900  /* Memory where to load boot monitor */
#define BOOTMON_OFFSET          0x0000  /*  segment and offset [0900:0000] */
#define BOOTMON_SIZE            0x0020  /* Boot monitor size in sector */


#define VGA_TEXT_COLOR_80x25    0x03
#define NUM_RETRIES             3       /* # of retries for disk read */
#define BOOT_TIMEOUT            10      /* Boot timeout in second */
/* IRQ */
#define IVT_IRQ0                0x08    /* IRQ0 = 0x08 (BIOS default) */
#define IVT_IRQ8                0x70    /* IRQ8 = 0x70 (BIOS default) */
/* Keyboard */
#define KBD_LCTRL               0x1d    /* Left ctrl */
#define KBD_LSHIFT              0x2a    /* Left shift */
#define KBD_RSHIFT              0x36    /* Right shift */
#define KBD_CAPS                0x3a    /* Caps lock */
#define KBD_RCTRL               0x5a    /* Right ctrl */
#define KBD_UP                  0x48    /* Up */
#define KBD_LEFT                0x4b    /* Left */
#define KBD_RIGHT               0x4d    /* Right */
#define KBD_DOWN                0x50    /* Down */
/* PIT */
#define PIT_CHANNEL0            (0x0 << 6)
#define PIT_CHANNEL1            (0x1 << 6)
#define PIT_CHANNEL2            (0x2 << 6)
#define PIT_LOONLY              (0x1 << 4)
#define PIT_HIONLY              (0x2 << 4)
#define PIT_HILO                (0x3 << 4)
#define PIT_MODE0               (0x0 << 1)
#define PIT_MODE1               (0x1 << 1)
#define PIT_MODE2               (0x2 << 1)
#define PIT_MODE3               (0x3 << 1)
#define PIT_MODE4               (0x4 << 1)
#define PIT_MODE5               (0x5 << 1)
#define PIT_BINARY              (0)
#define PIT_BCD                 (1)
/* Memory map */
#define MME_SIZE                 24     /* Memory map entry size */
#define MME_SIGN                 0x534d4150     /* MME signature ("SMAP")  */
/* Kernel */
#define KERNEL_LBA               33
#define TRANSFER_SIZE            0x80 // 64K 
#define KERNEL_SEG               0x1000
#define KERNEL_OFFSET            0x0000


#endif /* __MCUBE_X86_BOOTINFO_H__ */

