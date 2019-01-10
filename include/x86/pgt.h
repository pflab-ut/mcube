/**
 * @file include/x86/pgt.h
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

#ifndef __MCUBE_X86_PGT_H__
#define __MCUBE_X86_PGT_H__


#ifndef __ASSEMBLY__

#define invlpg(addr) asm volatile("invlpg (%%rax)" :: "a"((addr)))
#define set_cr3(cr3) asm volatile("movq %%rax,%%cr3" :: "a"((cr3)))

#define MASK_PAGE(a)        ((a) & ~0xfffULL)
#define MASK_SUPERPAGE(a)   ((a) & ~0x1fffffULL)

/*
 * CR3
 */
union pgt_cr3 {
  struct {
    uint64_t ignore2:3;
    uint64_t pwt:1;         /* PWT: Page-level Write Transparent */
    uint64_t pcd:1;         /* PCD: Page-level Cache Disable */
    uint64_t ignore1:7;
    uint64_t pml4:52;
  } ptr;
  uint64_t v;
} __attribute__ ((packed));

/*
 * PML4E
 */
union pgt_pml4_entry {
  struct {
    uint64_t present:1;
    uint64_t rw:1;
    uint64_t us:1;
    uint64_t pwt:1;
    uint64_t pcd:1;
    uint64_t a:1;
    uint64_t ignore2:1;
    uint64_t rsv:1;
    uint64_t ignore1:4;
    uint64_t pdpt:51;
    uint64_t xd:1;          /* execute-disable */
  } ptr;
  uint64_t v;
} __attribute__ ((packed));

/*
 * PDPTE
 */
union pgt_pdpt_entry {
  /* Pointer */
  struct {
    uint64_t present:1;
    uint64_t rw:1;
    uint64_t us:1;
    uint64_t pwt:1;
    uint64_t pcd:1;
    uint64_t a:1;
    uint64_t ignore2:1;
    uint64_t page:1;        /* 0 */
    uint64_t ignore1:4;
    uint64_t pdpt:51;
    uint64_t xd:1;
  } ptr;
  /* Page */
  struct {
    uint64_t present:1;
    uint64_t rw:1;
    uint64_t us:1;
    uint64_t pwt:1;
    uint64_t pcd:1;
    uint64_t a:1;
    uint64_t d:1;
    uint64_t page:1;        /* 1 */
    uint64_t g:1;
    uint64_t ignore1:3;
    uint64_t pat:1;
    uint64_t pdpt:50;
    uint64_t xd:1;
  } page;
  /* Value */
  uint64_t v;
} __attribute__ ((packed));

/*
 * PDE
 */
union pgt_pd_entry {
  /* Pointer */
  struct {
    uint64_t present:1;
    uint64_t rw:1;
    uint64_t us:1;
    uint64_t pwt:1;
    uint64_t pcd:1;
    uint64_t a:1;
    uint64_t ignore2:1;
    uint64_t page:1;        /* 0 */
    uint64_t ignore1:4;
    uint64_t pdpt:51;
    uint64_t xd:1;
  } ptr;
  /* Page */
  struct {
    uint64_t present:1;
    uint64_t rw:1;
    uint64_t us:1;
    uint64_t pwt:1;
    uint64_t pcd:1;
    uint64_t a:1;
    uint64_t d:1;
    uint64_t page:1;        /* 1 */
    uint64_t g:1;
    uint64_t ignore1:3;
    uint64_t pat:1;
    uint64_t pdpt:50;
    uint64_t xd:1;
  } page;
  /* Value */
  uint64_t v;
} __attribute__ ((packed));

/*
 * PTE
 */
union pgt_pt_entry {
  /* Page */
  struct {
    uint64_t present:1;
    uint64_t rw:1;
    uint64_t us:1;
    uint64_t pwt:1;
    uint64_t pcd:1;
    uint64_t a:1;
    uint64_t d:1;
    uint64_t pat:1;
    uint64_t g:1;
    uint64_t ignore1:3;
    uint64_t pdpt:51;
    uint64_t xd:1;
  } page;
  /* Value */
  uint64_t v;
} __attribute__((packed));


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

#endif /* __MCUBE_X86_PGT_H__ */

