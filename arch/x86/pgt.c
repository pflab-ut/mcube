/**
 * @file arch/x86/pgt.c
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


static uintptr_t get_cr3(void)
{
  uintptr_t cr3;
  asm volatile("movq %%cr3,%%rax" : "=a"((cr3)));
  return cr3;
}


/*
 * Calculate the physical address from virtual address
 */
static uintptr_t v2p(pgt_t *pgt, uintptr_t physical)
{
  return physical - pgt->p2v;
}


/*
 * Resolve the virtual address from a physical address
 */
static uintptr_t p2v(pgt_t *pgt, uintptr_t physical)
{
  return physical + pgt->p2v;
}

/*
 * Initialize the page table
 */
void pgt_init(pgt_t *pgt, void *pml4, uintptr_t p2v)
{
  pgt->p2v = p2v;
  pgt->free = NULL;
  kmemset(pml4, 0, 4096);
  pgt->cr3 = v2p(pgt, (uintptr_t) pml4);
}

/*
 * Resolve the physical address from a virtual address
 */
void *pgt_v2p(pgt_t *pgt, uintptr_t virtual)
{
  uintptr_t p;
  int idx;
  union pgt_pml4_entry *pml4;
  union pgt_pdpt_entry *pdpt;
  union pgt_pd_entry *pd;
  union pgt_pt_entry *pt;

  /* PML4 */
  p = MASK_PAGE(pgt->cr3);
  idx = (virtual >> 39) & 0x1ff;
  pml4 = (void *) p2v(pgt, p);
  if (!pml4[idx].ptr.present) {
    /* Not present */
    return NULL;
  }
  /* PDPT */
  p = MASK_PAGE(pml4[idx].v);
  idx = (virtual >> 30) & 0x1ff;
  pdpt = (void *) p2v(pgt, p);
  if (!pdpt[idx].ptr.present) {
    /* Not present */
    return NULL;
  }
  if (pdpt[idx].ptr.page) {
    /* 1 GiB paging */
    return (void *) MASK_SUPERPAGE(pdpt[idx].v);
  }
  /* PD */
  p = MASK_PAGE(pdpt->v);
  idx = (virtual >> 21) & 0x1ff;
  pd = (void *) p2v(pgt, p);
  if (!pd[idx].ptr.present) {
    /* Not present */
    return NULL;
  }
  if (pd[idx].ptr.page) {
    /* 2 MiB paging */
    return (void *) MASK_SUPERPAGE(pd[idx].v);
  }
  /* PT */
  p = MASK_PAGE(pd->v);
  idx = (virtual >> 12) & 0x1ff;
  pt = (void *) p2v(pgt, p);
  if (!pt[idx].page.present) {
    /* Not present */
    return NULL;
  }
  return (void *) MASK_PAGE(pt[idx].v);
}

/*
 * Get a page
 */
void *pgt_pop(pgt_t *pgt)
{
  pgt_entry_t *pg;

  if ((pg = pgt->free) != NULL) {
    pgt->free = pg->next;
  }

  return pg;
}

/*
 * Release a page
 */
void pgt_push(pgt_t *pgt, pgt_entry_t *pg)
{
  pg->next = pgt->free;
  pgt->free = pg;
}

/*
 * Map the virtual address to the specified physical address
 */
int pgt_map(pgt_t *pgt, uintptr_t virtual, uintptr_t physical, int superpage,
            int global, int rw, int user)
{
  uintptr_t p;
  union pgt_pml4_entry *pml4;
  union pgt_pdpt_entry *pdpt;
  union pgt_pd_entry *pd;
  union pgt_pt_entry *pt;
  int idx;

  /* Align */
  if (superpage) {
    virtual = virtual & ~((1ULL << 21) - 1);
    physical = physical & ~((1ULL << 21) - 1);
  } else {
    virtual = virtual & ~((1ULL << 12) - 1);
    physical = physical & ~((1ULL << 12) - 1);
  }

  /* PML4 */
  p = MASK_PAGE(pgt->cr3);
  idx = (virtual >> 39) & 0x1ff;
  pml4 = (void *) p2v(pgt, p);
  if (!pml4[idx].ptr.present) {
    /* Not present, then add a page here */
    if ((pdpt = pgt_pop(pgt)) == NULL) {
      return -1;
    }
    kmemset(pdpt, 0, 4096);
    pml4[idx].ptr.present = 1;
    pml4[idx].ptr.rw = 1;
    pml4[idx].ptr.us = 1;
    pml4[idx].v |= v2p(pgt, (uint64_t) pdpt);
  } else {
    p = MASK_PAGE(pml4[idx].v);
    pdpt = (void *) p2v(pgt, p);
  }

  /* PDPT */
  idx = (virtual >> 30) & 0x1ff;
  if (!pdpt[idx].ptr.present) {
    /* Not present, then add a page here */
    if ((pd = pgt_pop(pgt)) == NULL) {
      return -1;
    }
    kmemset(pd, 0, 4096);
    pdpt[idx].ptr.present = 1;
    pdpt[idx].ptr.rw = 1;
    pdpt[idx].ptr.us = 1;
    pdpt[idx].v |= v2p(pgt, (uint64_t) pd);
  } else {
    p = MASK_PAGE(pdpt[idx].v);
    pd = (void *) p2v(pgt, p);
  }

  /* PD */
  idx = (virtual >> 21) & 0x1ff;
  if (superpage) {
    if (pd[idx].ptr.present) {
      return -1;
    }
    /* Not present, then map here */
    pd[idx].page.present = 1;
    pd[idx].page.page = 1;
    pd[idx].page.rw = rw;
    pd[idx].page.us = user;
    pd[idx].page.g = global;
    pd[idx].v |= physical;
    /* Invalidate */
    if (MASK_PAGE(get_cr3()) == MASK_PAGE(pgt->cr3)) {
      panic("xxx");
      invlpg(virtual);
    }
  } else {
    if (!pd[idx].ptr.present) {
      /* Not present, then add a page here */
      if ((pt = pgt_pop(pgt)) == NULL) {
        return -1;
      }
      kmemset(pt, 0, 4096);
      pd[idx].ptr.present = 1;
      pd[idx].ptr.rw = 1;
      pd[idx].ptr.us = 1;
      pd[idx].v |= v2p(pgt, (uint64_t) pt);
    } else {
      p = MASK_PAGE(pd[idx].v);
      pt = (void *) p2v(pgt, p);
    }

    /* PT */
    idx = (virtual >> 12) & 0x1ff;
    if (pt[idx].page.present) {
      return -1;
    }
    /* Not present, then map here */
    pt[idx].page.present = 1;
    pt[idx].page.rw = rw;
    pt[idx].page.us = user;
    pt[idx].page.g = global;
    pt[idx].v |= physical;

    /* Invalidate */
    if (MASK_PAGE(get_cr3()) == MASK_PAGE(pgt->cr3)) {
      invlpg(virtual);
    }
  }

  return 0;
}

/*
 * Unmap the virtual address to the specified physical address
 */
int pgt_unmap(pgt_t *pgt, uintptr_t virtual, int superpage)
{
  uintptr_t p;
  union pgt_pml4_entry *pml4;
  union pgt_pdpt_entry *pdpt;
  union pgt_pd_entry *pd;
  union pgt_pt_entry *pt;
  int idx;
  int i;

  /* Align */
  if (superpage) {
    virtual = virtual & ~((1ULL << 21) - 1);
  } else {
    virtual = virtual & ~((1ULL << 12) - 1);
  }

  /* PML4 */
  p = MASK_PAGE(pgt->cr3);
  idx = (virtual >> 39) & 0x1ff;
  pml4 = (void *) p2v(pgt, p);
  if (!pml4[idx].ptr.present) {
    /* Not present */
    return -1;
  }

  /* PDPT */
  p = MASK_PAGE(pml4[idx].v);
  pdpt = (void *) p2v(pgt, p);
  idx = (virtual >> 30) & 0x1ff;
  if (!pdpt[idx].ptr.present) {
    /* Not present */
    return -1;
  }

  /* PD */
  p = MASK_PAGE(pdpt[idx].v);
  pd = (void *) p2v(pgt, p);
  idx = (virtual >> 21) & 0x1ff;
  if (!pd[idx].ptr.present) {
    /* Not present */
    return -1;
  }

  /* Release a superpage */
  if (superpage) {
    if (!pd[idx].ptr.page) {
      /* Not a page */
      return -1;
    }
    pd[idx].v = 0;

    /* Invalidate */
    if (MASK_PAGE(get_cr3()) == MASK_PAGE(pgt->cr3)) {
      invlpg(virtual);
    }
  } else {
    /* PT */
    p = MASK_PAGE(pd[idx].v);
    pt = (void *) p2v(pgt, p);
    idx = (virtual >> 12) & 0x1ff;
    if (!pt[idx].page.present) {
      /* Not present */
      return -1;
    }
    pt[idx].v = 0;

    /* Invalidate */
    if (MASK_PAGE(get_cr3()) == MASK_PAGE(pgt->cr3)) {
      invlpg(virtual);
    }

    /* Check all entries of PT */
    for (i = 0; i < 512; i++) {
      if (pt[i].page.present) {
        return 0;
      }
    }
    pgt_push(pgt, (void *) pt);
    idx = (virtual >> 21) & 0x1ff;
    pd[idx].v = 0;
  }

  /* Check all entries of PD */
  for (i = 0; i < 512; i++) {
    if (pd[i].ptr.present) {
      return 0;
    }
  }
  pgt_push(pgt, (void *) pd);
  idx = (virtual >> 30) & 0x1ff;
  pdpt[idx].v = 0;

  /* Check all entries of PDPT */
  for (i = 0; i < 512; i++) {
    if (pdpt[i].ptr.present) {
      return 0;
    }
  }
  pgt_push(pgt, (void *) pdpt);
  idx = (virtual >> 39) & 0x1ff;
  pml4[idx].v = 0;

  return 0;
}

/*
 * Invalidate page table
 */
void pgt_invalidate(pgt_t *pgt, uintptr_t virtual)
{
  (void) pgt;
  invlpg(virtual);
}

/*
 * Set the cr3 register
 */
void pgt_set_cr3(pgt_t *pgt)
{
  set_cr3(pgt->cr3);
}

