/**
 * @file include/x86/desc.h
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

#ifndef	__MCUBE_X86_DESC_H__
#define	__MCUBE_X86_DESC_H__

/* GDT type */
#define GDT_TYPE_EX     8
#define GDT_TYPE_DC     4
#define GDT_TYPE_RW     2
#define GDT_TYPE_AC     1
/* IDT flags */
#define IDT_PRESENT     0x80
#define IDT_INTGATE     0x0e
#define IDT_TRAPGATE    0x0f

#define TSS_INACTIVE    0x9
#define TSS_BUSY        0xb

#ifndef __ASSEMBLY__

/*
 * Global Descriptor
 *  base: 32-bit base address of the memory space
 *  limit: 20-bit size minus 1
 *  type: (Executable, Direction/Conforming, Readable/Writable, Accessed)
 *  DPL: Privilege (0: highest, 3: lowest)
 *  P: Present bit
 *  G: Granularity; 0 for 1 byte block, 1 for 4 KiB block granularity
 *  DB: Size; 0 for 16 bit mode, 1 for 32 bit mode
 *  L: 1 for 64 bit mode
 *  A: 0
 *  S: 1
 */
struct gdt_desc {
    uint16_t w0;        /* limit[0:15] */
    uint16_t w1;        /* base[0:15] */
    uint16_t w2;        /* base[16:23] type[0:3] S DPL[0:1] P  */
    uint16_t w3;        /* limit[16:19] A L DB G base[24:31] */
} __attribute__ ((packed));

/*
 * TSS
 */
struct gdt_desc_tss {
    uint16_t w0;
    uint16_t w1;
    uint16_t w2;
    uint16_t w3;
    uint16_t w4;
    uint16_t w5;
    uint16_t w6;
    uint16_t w7;
} __attribute__ ((packed));

/*
 * Global Descriptor Table Register
 */
struct gdtr {
    uint16_t size;
    uint64_t base;      /* (struct gdt_desc *) */
} __attribute__ ((packed));

/*
 * Interrupt Descriptor
 */
struct idt_gate_desc {
    uint16_t target_lo;
    uint16_t selector;
    uint8_t reserved1;
    uint8_t flags;
    uint16_t target_mid;
    uint32_t target_hi;
    uint32_t reserved2;
} __attribute__ ((packed));

/*
 * Interrupt Descriptor Table Register
 */
struct idtr {
    uint16_t size;
    uint64_t base;      /* (struct idt_gate_desc *) */
} __attribute__ ((packed));

/* Prototype declarations */
struct gdtr * gdt_init(void);
struct idtr * idt_init(void);
void idt_setup_intr_gate(int, void *);
void idt_setup_trap_gate(int, void *);
void tss_init(void);
void tr_load(int);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_DESC_H__ */

