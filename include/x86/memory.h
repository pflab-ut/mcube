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

#ifndef _ADVOS_KERNEL_MEMORY_H
#define _ADVOS_KERNEL_MEMORY_H


#define MEMORY_ZONE_UNKNOWN             -1
#define MEMORY_ZONE_DMA                 0
#define MEMORY_ZONE_KERNEL              1
#define MEMORY_ZONE_CORE_NUM            2
#define MEMORY_ZONE_NUMA_AWARE          2

#define MEMORY_ZONE_KERNEL_LB           0x01000000
#define MEMORY_ZONE_NUMA_AWARE_LB       0x04000000

#define MEMORY_PHYS_BUDDY_ORDER         18

#define MEMORY_PAGESIZE_SHIFT           12
#define MEMORY_PAGESIZE                 (1ULL << MEMORY_PAGESIZE_SHIFT)
#define MEMORY_SUPERPAGESIZE_SHIFT      21
#define MEMORY_SUPERPAGESIZE            (1ULL << MEMORY_SUPERPAGESIZE_SHIFT)

/* Page flags */
#define MEMORY_PGF_WIRED                (1 << 0)

#define MEMORY_SLAB_NUM_PAGES           8
#define MEMORY_SLAB_CACHE_NAME_MAX      64
#define MEMORY_SLAB_ALIGNMENT           64 /* Cacheline size (must be 2^n) */
#define MEMORY_SLAB_CACHE_NAME          "slab_cache"

#ifndef __ASSEMBLY__

/*
 * PAGE
 */
struct _phys_memory_buddy_page {
    /* Linked list's next pointer (virtual address) */
    struct _phys_memory_buddy_page *next;
} __attribute__ ((packed));
typedef struct _phys_memory_buddy_page phys_memory_buddy_page_t;

/*
 * Physical memory zone
 */
typedef struct {
    /* Flag to note the zone is initialized or not */
    int valid;

    /* Head pointers to page blocks at each order of buddy system */
    phys_memory_buddy_page_t *heads[MEMORY_PHYS_BUDDY_ORDER + 1];
} phys_memory_zone_t;

/*
 * Physical memory management
 */
typedef struct {
    /* Offset to calculate virtual address from physical address */
    uintptr_t p2v;

    /* Core zones */
    phys_memory_zone_t czones[MEMORY_ZONE_CORE_NUM];

    /* NUMA zones */
    int max_domain;
    phys_memory_zone_t *numazones;
} phys_memory_t;

/*
 * System memory map entry
 */
typedef struct {
    uint64_t base;
    uint64_t len;
    uint32_t type;
    uint32_t attr;
} __attribute__ ((packed)) memory_sysmap_entry_t;

/*
 * Page
 */
typedef struct page page_t;
struct page {
    /* Physical address */
    uintptr_t physical;
    /* Flags */
    uint16_t flags;
    /* Zone of the buddy system */
    uint8_t zone;
    /* Order of the buddy system */
    uint8_t order;
    /* Numa-domain if available */
    uint32_t numadomain;
    /* Pointer to the next page in the same object */
    page_t *next;
};

/*
 * Object
 */
typedef struct virt_memory_object virt_memory_object_t;
struct virt_memory_object {
    /* Pointer to the list of pages */
    page_t *pages;
    /* Size */
    size_t size;
};

/*
 * Virtual memory entry (allocated)
 */
typedef struct virt_memory_entry virt_memory_entry_t;
struct virt_memory_entry {
    /* Start virtual address of this entry */
    uintptr_t start;
    /* Size */
    size_t size;
    /* Pointer to the object */
    virt_memory_object_t *object;

    /* Binary tree for start address ordering */
    struct {
        virt_memory_entry_t *left;
        virt_memory_entry_t *right;
    } atree;
};

/*
 * Free spacw management
 */
typedef struct virt_memory_free virt_memory_free_t;
struct virt_memory_free {
    /* Start address of this space */
    uintptr_t start;
    /* Size of this space */
    size_t size;

    /* Binary tree for start address ordering */
    struct {
        virt_memory_free_t *left;
        virt_memory_free_t *right;
    } atree;
    /* Binary tree for size ordering */
    struct {
        virt_memory_free_t *left;
        virt_memory_free_t *right;
    } stree;
};

/*
 * Memory block
 */
typedef struct virt_memory_block virt_memory_block_t;
struct virt_memory_block {
    /* Start virtual address of this block */
    uintptr_t start;
    /* End virtual address of this block */
    uintptr_t end;
    /* Pointer to the next block */
    virt_memory_block_t *next;

    /* Allocated entries */
    virt_memory_entry_t *entries;

    /* Free space list */
    struct {
        virt_memory_free_t *atree;
        virt_memory_free_t *stree;
    } frees;
};

/*
 * Data structure used for virtual memory management
 */
union virt_memory_data {
    page_t page;
    virt_memory_object_t object;
    virt_memory_entry_t entry;
    virt_memory_free_t free;
    virt_memory_block_t block;
    union virt_memory_data *next;
};

/*
 * Memory
 */
typedef struct {
    /* List of blocks */
    virt_memory_block_t *blocks;

    /* Physical memory manager */
    phys_memory_t *phys;

    /* List of memory management data structures */
    union virt_memory_data *lists;

    /* Architecture-specific defintions */
    void *arch;
    int (*map)(void *, uintptr_t, page_t *);
    int (*unmap)(void *, uintptr_t, page_t *);
} memory_t;

/*
 * Define memory_slab_cache_t first
 */
typedef struct memory_slab_cache memory_slab_cache_t;

/*
 * Slab header
 */
typedef struct memory_slab_hdr memory_slab_hdr_t;
struct memory_slab_hdr {
    /* Pointer to the next slab header */
    memory_slab_hdr_t *next;
    /* Parent cache */
    memory_slab_cache_t *cache;
    /* The number of objects in this slab */
    int nobjs;
    /* The number of allocated objects */
    int nused;
    /* Pointer to the first object in this slab */
    void *obj_head;
    /* Free marks follows (nobjs byte) */
    uint8_t marks[0];
    /* Objects follow */
};

/*
 * free list of slab objects
 */
typedef struct {
    memory_slab_hdr_t *partial;
    memory_slab_hdr_t *full;
    memory_slab_hdr_t *empty;
} memory_slab_free_list_t;

/*
 * Slab cache
 */
struct memory_slab_cache {
    char name[MEMORY_SLAB_CACHE_NAME_MAX];
    size_t size;
    memory_slab_free_list_t freelist;
    /* Search tree */
    memory_slab_cache_t *left;
    memory_slab_cache_t *right;
};

/*
 * Slab allocator
 */
typedef struct {
    memory_t *mem;
    memory_slab_cache_t *root;
} memory_slab_allocator_t;

/* Defined in physmem.c */
void
phys_mem_buddy_add_region(phys_memory_buddy_page_t **, uintptr_t, uintptr_t);
void * phys_mem_buddy_alloc(phys_memory_buddy_page_t **, int);
void phys_mem_buddy_free(phys_memory_buddy_page_t **, void *, int);
void * phys_mem_alloc(phys_memory_t *, int, int, int);
void phys_mem_free(phys_memory_t *, void *, int, int, int);
int phys_memory_init(phys_memory_t *, int, memory_sysmap_entry_t *, uint64_t);

/* Defined in memory.c */
int memory_init(memory_t *, phys_memory_t *, void *,
                int (*map)(void *, uintptr_t, page_t *),
                int (*unmap)(void *, uintptr_t, page_t *));
int memory_block_add(memory_t *, uintptr_t, uintptr_t);
int memory_wire(memory_t *, uintptr_t, size_t, uintptr_t);
void * memory_alloc_pages(memory_t *, size_t, int, int);
void memory_free_pages(memory_t *, void *);

/* To be implemented */
void * kmalloc(size_t);
void kfree(void *);

/* Defined in slab.c */
int memory_slab_init(memory_slab_allocator_t *, memory_t *);
void * memory_slab_alloc(memory_slab_allocator_t *, const char *);
void memory_slab_free(memory_slab_allocator_t *, const char *, void *);
int memory_slab_create_cache(memory_slab_allocator_t *, const char *, size_t);

#endif /* !__ASSEMBLY__ */

#endif

