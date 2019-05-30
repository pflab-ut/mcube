/**
 * @file include/mcube/llist.h
 *
 * @author Hiroyuki Chishiro
 */
/* This code is from https://github.com/CCareaga/heap_allocator */
#ifndef __MCUBE_MCUBE_LLIST_H__
#define __MCUBE_MCUBE_LLIST_H__

#ifndef __ASSEMBLY__

void add_heap_node(bin_t *bin, heap_node_t *heap_node);

void remove_heap_node(bin_t *bin, heap_node_t *heap_node);

heap_node_t *get_best_fit(bin_t *list, size_t size);
heap_node_t *get_last_heap_node(bin_t *list);

heap_node_t *next(heap_node_t *current);
heap_node_t *prev(heap_node_t *current);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_LLIST_H__ */
