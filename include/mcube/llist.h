/**
 * @file include/mcube/llist.h
 *
 * @author Hiroyuki Chishiro
 */
/* This code is from https://github.com/CCareaga/heap_allocator */
#ifndef __MCUBE_MCUBE_LLIST_H__
#define __MCUBE_MCUBE_LLIST_H__

#ifndef __ASSEMBLY__

void add_node(bin_t *bin, node_t *node);

void remove_node(bin_t *bin, node_t *node);

node_t *get_best_fit(bin_t *list, size_t size);
node_t *get_last_node(bin_t *list);

node_t *next(node_t *current);
node_t *prev(node_t *current);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_LLIST_H__ */
