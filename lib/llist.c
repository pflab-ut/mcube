/**
 * @file lib/llist.c
 *
 * @author Hiroyuki Chishiro
 */
/* This code is from https://github.com/CCareaga/heap_allocator */
#include <mcube/mcube.h>

void add_heap_node(bin_t *bin, heap_node_t *heap_node)
{
  heap_node->next = NULL;
  heap_node->prev = NULL;

  if (!bin->head) {
    bin->head = heap_node;
    return;
  }

  // we need to save next and prev while we iterate
  heap_node_t *current_heap_node = bin->head;
  heap_node_t *previous = NULL;

  // iterate until we get the the end of the list or we find a
  // heap_node whose size is
  while (current_heap_node && current_heap_node->size <= heap_node->size) {
    previous = current_heap_node;
    current_heap_node = current_heap_node->next;
  }

  if (!current_heap_node) { // we reached the end of the list
    previous->next = heap_node;
    heap_node->prev = previous;
  } else {
    if (previous) { // middle of list, connect all links!
      heap_node->next = current_heap_node;
      previous->next = heap_node;

      heap_node->prev = previous;
      current_heap_node->prev = heap_node;
    } else { // head is the only element
      heap_node->next = bin->head;
      bin->head->prev = heap_node;
      bin->head = heap_node;
    }
  }
}

void remove_heap_node(bin_t *bin, heap_node_t *heap_node)
{
  if (!bin->head) {
    return;
  }

  if (bin->head == heap_node) {
    bin->head = bin->head->next;
    return;
  }

  heap_node_t *tmp = bin->head->next;

  while (tmp) {
    if (tmp == heap_node) { // found the heap_node
      if (!tmp->next) { // last item
        tmp->prev->next = NULL;
      } else { // middle item
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
      }

      // we dont worry about deleting the head here because we already checked that
      return;
    }

    tmp = tmp->next;
  }
}

heap_node_t *get_best_fit(bin_t *bin, size_t size)
{
  if (!bin->head) {
    return NULL; // empty list!
  }

  heap_node_t *tmp = bin->head;

  while (tmp) {
    if (tmp->size >= size) {
      return tmp; // found a fit!
    }

    tmp = tmp->next;
  }

  return NULL; // no fit!
}

heap_node_t *get_last_heap_node(bin_t *bin)
{
  heap_node_t *tmp = bin->head;

  while (tmp->next) {
    tmp = tmp->next;
  }

  return tmp;
}

