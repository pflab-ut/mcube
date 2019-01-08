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
 * Prototype declarations
 */
static union virt_memory_data * _data_alloc(memory_t *);
static void _data_free(memory_t *, union virt_memory_data *);
static virt_memory_block_t * _find_block(memory_t *, uintptr_t);
static int _free_add(virt_memory_block_t *, virt_memory_free_t *);
static virt_memory_free_t *
_free_atree_delete(virt_memory_free_t **, virt_memory_free_t *);
static virt_memory_free_t *
_free_delete(virt_memory_block_t *, virt_memory_free_t *);

/*
 * Initialize virtual memory
 */
int
memory_init(memory_t *mem, phys_memory_t *phys, void *arch,
            int (*map)(void *, uintptr_t, page_t *),
            int (*unmap)(void *, uintptr_t, page_t *))
{
    union virt_memory_data *data;
    size_t nr;
    size_t i;

    mem->phys = phys;
    mem->blocks = NULL;
    mem->lists = NULL;

    /* Allocate 8 MiB for page management */
    data = phys_mem_alloc(phys, 11, MEMORY_ZONE_KERNEL, 0);
    if ( NULL == data ) {
        return -1;
    }
    nr = (MEMORY_PAGESIZE << 9) / sizeof(union virt_memory_data);
    for ( i = 1; i < nr; i++ ){
        data[i - 1].next = &data[i];
    }
    data[nr - 1].next = NULL;
    mem->lists = data;

    /* Architecture specific data structure */
    mem->arch = arch;
    mem->map = map;
    mem->unmap = unmap;

    return 0;
}

/*
 * Allocate virtual memory data
 */
static union virt_memory_data *
_data_alloc(memory_t *mem)
{
    union virt_memory_data *data;

    if ( NULL == mem->lists ) {
        return NULL;
    }
    data = mem->lists;
    mem->lists = data->next;

    /* Zeros */
    kmemset(data, 0, sizeof(union virt_memory_data));

    return data;
}

/*
 * Free virtual memory data
 */
static void
_data_free(memory_t *mem, union virt_memory_data *data)
{
    data->next = mem->lists;
    mem->lists = data;
}

/*
 * Add a new memory block
 */
int
memory_block_add(memory_t *mem, uintptr_t start, uintptr_t end)
{
    virt_memory_free_t *fr;
    virt_memory_block_t *n;
    virt_memory_block_t **b;

    /* Allocate data and initialize the block */
    n = (virt_memory_block_t *)_data_alloc(mem);
    if ( NULL == n ) {
        return -1;
    }
    n->start = start;
    n->end = end;
    n->next = NULL;
    n->entries = NULL;
    n->frees.atree = NULL;
    n->frees.stree = NULL;

    /* Add a free entry aligned to the block and the page size */
    fr = (virt_memory_free_t *)_data_alloc(mem);
    if ( NULL == fr ) {
        _data_free(mem, (union virt_memory_data *)n);
        return -1;
    }
    fr->start = (start + MEMORY_PAGESIZE - 1)
        & ~(uintptr_t)(MEMORY_PAGESIZE - 1);
    fr->size = ((end + 1) & ~(uintptr_t)(MEMORY_PAGESIZE - 1)) - fr->start;
    n->frees.atree = fr;
    n->frees.stree = fr;

    /* Insert the block to the sorted list */
    b = &mem->blocks;
    while ( NULL != *b ) {
        if ( (*b)->start > n->start ) {
            /* Try to insert here */
            if ( n->end >= (*b)->start ) {
                /* Overlapping space, then raise an error */
                _data_free(mem, (union virt_memory_data *)fr);
                _data_free(mem, (union virt_memory_data *)n);
                return -1;
            }
            break;
        }
        b = &(*b)->next;
    }
    n->next = *b;
    *b = n;

    return 0;
}

/*
 * Find a memory block including the specified address
 */
static virt_memory_block_t *
_find_block(memory_t *mem, uintptr_t addr)
{
    virt_memory_block_t *b;

    /* Search a block */
    b = mem->blocks;
    while ( NULL != b ) {
        if ( addr >= b->start && addr <= b->end ) {
            /* Found */
            break;
        }
        b = b->next;
    }
    return b;
}

/*
 * Find a free entry including to the start
 */
static virt_memory_free_t *
_find_free_entry(virt_memory_free_t *e, uintptr_t addr)
{
    if ( NULL == e ) {
        return NULL;
    }
    if ( addr >= e->start && addr < e->start + e->size ) {
        /* Found */
        return e;
    }
    if ( addr < e->start ) {
        /* Search the left */
        return _find_free_entry(e->atree.left, addr);
    } else {
        /* Search the right */
        return _find_free_entry(e->atree.right, addr);
    }
}

/*
 * Find a free entry neighboring to the start and the end
 */
static virt_memory_free_t *
_find_neighbor_free_entry(virt_memory_free_t *e, uintptr_t start, uintptr_t end)
{
    if ( NULL == e ) {
        return NULL;
    }
    if ( end == e->start || start == e->start + e->size ) {
        /* Found */
        return e;
    }
    if ( start >= e->start && start < e->start + e->size ) {
        /* Overlapping region */
        return NULL;
    } else if ( start < e->start ) {
        /* Search the left */
        return _find_neighbor_free_entry(e->atree.left, start, end);
    } else {
        /* Search the right */
        return _find_neighbor_free_entry(e->atree.right, start, end);
    }
}

/*
 * Add an entry
 */
static int
_entry_add(virt_memory_entry_t **t, virt_memory_entry_t *n)
{
    if ( NULL == *t ) {
        /* Insert here */
        *t = n;
        n->atree.left = NULL;
        n->atree.right = NULL;
        return 0;
    }
    if ( n->start == (*t)->start ) {
        return -1;
    }
    if ( n->start > (*t)->start ) {
        return _entry_add(&(*t)->atree.right, n);
    } else {
        return _entry_add(&(*t)->atree.left, n);
    }
}

/*
 * Delete an entry
 */
static virt_memory_entry_t *
_entry_delete(virt_memory_entry_t **t, virt_memory_entry_t *n)
{
    virt_memory_entry_t **x;

    if ( NULL == *t ) {
        /* Not found */
        return NULL;
    }
    if ( n == (*t) ) {
        if ( n->atree.left && n->atree.right ) {
            *t = n->atree.left;
            x = &n->atree.left;
            while ( NULL != *x ) {
                x = &(*x)->atree.right;
            }
            *x = n->atree.right;
        } else if ( n->atree.left ) {
            *t = n->atree.left;
        } else if ( n->atree.right ) {
            *t = n->atree.right;
        } else {
            *t = NULL;
        }
        return n;
    }
    if ( n->start > (*t)->start ) {
        return _entry_delete(&(*t)->atree.right, n);
    } else {
        return _entry_delete(&(*t)->atree.left, n);
    }
}

/*
 * Calculate the page order
 */
static int
_order(uintptr_t addr1, uintptr_t addr2, size_t size)
{
    uintptr_t p1;
    uintptr_t p2;
    int order;

    order = 0;
    p1 = addr1 >> MEMORY_PAGESIZE_SHIFT;
    p2 = addr2 >> MEMORY_PAGESIZE_SHIFT;
    for ( order = 0; ; order++ ) {
        if ( p1 & ((2ULL << order) - 1) ) {
            break;
        }
        if ( p2 & ((2ULL << order) - 1) ) {
            break;
        }
        /* Check the size */
        if ( (2ULL << (MEMORY_PAGESIZE_SHIFT + order)) > size ) {
            /* Exceed the size for order + 1, then terminate here */
            break;
        }
    }

    return order;
}

/*
 * Wire pages
 */
int
memory_wire(memory_t *mem, uintptr_t virtual, size_t nr, uintptr_t physical)
{
    virt_memory_block_t *b;
    uintptr_t endplus1;
    virt_memory_free_t *f;
    virt_memory_free_t *f0;
    virt_memory_free_t *f1;
    virt_memory_entry_t *e;
    size_t size;
    page_t *p;
    page_t **pp;
    int ret;
    int order;

    /* Page alignment check */
    if ( virtual & (MEMORY_PAGESIZE - 1) ) {
        return -1;
    }
    if ( physical & (MEMORY_PAGESIZE - 1) ) {
        return -1;
    }

    /* Find a block including the virtual address */
    b = _find_block(mem, virtual);
    if ( NULL == b ) {
        /* Not found */
        return -1;
    }

    /* Find a free space corresponding to the virtual address */
    f = _find_free_entry(b->frees.atree, virtual);
    if ( NULL == f ) {
        /* Not found */
        return -1;
    }

    /* Check if the whole size is within this space */
    size = MEMORY_PAGESIZE * nr;
    if ( virtual + size > f->start + f->size ) {
        return -1;
    }

    /* Prepare an entry and an object */
    e = (virt_memory_entry_t *)_data_alloc(mem);
    if ( NULL == e ) {
        goto error_entry;
    }
    e->start = virtual;
    e->size = nr * MEMORY_PAGESIZE;
    e->object = (virt_memory_object_t *)_data_alloc(mem);
    if ( NULL == e->object ) {
        goto error_obj;
    }
    e->object->size = nr * MEMORY_PAGESIZE;
    e->object->pages = NULL;

    /* Prepare for free spaces */
    f0 = (virt_memory_free_t *)_data_alloc(mem);
    if ( NULL == f0 ) {
        goto error_f0;
    }
    f1 = (virt_memory_free_t *)_data_alloc(mem);
    if ( NULL == f1 ) {
        goto error_f1;
    }

    /* Allocate and map all pages */
    endplus1 = virtual + size;
    pp = &e->object->pages;
    while ( virtual < endplus1 ) {
        /* Allocate a page data structure */
        p = (page_t *)_data_alloc(mem);
        if ( NULL == p ) {
            goto error_page;
        }
        p->physical = physical;
        p->flags = MEMORY_PGF_WIRED;
        p->next = NULL;
        /* Calculate the order to minimize the number of page_t */
        order = _order(virtual, physical, endplus1 - virtual);
        p->order = order;
        ret = mem->map(mem->arch, virtual, p);
        if ( ret < 0 ) {
            _data_free(mem, (union virt_memory_data *)p);
            goto error_page;
        }
        virtual += 1ULL << (order + MEMORY_PAGESIZE_SHIFT);
        physical += 1ULL << (order + MEMORY_PAGESIZE_SHIFT);

        *pp = p;
        pp = &p->next;
    }

    /* Add this entry */
    ret = _entry_add(&b->entries, e);
    if ( ret < 0 ) {
        goto error_page;
    }

    /* Remove the free entry */
    f = _free_delete(b, f);
    kassert( f != NULL );
    if ( f->start == e->start && f->size == e->size  ) {
        /* Remove the entire entry */
        _data_free(mem, (union virt_memory_data *)f0);
        _data_free(mem, (union virt_memory_data *)f1);
    } else if ( f->start == e->start ) {
        /* The start address is same, then add the rest to the free entry */
        f0->start = f->start + e->size;
        f0->size = f->size - e->size;
        ret = _free_add(b, f0);
        if ( ret < 0 ) {
            goto error_post;
        }
        _data_free(mem, (union virt_memory_data *)f1);
    } else if ( f->start + f->size == e->start + e->size ) {
        /* The end address is same, then add the rest to the free entry */
        f0->start = f->start;
        f0->size = f->size - e->size;
        ret = _free_add(b, f0);
        if ( ret < 0 ) {
            goto error_post;
        }
        _data_free(mem, (union virt_memory_data *)f1);
    } else {
        f0->start = f->start;
        f0->size = e->start + e->size - f->start;
        f1->start = e->start + e->size;
        f1->size = f->start + f->size - f1->start;
        ret = _free_add(b, f0);
        if ( ret < 0 ) {
            goto error_post;
        }
        ret = _free_add(b, f1);
        if ( ret < 0 ) {
            goto error_free;
        }
    }
    _data_free(mem, (union virt_memory_data *)f);

    return 0;

error_free:
    _free_delete(b, f0);
error_post:
    _entry_delete(&b->entries, e);
    /* Add it back */
    _free_add(b, f);
error_page:
    p = e->object->pages;
    virtual = e->start;
    while ( NULL != p ) {
        ret = mem->unmap(mem->arch, virtual, p);
        kassert(ret == 0);
        virtual += ((uintptr_t)MEMORY_PAGESIZE << p->order);
        _data_free(mem, (union virt_memory_data *)p);
        p = p->next;
    }
    _data_free(mem, (union virt_memory_data *)f1);
error_f1:
    _data_free(mem, (union virt_memory_data *)f0);
error_f0:
    _data_free(mem, (union virt_memory_data *)e->object);
error_obj:
    _data_free(mem, (union virt_memory_data *)e);
error_entry:
    return -1;
}

/*
 * Add a node to the free entry tree
 */
static int
_free_atree_add(virt_memory_free_t **t, virt_memory_free_t *n)
{
    if ( NULL == *t ) {
        /* Insert here */
        *t = n;
        n->atree.left = NULL;
        n->atree.right = NULL;
        return 0;
    }
    if ( n->start == (*t)->start ) {
        return -1;
    }
    if ( n->start > (*t)->start ) {
        return _free_atree_add(&(*t)->atree.right, n);
    } else {
        return _free_atree_add(&(*t)->atree.left, n);
    }
}
static int
_free_stree_add(virt_memory_free_t **t, virt_memory_free_t *n)
{
    if ( NULL == *t ) {
        /* Insert here */
        *t = n;
        n->stree.left = NULL;
        n->stree.right = NULL;
        return 0;
    } else if ( (*t)->size == n->size ) {
        /* Insert here */
        n->stree.left = *t;
        n->stree.right = NULL;
        *t = n;
        return 0;
    }
    if ( n->size > (*t)->size ) {
        return _free_stree_add(&(*t)->stree.right, n);
    } else {
        return _free_stree_add(&(*t)->stree.left, n);
    }
}
static int
_free_add(virt_memory_block_t *b, virt_memory_free_t *n)
{
    int ret;
    virt_memory_free_t *p;

    ret = _free_atree_add(&b->frees.atree, n);
    if ( ret < 0 ) {
        return -1;
    }
    ret = _free_stree_add(&b->frees.stree, n);
    if ( ret < 0 ) {
        p = _free_atree_delete(&b->frees.atree, n);
        kassert( p != NULL );
        return -1;
    }

    return 0;
}

/*
 * Delete the specified node from the free entry tree
 */
static virt_memory_free_t *
_free_atree_delete(virt_memory_free_t **t, virt_memory_free_t *n)
{
    virt_memory_free_t **x;

    if ( NULL == *t ) {
        /* Not found */
        return NULL;
    }
    if ( n == *t ) {
        if ( n->atree.left && n->atree.right ) {
            *t = n->atree.left;
            x = &n->atree.left;
            while ( NULL != *x ) {
                x = &(*x)->atree.right;
            }
            *x = n->atree.right;
        } else if ( n->atree.left ) {
            *t = n->atree.left;
        } else if ( n->atree.right ) {
            *t = n->atree.right;
        } else {
            *t = NULL;
        }
        return n;
    }
    if ( n->start > (*t)->start ) {
        return _free_atree_delete(&(*t)->atree.right, n);
    } else {
        return _free_atree_delete(&(*t)->atree.left, n);
    }
}
static virt_memory_free_t *
_free_stree_delete(virt_memory_free_t **t, virt_memory_free_t *n)
{
    virt_memory_free_t **x;

    if ( NULL == *t ) {
        return NULL;
    }
    if ( n == *t ) {
        if ( n->stree.left && n->stree.right ) {
            *t = n->stree.left;
            x = &n->stree.left;
            while ( NULL != *x ) {
                x = &(*x)->stree.right;
            }
            *x = n->stree.right;
        } else if ( n->stree.left ) {
            *t = n->stree.left;
        } else if ( n->stree.right ) {
            *t = n->stree.right;
        } else {
            *t = NULL;
        }
        return n;
    }
    if ( n->start > (*t)->start ) {
        return _free_stree_delete(&(*t)->stree.right, n);
    } else {
        return _free_stree_delete(&(*t)->stree.left, n);
    }
}
static virt_memory_free_t *
_free_delete(virt_memory_block_t *b, virt_memory_free_t *n)
{
    virt_memory_free_t *fa;
    virt_memory_free_t *fs;

    fa = _free_atree_delete(&b->frees.atree, n);
    fs = _free_stree_delete(&b->frees.stree, n);
    kassert( fa == fs );

    return fa;
}

/*
 * Search the tree by size
 */
static virt_memory_free_t *
_search_fit_size(virt_memory_block_t *block, virt_memory_free_t *t, size_t sz)
{
    virt_memory_free_t *s;

    if ( NULL == t ) {
        return NULL;
    }
    if ( sz > t->size ) {
        /* Search the right subtree */
        return _search_fit_size(block, t->stree.right, sz);
    } else {
        /* Search the left subtree */
        s = _search_fit_size(block, t->stree.left, sz);
        if ( NULL == s ) {
            /* Not found any block that fits this size, then return this node */
            return t;
        }
        return s;
    }
}

/*
 * Allocate pages from the block
 */
static void *
_alloc_pages_block(memory_t *mem, virt_memory_block_t *block, size_t nr,
                   int zone, int numadomain)
{
    int superpage;
    size_t size;
    virt_memory_free_t *f;
    virt_memory_free_t *f0;
    virt_memory_free_t *f1;
    virt_memory_entry_t *e;
    page_t **pp;
    page_t *p;
    void *r;
    size_t i;
    int ret;
    uintptr_t virtual;

    /* Search from the binary tree */
    size = nr * MEMORY_PAGESIZE;
    superpage = 0;
    if ( size >= MEMORY_SUPERPAGESIZE ) {
        /* Search larger size to make sure to align superpages */
        size += MEMORY_SUPERPAGESIZE;
        superpage = 1;
    }
    f = _search_fit_size(block, block->frees.stree, size);
    if ( NULL == f ) {
        /* No available space */
        return NULL;
    }

    /* Allocate an entry and an object */
    e = (virt_memory_entry_t *)_data_alloc(mem);
    if ( NULL == e ) {
        goto error_entry;
    }
    e->size = nr * MEMORY_PAGESIZE;
    e->object = (virt_memory_object_t *)_data_alloc(mem);
    if ( NULL == e->object ) {
        goto error_obj;
    }
    e->object->size = nr * MEMORY_PAGESIZE;
    e->object->pages = NULL;;

    /* Prepare for free spaces */
    f0 = (virt_memory_free_t *)_data_alloc(mem);
    if ( NULL == f0 ) {
        goto error_f0;
    }
    f1 = (virt_memory_free_t *)_data_alloc(mem);
    if ( NULL == f1 ) {
        goto error_f1;
    }

    /* Align for superpages */
    if ( superpage ) {
        if ( f->start & (MEMORY_SUPERPAGESIZE - 1) ) {
            /* Not aligned to superpage, then align */
            e->start = (f->start + (MEMORY_SUPERPAGESIZE - 1))
                & ~(uintptr_t)(MEMORY_SUPERPAGESIZE - 1);
        } else {
            /* Aligned to superpage */
            e->start = f->start;
        }
    } else {
        e->start = f->start;
    }

    /* Allocate and map superpages */
    pp = &e->object->pages;
    for ( i = 0;
          i + (1 << (MEMORY_SUPERPAGESIZE_SHIFT - MEMORY_PAGESIZE_SHIFT)) <= nr;
          i += (1 << (MEMORY_SUPERPAGESIZE_SHIFT - MEMORY_PAGESIZE_SHIFT)) ) {
        p = (page_t *)_data_alloc(mem);
        if ( NULL == p ) {
            goto error_page;
        }
        p->zone = zone;
        p->numadomain = numadomain;
        p->flags = 0;
        p->order = MEMORY_SUPERPAGESIZE_SHIFT - MEMORY_PAGESIZE_SHIFT;
        p->next = NULL;

        /* Allocate a physical superpage */
        r = phys_mem_alloc(mem->phys, p->order, p->zone, p->numadomain);
        if ( NULL == r ) {
            _data_free(mem, (union virt_memory_data *)p);
            goto error_page;
        }
        p->physical = (uintptr_t)r;

        /* Map */
        ret = mem->map(mem->arch, e->start + i * MEMORY_PAGESIZE, p);
        if ( ret < 0 ) {
            _data_free(mem, (union virt_memory_data *)p);
            phys_mem_free(mem->phys, (void *)p->physical, p->order, p->zone,
                          p->numadomain);
            goto error_page;
        }

        *pp = p;
        pp = &p->next;
    }

    /* Allocate and map pages */
    for ( ; i < nr; i++ ) {
        p = (page_t *)_data_alloc(mem);
        if ( NULL == p ) {
            goto error_page;
        }
        p->zone = zone;
        p->numadomain = numadomain;
        p->flags = 0;
        p->order = 0;
        p->next = NULL;

        /* Allocate a physical page */
        r = phys_mem_alloc(mem->phys, p->order, p->zone, p->numadomain);
        if ( NULL == r ) {
            _data_free(mem, (union virt_memory_data *)p);
            goto error_page;
        }
        p->physical = (uintptr_t)r;

        /* Map */
        mem->map(mem->arch, e->start + i * MEMORY_PAGESIZE, p);
        if ( ret < 0 ) {
            _data_free(mem, (union virt_memory_data *)p);
            phys_mem_free(mem->phys, (void *)p->physical, p->order, p->zone,
                          p->numadomain);
            goto error_page;
        }

        *pp = p;
        pp = &p->next;
    }

    /* Add this entry */
    ret = _entry_add(&block->entries, e);
    if ( ret < 0 ) {
        goto error_page;
    }

    /* Remove the free entry */
    f = _free_delete(block, f);
    kassert( f != NULL );
    if ( f->start == e->start && f->size == e->size  ) {
        /* Remove the entire entry */
        _data_free(mem, (union virt_memory_data *)f0);
        _data_free(mem, (union virt_memory_data *)f1);
    } else if ( f->start == e->start ) {
        /* The start address is same, then add the rest to the free entry */
        f0->start = f->start + e->size;
        f0->size = f->size - e->size;
        ret = _free_add(block, f0);
        if ( ret < 0 ) {
            goto error_post;
        }
    } else if ( f->start + f->size == e->start + e->size ) {
        /* The end address is same, then add the rest to the free entry */
        f0->start = f->start;
        f0->size = f->size - e->size;
        ret = _free_add(block, f0);
        if ( ret < 0 ) {
            goto error_post;
        }
        _data_free(mem, (union virt_memory_data *)f1);
    } else {
        f0->start = f->start;
        f0->size = e->start + e->size - f->start;
        f1->start = e->start + e->size;
        f1->size = f->start + f->size - f1->start;
        ret = _free_add(block, f0);
        if ( ret < 0 ) {
            goto error_post;
        }
        ret = _free_add(block, f1);
        if ( ret < 0 ) {
            goto error_free;
        }
    }
    _data_free(mem, (union virt_memory_data *)f);

    return (void *)e->start;

error_free:
    _free_delete(block, f0);
error_post:
    _entry_delete(&block->entries, e);
    /* Add it back */
    _free_add(block, f);
error_page:
    p = e->object->pages;
    virtual = e->start;
    while ( NULL != p ) {
        ret = mem->unmap(mem->arch, virtual, p);
        kassert(ret == 0);
        phys_mem_free(mem->phys, (void *)p->physical, p->order, p->zone,
                      p->numadomain);
        virtual += ((uintptr_t)MEMORY_PAGESIZE << p->order);
        _data_free(mem, (union virt_memory_data *)p);
        p = p->next;
    }
    _data_free(mem, (union virt_memory_data *)f1);
error_f1:
    _data_free(mem, (union virt_memory_data *)f0);
error_f0:
    _data_free(mem, (union virt_memory_data *)e->object);
error_obj:
    _data_free(mem, (union virt_memory_data *)e);
error_entry:
    return NULL;
}

/*
 * Allocate pages
 */
void *
memory_alloc_pages(memory_t *mem, size_t nr, int zone, int domain)
{
    virt_memory_block_t *block;
    void *ptr;

    block = mem->blocks;
    ptr = NULL;
    while ( NULL != block && NULL == ptr ) {
        ptr = _alloc_pages_block(mem, block, nr, zone, domain);
        block = block->next;
    }

    return ptr;
}

/*
 * Find the entry corresponding to the specified address
 */
static virt_memory_entry_t *
_find_entry(virt_memory_entry_t *e, uintptr_t addr)
{
    if ( NULL == e ) {
        return NULL;
    }
    if ( addr >= e->start && addr < e->start + e->size ) {
        /* Found */
        return e;
    }
    if ( addr < e->start ) {
        /* Search the left */
        return _find_entry(e->atree.left, addr);
    } else {
        /* Search the right */
        return _find_entry(e->atree.right, addr);
    }
}

/*
 * Free pages from the list of the specified page list
 */
static void
_pages_free(memory_t *mem, page_t *p)
{
    while ( NULL != p ) {
        /* Free physical pages */
        phys_mem_free(mem->phys, (void *)p->physical, p->order, p->zone,
                      p->numadomain);

        /* Free this page */
        _data_free(mem, (union virt_memory_data *)p);

        p = p->next;
    }
}

/*
 * Free an entry
 */
static int
_entry_free(memory_t *mem, virt_memory_block_t *b, virt_memory_entry_t *e)
{
    virt_memory_free_t free;
    virt_memory_free_t *f;
    void *r;
    int ret;

    /* Find a neighboring free entry by address */
    f = _find_neighbor_free_entry(b->frees.atree, e->start, e->start + e->size);
    if ( NULL == f ) {
        /* Not found, then convert the data structure to free entry */
        kmemset(&free, 0, sizeof(virt_memory_free_t));
        free.start = e->start;
        free.size = e->size;
        f = (virt_memory_free_t *)e;
        kmemcpy(f, &free, sizeof(virt_memory_free_t));

        /* Add this node to the free entry tree */
        ret = _free_add(b, f);
        if ( ret < 0 ) {
            return -1;
        }
    } else {
        /* Remove the node first */
        r = _free_delete(b, f);
        kassert( r != NULL );

        /* Expand the free region */
        if ( f->start == e->start + e->size ) {
            f->start = e->start;
            f->size = f->size + e->size;
        } else {
            f->size = f->size + e->size;
        }
        _data_free(mem, (union virt_memory_data*)e);

        /* Rebalance the size-based tree */
        ret = _free_add(b, f);
        kassert( ret == 0 );
    }

    return 0;
}

/*
 * Free pages
 */
void
memory_free_pages(memory_t *mem, void *ptr)
{
    virt_memory_block_t *b;
    uintptr_t addr;
    virt_memory_entry_t *e;
    page_t *page;
    void *r;

    /* Convert the pointer to the address in integer */
    addr = (uintptr_t)ptr;

    /* Find a block */
    b = _find_block(mem, addr);
    if ( NULL == b ) {
        /* Not found */
        return;
    }

    /* Find an entry corresponding to the virtual address */
    e = _find_entry(b->entries, addr);
    if ( NULL == e ) {
        /* Not found */
        return;
    }
    if ( addr != e->start ) {
        /* Start address does not match. */
        return;
    }

    /* Find pages from the object */
    page = e->object->pages;
    /* Free the corersponding pages */
    _pages_free(mem, page);
    /* Free the object */
    _data_free(mem, (union virt_memory_data *)e->object);
    /* Retuurn to the free entry */
    r = _entry_delete(&b->entries, e);
    kassert( r == e );
    _entry_free(mem, b, e);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
