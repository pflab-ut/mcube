/**
 * @file include/mcube/heap.h
 *
 * @author Hiroyuki Chishiro
 */
/* This code is from https://github.com/CCareaga/heap_allocator. */
#ifndef __MCUBE_MCUBE_HEAP_H__
#define __MCUBE_MCUBE_HEAP_H__

/**
 * @def HEAP_INIT_SIZE
 * @brief Heap's initialize size.
 */
#define HEAP_INIT_SIZE 0x10000

/**
 * @def HEAP_MAX_SIZE
 * @brief Heap's maximum size.
 */
#define HEAP_MAX_SIZE 0xf0000

/**
 * @def HEAP_MIN_SIZE
 * @brief Heap's minimum size.
 */
#define HEAP_MIN_SIZE 0x10000

/**
 * @def MIN_ALLOC_SZ
 * @brief Minimum allocation size.
 */
#define MIN_ALLOC_SZ 4

/**
 * @def MIN_WILDERNESS
 * @brief Minimum wilderness.
 */
#define MIN_WILDERNESS 0x2000

/**
 * @def MAX_WILDERNESS
 * @brief Maximum wilderness.
 */
#define MAX_WILDERNESS 0x1000000


/**
 * @def BIN_COUNT
 * @brief Number of bins.
 */
#define BIN_COUNT 9

/**
 * @def BIN_MAX_IDX
 * @brief Maximum index of bin.
 */
#define BIN_MAX_IDX (BIN_COUNT - 1)

#ifndef __ASSEMBLY__

/**
 * @struct heap_node
 * @brief Heap node information.
 */
struct heap_node {
  /**
   * Hole.
   */
  unsigned int hole;

  /**
   * Size.
   */
  unsigned int size;

  /**
   * Next heap.
   */
  struct heap_node *next;

  /**
   * Previous heap.
   */
  struct heap_node *prev;
};

/**
 * @typedef heap_node_t
 * @brief Typedef of @struct heap_node.
 */
typedef struct heap_node heap_node_t;

/**
 * @struct footer
 * @brief Footer information.
 */
struct footer {
  /**
   * Header.
   */
  heap_node_t *header;
};

/**
 * @typedef footer_t
 * @brief Typedef of @struct footer.
 */
typedef struct footer footer_t;

/**
 * @struct bin
 * @brief Bin information.
 */
struct bin {
  heap_node_t *head;
};

/**
 * @typedef bin_t
 * @brief Typedef of @struct bin.
 */
typedef struct bin bin_t;

/**
 * @struct heap
 * @brief Heap information.
 */
struct heap {
  long start;
  long end;
  bin_t bins[BIN_COUNT][sizeof(bin_t)];
};

/**
 * @typedef heap_t
 * @brief Typedef of @struct heap.
 */
typedef struct heap heap_t;

/**
 * @fn void init_heap(heap_t *heap, long start)
 * @brief initializes a new heap structure, provided an empty heap struct,
 *        and a place to start the heap.
 * NOTE: this function uses HEAP_INIT_SIZE to determine
 * how large the heap is so make sure the same constant
 * is used when allocating memory for your heap!
 *
 * @param heap Heap table.
 * @param start Start address.
 */
void init_heap(heap_t *heap, long start);

/**
 * @fn void *heap_alloc(heap_t *heap, size_t size)
 * @brief allocate heap.
 * It takes the heap struct pointer and the size of the chunk we
 * want. This function will search through the bins until
 * it finds a suitable chunk. it will then split the chunk
 * if neccesary and return the start of the chunk.
 *
 * @param heap Heap table.
 * @parma size Size.
 */
void *heap_alloc(heap_t *heap, size_t size);

/**
 * @fn void heap_free(heap_t *heap, void *p)
 * @brief free heap.
 * It takes the heap struct pointer and the pointer provided by the
 * heap_alloc function. The given chunk will be possibly
 * coalesced and then placed in the correct bin.
 *
 * @param heap Heap table.
 * @param p Pointer to free heap.
 */
void heap_free(heap_t *heap, void *p);

/**
 * @fn uint expand(heap_t *heap, size_t sz)
 * @brief expand heap.
 *
 * @param heap Heap table.
 * @param size Size.
 * @return True if success.
 */
uint expand(heap_t *heap, size_t size);

/**
 * @fn void contract(heap_t *heap, size_t size)
 * @brief contract heap.
 *
 * @param heap Heap table.
 * @param size Size.
 */
void contract(heap_t *heap, size_t size);

/**
 * @fn uint get_bin_index(size_t size)
 * @brief convert size => bin index.
 * Changing this function will change the binning policy of the heap.
 * Right now it just places any allocation < 8 in bin 0 and then for anything
 * above 8 it bins using the log base 2 of the size.
 *
 * @param size Size.
 * @return Bin index.
 */
uint get_bin_index(size_t size);

/**
 * this function will create a footer given heap node
 * the heap_node's size must be set to the correct value!
 */
void create_foot(heap_node_t *head);

/**
 * @fn footer_t *get_foot(heap_node_t *head)
 * @brief get the footer pointer given heap node.
 *
 * @param heap_node Heap node.
 * @return Pointer to footer pointer.
 *
 */
footer_t *get_foot(heap_node_t *heap_node);

/**
 * @fn heap_node_t *get_wilderness(heap_t *heap)
 * @brief get the wilderness heap node given a heap struct pointer.
 * NOTE: this function banks on the heap's end field being
 * correct, it simply uses the footer at the end of the
 * heap because that is always the wilderness.
 *
 * @param heap Heap node.
 * @return Wilderness heap node.
 */
heap_node_t *get_wilderness(heap_t *heap);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_HEAP_H__ */
