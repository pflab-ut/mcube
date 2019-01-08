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

#ifndef _ADVOS_KERNEL_PGT_H
#define _ADVOS_KERNEL_PGT_H

#ifndef __ASSEMBLY__

/*
 * Entry
 */
typedef struct pgt_entry pgt_entry_t;
struct pgt_entry {
    pgt_entry_t *next;
};

/*
 * Page table
 */
typedef struct {
    /* Pointer (physical) to PML4 */
    uintptr_t cr3;
    /* Offset to calculate the virtual address from the physical address */
    uintptr_t p2v;
    /* Free pages to be used as page tables */
    pgt_entry_t *free;
} pgt_t;

/* Prototype declarations */
void pgt_init(pgt_t *, void *, uintptr_t);
void * pgt_pop(pgt_t *);
void pgt_push(pgt_t *, pgt_entry_t *);
void * pgt_v2p(pgt_t *, uintptr_t);
int pgt_map(pgt_t *, uintptr_t, uintptr_t, int, int, int, int);
int pgt_unmap(pgt_t *, uintptr_t, int);
void pgt_set_cr3(pgt_t *);

#endif /* !__ASSEMBLY__ */

#endif
