/**
 * @file include/mcube/llist.h
 *
 * @author Hiroyuki Chishiro
 */
/* This code is from https://github.com/CCareaga/heap_allocator */
#ifndef __MCUBE_MCUBE_LLIST_H__
#define __MCUBE_MCUBE_LLIST_H__

#ifndef __ASSEMBLY__

/**
 * @fn void add_heap_node(bin_t *bin, heap_node_t *heap_node)
 * @brief add heap node.
 *
 * @param bin Bin.
 * @param heap_node Heap node.
 */
void add_heap_node(bin_t *bin, heap_node_t *heap_node);

/**
 * @fn void remove_heap_node(bin_t *bin, heap_node_t *heap_node)
 * @brief remove heap node.
 *
 * @param bin Bin.
 * @param heap_node Heap node.
 */
void remove_heap_node(bin_t *bin, heap_node_t *heap_node);

/**
 * @fn heap_node_t *get_best_fit(bin_t *list, size_t size)
 * @brief get best-fit bin.
 *
 * @param list Bin list.
 * @param size Size.
 * @return Node.
 */
heap_node_t *get_best_fit(bin_t *list, size_t size);

/**
 * @fn heap_node_t *get_last_heap_node(bin_t *list)
 * @brief get last heap node.
 *
 * @param list Bin list.
 * @return Node.
 */
heap_node_t *get_last_heap_node(bin_t *list);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_LLIST_H__ */
