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

/*
 * Setup a null descriptor
 */
static void
gdt_setup_desc_null(struct gdt_desc *e)
{
    e->w0 = 0;
    e->w1 = 0;
    e->w2 = 0;
    e->w3 = 0;
}

/*
 * Setup global descriptor table entry
 */
static void
gdt_setup_desc(struct gdt_desc *e, uint32_t base, uint32_t limit, uint8_t type,
               uint8_t dpl, uint8_t l, uint8_t db, uint8_t g)
{
    limit &= 0xfffff;
    type &= 0xf;

    /* Setup the descriptor: see desc.h for detailed information */
    /* P=1, A=0, S=1 */
    /* 16bit: l=0, db=0
       32bit: l=0, db=1
       64bit: l=1, db=0 */
    e->w0 = limit & 0xffff;
    e->w1 = base & 0xffff;
    e->w2 = ((base >> 16) & 0xff) | ((uint64_t)type << 8) | ((uint64_t)1 << 12)
        | ((uint64_t)dpl << 13) | ((uint64_t)1 << 15);
    e->w3 = ((limit >> 16) & 0xf) | ((uint64_t)l << 5) | ((uint64_t)db << 6)
        | ((uint64_t)g << 7) | (((base >> 24) & 0xff) << 8);
}

/*
 * Setup global descriptor table for TSS
 */
static void
gdt_setup_desc_tss(struct gdt_desc_tss *e, uint64_t base, uint32_t limit,
                   uint8_t type, uint8_t dpl, uint8_t g)
{
    limit &= 0xfffff;
    type &= 0xf;

    /* g => *4KiB */
    e->w0 = limit & 0xffff;
    e->w1 = base & 0xffff;
    e->w2 = ((base >> 16) & 0xff) | ((uint64_t)type << 8)
        | ((uint64_t)dpl << 13) | ((uint64_t)1 << 15);
    e->w3 = ((limit >> 16) & 0xf) | ((uint64_t)g << 7)
        | (((base >> 24) & 0xff) << 8);
    e->w4 = base >> 32;
    e->w5 = base >> 40;
    e->w6 = 0;
    e->w7 = 0;
}

/*
 * Initialize global descriptor table
 */
struct gdtr *
gdt_init(void)
{
    uint64_t sz;
    struct gdt_desc *gdt;
    struct gdt_desc_tss *tss;
    struct gdtr *gdtr;
    uint8_t code;
    uint8_t data;

    sz = GDT_NR * sizeof(struct gdt_desc)
        + MAX_PROCESSORS * sizeof(struct gdt_desc_tss);
    /* GDT register */
    gdtr = (struct gdtr *)(GDT_ADDR + sz);
    /* Global descriptors */
    gdt = (struct gdt_desc *)GDT_ADDR;
    /* Type */
    code = GDT_TYPE_EX | GDT_TYPE_RW;
    data = GDT_TYPE_RW;

    /* Null descriptor */
    gdt_setup_desc_null(&gdt[0]);
    /* Code and data descriptor for each ring */
    gdt_setup_desc(&gdt[1], 0, 0xfffff, code, 0, 1, 0, 1); /* Ring 0 code */
    gdt_setup_desc(&gdt[2], 0, 0xfffff, data, 0, 1, 0, 1); /* Ring 0 data */
    gdt_setup_desc(&gdt[3], 0, 0xfffff, code, 3, 0, 1, 1); /* Ring 3 code */
    gdt_setup_desc(&gdt[4], 0, 0xfffff, data, 3, 0, 1, 1); /* Ring 3 data */
    gdt_setup_desc(&gdt[5], 0, 0xfffff, code, 3, 1, 0, 1); /* Ring 3 code */
    gdt_setup_desc(&gdt[6], 0, 0xfffff, data, 3, 1, 0, 1); /* Ring 3 data */

    /* TSS */
    tss = (struct gdt_desc_tss *)(GDT_ADDR + GDT_TSS_SEL_BASE);
    gdt_setup_desc_tss(&tss[0], 0x00078000, sizeof(struct tss) - 1,
                       TSS_INACTIVE, 0, 0);

    /* Set the GDT base address and the table size */
    gdtr->base = (uint64_t)gdt;
    gdtr->size = sz - 1;

    return gdtr;
}

/*
 * Setup gate descriptor
 */
static void
idt_setup_gate_desc(struct idt_gate_desc *idt, uint64_t base, uint16_t selector,
                    uint8_t flags)
{
    idt->target_lo = (uint16_t)(base & 0xffffULL);
    idt->selector = (uint16_t)selector;
    idt->reserved1 = 0;
    idt->flags = flags;
    idt->target_mid = (uint16_t)((base & 0xffff0000ULL) >> 16);
    idt->target_hi = (uint16_t)((base & 0xffffffff00000000ULL) >> 32);
    idt->reserved2 = 0;
}

/*
 * Setup an interrupt gate in the interrupt descriptor table
 */
void
idt_setup_intr_gate(int nr, void *target)
{
    struct idt_gate_desc *idt;

    idt = (struct idt_gate_desc *)(IDT_ADDR
                                   + nr * sizeof(struct idt_gate_desc));
    idt_setup_gate_desc(idt, (uint64_t)target, GDT_RING0_CODE_SEL,
                        IDT_PRESENT | IDT_INTGATE);
}

/*
 * Setup a trap gate in the interrupt descriptor table
 */
void
idt_setup_trap_gate(int nr, void *target)
{
    struct idt_gate_desc *idt;

    idt = (struct idt_gate_desc *)(IDT_ADDR
                                   + nr * sizeof(struct idt_gate_desc));
    idt_setup_gate_desc(idt, (uint64_t)target, GDT_RING0_CODE_SEL,
                        IDT_PRESENT | IDT_TRAPGATE);
}

/*
 * Initialize interrupt descriptor table
 */
struct idtr *
idt_init(void)
{
    struct idtr *idtr;
    int i;

    /* Setup the interrupt gates */
    for ( i = 0; i < IDT_NR; i++ ) {
        idt_setup_intr_gate(i, &intr_null);
    }

    idtr = (struct idtr *)(IDT_ADDR + sizeof(struct idt_gate_desc) * IDT_NR);
    idtr->base = IDT_ADDR;
    idtr->size = IDT_NR * sizeof(struct idt_gate_desc) - 1;

    return idtr;
}

/*
 * Initialize all TSS
 */
void
tss_init(void)
{
    struct tss *tss;

    tss = (struct tss *)0x00078000;
    tss->reserved1 = 0;
    tss->rsp0l = 0;
    tss->rsp0h = 0;
    tss->rsp1l = 0;
    tss->rsp1h = 0;
    tss->rsp2l = 0;
    tss->rsp2h = 0;
    tss->reserved2 = 0;
    tss->reserved3 = 0;
    tss->ist1l = 0;
    tss->ist1h = 0;
    tss->ist2l = 0;
    tss->ist2h = 0;
    tss->ist3l = 0;
    tss->ist3h = 0;
    tss->ist4l = 0;
    tss->ist4h = 0;
    tss->ist5l = 0;
    tss->ist5h = 0;
    tss->ist6l = 0;
    tss->ist6h = 0;
    tss->ist7l = 0;
    tss->ist7h = 0;
    tss->reserved4 = 0;
    tss->reserved5 = 0;
    tss->reserved6 = 0;
    tss->iomap = 0;
}

/*
 * Load task register for nr-th processor
 */
void
tr_load(int nr)
{
    int tr;

    tr = GDT_TSS_SEL_BASE + nr * sizeof(struct gdt_desc_tss);
    ltr(tr);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
