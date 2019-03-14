/**
 * @file include/mcube/heap.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_HEAP_H__
#define __MCUBE_MCUBE_HEAP_H__

#define HEAP_INIT_SIZE 0x10000
#define HEAP_MAX_SIZE 0xF0000
#define HEAP_MIN_SIZE 0x10000

#define MIN_ALLOC_SZ 4

#define MIN_WILDERNESS 0x2000
#define MAX_WILDERNESS 0x1000000

#define BIN_COUNT 9
#define BIN_MAX_IDX (BIN_COUNT - 1)

#ifndef __ASSEMBLY__

typedef struct node {
  unsigned int hole;
  unsigned int size;
  struct node* next;
  struct node* prev;
} node_t;

typedef struct { 
  node_t *header;
} footer_t;

typedef struct {
  node_t* head;
} bin_t;

typedef struct {
  long start;
  long end;
  bin_t bins[BIN_COUNT][sizeof(bin_t)];
} heap_t;


void init_heap(heap_t *heap, long start);

void *heap_alloc(heap_t *heap, size_t size);
void heap_free(heap_t *heap, void *p);
int expand(heap_t *heap, size_t sz);
void contract(heap_t *heap, size_t sz);

int get_bin_index(size_t sz);
void create_foot(node_t *head);
footer_t *get_foot(node_t *head);

node_t *get_wilderness(heap_t *heap);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_HEAP_H__ */
