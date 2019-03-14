/**
 * @file lib/llist.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void add_node(bin_t *bin, node_t *node)
{
  node->next = NULL;
  node->prev = NULL;

  if (bin->head == NULL) {
    bin->head = node;
    return;
  }
    
  // we need to save next and prev while we iterate
  node_t *current_node = bin->head;
  node_t *previous = NULL;
  // iterate until we get the the end of the list or we find a 
  // node whose size is
  while (current_node != NULL && current_node->size <= node->size) {
    previous = current_node;
    current_node = current_node->next;
  }

  if (current_node == NULL) { // we reached the end of the list
    previous->next = node;
    node->prev = previous;
  } else {
    if (previous != NULL) { // middle of list, connect all links!
      node->next = current_node;
      previous->next = node;

      node->prev = previous;
      current_node->prev = node;
    } else { // head is the only element
      node->next = bin->head;
      bin->head->prev = node;
      bin->head = node;
    }
  }
}

void remove_node(bin_t * bin, node_t *node)
{
  if (bin->head == NULL) {
    return;
  }
  if (bin->head == node) { 
    bin->head = bin->head->next;
    return;
  }
    
  node_t *tmp = bin->head->next;
  while (tmp != NULL) {
    if (tmp == node) { // found the node
      if (tmp->next == NULL) { // last item
        tmp->prev->next = NULL;
      }
      else { // middle item
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
      }
      // we dont worry about deleting the head here because we already checked that
      return;
    }
    tmp = tmp->next;
  }
}

node_t *get_best_fit(bin_t *bin, size_t size)
{
  if (bin->head == NULL) {
    return NULL; // empty list!
  }

  node_t *tmp = bin->head;

  while (tmp != NULL) {
    if (tmp->size >= size) {
      return tmp; // found a fit!
    }
    tmp = tmp->next;
  }
  return NULL; // no fit!
}

node_t *get_last_node(bin_t *bin)
{
  node_t *tmp = bin->head;

  while (tmp->next != NULL) {
    tmp = tmp->next;
  }
  return tmp;
}

