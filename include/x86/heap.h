/**
 * @file include/x86/heap.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_HEAP_H__
#define	__MCUBE_X86_HEAP_H__


//============================================================================
/// @brief      A simple heap memory manager
//
//  Copyright 2016 Brett Vickers.
//  Use of this source code is governed by a BSD-style license
//  that can be found in the MonkOS LICENSE file.
//============================================================================

#ifndef __ASSEMBLY__

typedef struct heap heap_t;

//----------------------------------------------------------------------------
//  @function   heap_create
/// @brief      Create a new heap from which to allocate virtual memory.
/// @param[in]  pt          The page table from which virtual memory is to be
///                         allocated.
/// @param[in]  vaddr       The virtual address of the first byte to use for
///                         the heap.
/// @param[in]  maxpages    The maximum number of pages that the heap will
///                         grow to fill.
/// @returns    A pointer to a the created heap structure.
//----------------------------------------------------------------------------
heap_t *heap_create(pagetable_t *pt, void *vaddr, uint64_t maxpages);

//----------------------------------------------------------------------------
//  @function   heap_destroy
/// @brief      Destroy a heap, returning its memory to page table.
/// @param[in]  heap        The heap to be destroyed.
//----------------------------------------------------------------------------
void heap_destroy(heap_t *heap);

//----------------------------------------------------------------------------
//  @function   heap_alloc
/// @brief      Allocate memory from a heap.
/// @param[in]  heap    The heap from which to allocate the memory.
/// @param[in]  size    The size, in bytes, of the allocation.
/// @returns    A pointer to a the allocated memory.
//----------------------------------------------------------------------------
void *heap_alloc(heap_t *heap, uint64_t size);

//----------------------------------------------------------------------------
//  @function   heap_free
/// @brief      Free memory previously allocated with heap_alloc.
/// @param[in]  heap    The heap from which the memory was allocated.
/// @param[in]  ptr     A pointer to the memory that was allocated.
//----------------------------------------------------------------------------
void heap_free(heap_t *heap, void *ptr);


// ALLOC_PAGES: The minimum number of pages to allocate each time the heap
// is grown.
#define ALLOC_PAGES     16

// block_header flags
#define FLAG_ALLOCATED  (1 << 0)

// round16 returns the first value x greater than or equal to n that satisfies
// (x mod 16) = r
#define round16(n, r)  (((((n) - (r) + 31) >> 4) << 4) + (r) - 16)

// total_bytes returns the total number of bytes contained in the block 'b',
// including the block header and footer.  'b' may be a pointer to a
// block_header or a block_footer struct.
#define total_bytes(b)                                          \
  ((b)->size + sizeof(block_header_t) + sizeof(block_footer_t))

struct heap {
  pagetable_t          *pt;           // page table that owns the heap
  void                 *vaddr;        // address of heap start
  uint64_t              pages;        // pages currently alloced to the heap
  uint64_t              maxpages;     // max pages used by the heap
  struct fblock_header *first_fblock; // first free block in the heap
  uint64_t              reserved;     // pad to a multiple of 16 bytes
};

typedef struct block_header {
  uint64_t size;          // size of block in bytes (minus header/footer)
  uint64_t flags;
} block_header_t;

typedef struct block_footer {
  uint64_t size;          // size of the preceding block
} block_footer_t;

typedef struct fblock_header {
  struct block_header   block;
  struct fblock_header *next_fblock;  // next free block in the heap
  struct fblock_header *prev_fblock;  // prev free block in the heap
} fblock_header_t;

#endif /* !__ASSEMBLY__ */


#endif	/* __MCUBE_X86_MM_H__ */
