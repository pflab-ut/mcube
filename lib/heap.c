/**
 * @file lib/heap.c
 *
 * @author Hiroyuki Chishiro
 */
/* This code is from https://github.com/CCareaga/heap_allocator. */
#include <mcube/mcube.h>


static uint overhead = sizeof(footer_t) + sizeof(heap_node_t);
uint offset = 8;

void init_heap(heap_t *heap, long start)
{
  // first we create the initial region, this is the "wilderness" chunk
  // the heap starts as just one big chunk of allocatable memory
  heap_node_t *init_region = (heap_node_t *) start;
  init_region->hole = 1;
  init_region->size = (HEAP_INIT_SIZE) - sizeof(heap_node_t) - sizeof(footer_t);

  create_foot(init_region); // create a foot (size must be defined)

  // now we add the region to the correct bin and setup the heap struct
  add_heap_node(heap->bins[get_bin_index(init_region->size)], init_region);

  heap->start = start;
  heap->end   = start + HEAP_INIT_SIZE;
}

void *heap_alloc(heap_t *heap, size_t size)
{
  // first get the bin index that this chunk size should be in
  uint index = get_bin_index(size);
  // now use this bin to try and find a good fitting chunk!
  bin_t *temp = (bin_t *) heap->bins[index];
  heap_node_t *found = get_best_fit(temp, size);

  // while no chunk if found advance through the bins until we
  // find a chunk or get to the wilderness
  while (!found) {
    temp = heap->bins[++index];
    found = get_best_fit(temp, size);
  }


  // if the differnce between the found chunk and the requested chunk
  // is bigger than the overhead (metadata size) + the min alloc size
  // then we should split this chunk, otherwise just return the chunk
  if ((found->size - size) > (overhead + MIN_ALLOC_SZ)) {
    // do the math to get where to split at, then set its metadata
    heap_node_t *split = (heap_node_t *)(((char *) found + sizeof(
                                            heap_node_t) + sizeof(
                                            footer_t)) + size);
    split->size = found->size - size - sizeof(heap_node_t) - sizeof(footer_t);
    split->hole = 1;

    create_foot(split); // create a footer for the split

    // now we need to get the new index for this split chunk
    // place it in the correct bin
    uint new_idx = get_bin_index(split->size);

    add_heap_node(heap->bins[new_idx], split);

    found->size = size; // set the found chunks size
    create_foot(found); // since size changed, remake foot
  }

  found->hole = 0; // not a hole anymore
  remove_heap_node(heap->bins[index], found); // remove it from its bin

  // these following lines are checks to determine if the heap should
  // be expanded or contracted
  // ==========================================
  heap_node_t *wild = get_wilderness(heap);

  if (wild->size < MIN_WILDERNESS) {
    uint success = expand(heap, 0x1000);

    if (success == 0) {
      return NULL;
    }
  } else if (wild->size > MAX_WILDERNESS) {
    contract(heap, 0x1000);
  }

  // ==========================================

  // since we don't need the prev and next fields when the chunk
  // is in use by the user, we can clear these and return the
  // address of the next field
  found->prev = NULL;
  found->next = NULL;
  return &found->next;
}

void heap_free(heap_t *heap, void *p)
{
  bin_t *list;
  footer_t *new_foot, *old_foot;

  // the actual head of the heap_node is not p, it is p minus the size
  // of the fields that precede "next" in the heap_node structure
  // if the heap_node being free is the start of the heap then there is
  // no need to coalesce so just put it in the right list
  heap_node_t *head = (heap_node_t *)((char *) p - offset);

  if (head == (heap_node_t *)(uintptr_t) heap->start) {
    head->hole = 1;
    add_heap_node(heap->bins[get_bin_index(head->size)], head);
    return;
  }

  // these are the next and previous heap_nodes in the heap, not the prev and next
  // in a bin. to find prev we just get subtract from the start of the head heap_node
  // to get the footer of the previous heap_node (which gives us the header pointer).
  // to get the next heap_node we simply get the footer and add the sizeof(footer_t).
  heap_node_t *next = (heap_node_t *)((char *) get_foot(head) + sizeof(footer_t));
  footer_t *f = (footer_t *)((char *) head - sizeof(footer_t));
  heap_node_t *prev = f->header;

  // if the previous heap_node is a hole we can coalese!
  if (prev->hole) {
    // remove the previous heap_node from its bin
    list = heap->bins[get_bin_index(prev->size)];
    remove_heap_node(list, prev);

    // re-calculate the size of thie heap_node and recreate a footer
    prev->size += overhead + head->size;
    new_foot = get_foot(head);
    new_foot->header = prev;

    // previous is now the heap_node we are working with, we head to prev
    // because the next if statement will coalesce with the next heap_node
    // and we want that statement to work even when we coalesce with prev
    head = prev;
  }

  // if the next heap_node is free coalesce!
  if (next->hole) {
    // remove it from its bin
    list = heap->bins[get_bin_index(next->size)];
    remove_heap_node(list, next);

    // re-calculate the new size of head
    head->size += overhead + next->size;

    // clear out the old metadata from next
    old_foot = get_foot(next);
    old_foot->header = 0;
    next->size = 0;
    next->hole = 0;

    // make the new footer!
    new_foot = get_foot(head);
    new_foot->header = head;
  }

  // this chunk is now a hole, so put it in the right bin!
  head->hole = 1;
  add_heap_node(heap->bins[get_bin_index(head->size)], head);
}

// these are left here to implement contraction / expansion
uint expand(__unused heap_t *heap, __unused size_t size)
{
  return 0;
}

void contract(__unused heap_t *heap, __unused size_t size)
{
  return;
}

uint get_bin_index(size_t size)
{
  uint index = 0;
  size = size < 4 ? 4 : size;

  while (size >>= 1) {
    index++;
  }

  index -= 2;

  if (index > BIN_MAX_IDX) {
    index = BIN_MAX_IDX;
  }

  return index;
}

void create_foot(heap_node_t *head)
{
  footer_t *foot = get_foot(head);
  foot->header = head;
}

footer_t *get_foot(heap_node_t *heap_node)
{
  return (footer_t *)((char *) heap_node + sizeof(heap_node_t) + heap_node->size);
}

heap_node_t *get_wilderness(heap_t *heap)
{
  footer_t *wild_foot = (footer_t *)((char *) heap->end - sizeof(footer_t));
  return wild_foot->header;
}
