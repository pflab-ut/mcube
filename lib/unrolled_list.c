/**
 * @file lib/unrolled_list.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Unrolled Linked List -- A linked list of small arrays!
 *
 * Copyright (C) 2012 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * This is an ad-hoc UnrolledLinkedList-like structure for storing a
 * thread's File Descriptor Table.  Its core advantages are:
 *
 *  - Semi-random access using indices as the search keys,  which is
 *        suitable for the Unix file descriptors [0->N] continuous range.
 *  - Automatic and efficient reuse of deleted keys (i.e., close()-d
 *        descriptors), thus saving memory space as much as possible.
 *
 * NOTE! This is mostly how it's done on Solaris: "The process-level [file
 * descriptor table] entries are allocated dynamically in groups of 24 as
 * files are opened, up to the per-process open file limit." --Jim Mauro
 *
 * NOTE! In Linux, a dynamic array is used with an incrementing pointer
 * that wraps at end, thus minimizing search time for a free FD number/idx.
 */

#include <mcube/mcube.h>

/**
 * @fn static struct unrolled_node *__unode_new(uint node_num, uint array_len)
 * @brief allocate and initialize a new list node.
 *
 * @param node_num Node number.
 * @param array_len Array length.
 * @return Node.
 */
static struct unrolled_node *__unode_new(uint node_num, uint array_len)
{
  struct unrolled_node *node;

  if (!(node = kmalloc(sizeof(struct unrolled_node)))) {
    panic("Error: cannot allocate memory %lu\n", sizeof(struct unrolled_node));
  }

  node->array = kmalloc(array_len * sizeof(void *));
  memset(node->array, 0, array_len * sizeof(void *));
  node->array_len = array_len;
  node->array_nrfree = array_len;
  node->num = node_num;
  node->next = NULL;
  return node;
}

/**
 * @fn static void __unode_free(struct unrolled_node *node)
 * @brief free all of a node's memory storage.
 *
 * @param node Node.
 */
static void __unode_free(struct unrolled_node *node)
{
  kfree(node->array);
  kfree(node);
}

/**
 * @fn static uint __unode_array_find_free_idx(struct unrolled_node *node)
 * @brief return index of first free cell in given node's array
 *
 * @param node Node.
 * @return Index of first free cell.
 */
static uint __unode_array_find_free_idx(struct unrolled_node *node)
{
  assert(node->array_nrfree > 0);

  for (uint i = 0; i < node->array_len; i++) {
    if (!node->array[i]) {
      return i;
    }
  }

  assert(false);
}

/**
 * @fn static void __unode_store_val_in_array(struct unrolled_node *node,
 *                                            uint array_idx,
 *                                            void *val)
 * @brief store value in array.
 *
 * @param node Node.
 * @param array_idx Array index.
 * @param val Value.
 */
static void __unode_store_val_in_array(struct unrolled_node *node,
                                       uint array_idx,
                                       void *val)
{
  assert(val);
  assert(array_idx < node->array_len);
  assert(!node->array[array_idx]);

  node->array_nrfree--;
  node->array[array_idx] = val;
}

/**
 * @fn __unused static void __unode_update_val_in_array(struct unrolled_node *node,
 *                                                      uint array_idx, void *val)
 * @brief update value in array.
 *
 * @param node Node.
 * @param array_idx Array index.
 * @param val Value.
 */
__unused static void __unode_update_val_in_array(struct unrolled_node *node,
                                                 uint array_idx, void *val)
{
  assert(val);
  assert(array_idx < node->array_len);
  assert(node->array[array_idx]);

  node->array[array_idx] = val;
}

/**
 * @fn static struct unrolled_node *__get_node(struct unrolled_head *head, uint key,
 *                                             uint *array_idx)
 * @brief get node.
 *
 * @param head Head.
 * @param key Key.
 * @param array_idx Array index.
 * @return Node holding given @a key's value, and the index
 * within such node's array of data, and NULL if the @a key was out of range.
 */
static struct unrolled_node *__get_node(struct unrolled_head *head, uint key,
                                        uint *array_idx)
{
  struct unrolled_node *node;
  uint node_num;

  node = head->node;

  if (!node) {      /* Empty list */
    return NULL;
  }

  node_num = key / node->array_len;
  *array_idx = key % node->array_len;

  while (node_num--) {
    if (!node) {
      /* Out of Range key */
      return NULL;
    }

    node = node->next;
  }

  return node;
}

/*
 * The Public API starts here:
 */

void unrolled_init(struct unrolled_head *head, uint array_len)
{
  head->node = NULL;
  head->array_len = array_len;
  assert(head->array_len > 0);
}

/*
 * unrolled_free - Free _all_ of the list storage!
 */
void unrolled_free(struct unrolled_head *head)
{
  struct unrolled_node *node, *prev;

  node = head->node;

  while (node) {
    prev = node;
    node = node->next;
    __unode_free(prev);
  }
}

uint unrolled_insert(struct unrolled_head *head, void *val)
{
  struct unrolled_node *node, *prev;
  uint idx;

  if (!head->node) {
    head->node = __unode_new(0, head->array_len);
  }

  assert(val);
  assert(head->node);

  for (node = head->node; node; node = node->next) {
    if (node->array_nrfree > 0) {
      idx = __unode_array_find_free_idx(node);
      __unode_store_val_in_array(node, idx, val);
      return (node->num * node->array_len) + idx;
    }

    prev = node;
  }

  /* No node with free array space was found */
  node = __unode_new(prev->num + 1, prev->array_len);
  __unode_store_val_in_array(node, 0, val);
  prev->next = node;
  return node->num * node->array_len;
}

void *unrolled_lookup(struct unrolled_head *head, uint key)
{
  struct unrolled_node *node;
  uint array_idx;

  node = __get_node(head, key, &array_idx);

  if (!node) {
    return node;
  }

  return node->array[array_idx];
}

void unrolled_remove_key(struct unrolled_head *head, uint key)
{
  struct unrolled_node *node;
  uint array_idx;

  node = __get_node(head, key, &array_idx);

  if (!node || !node->array[array_idx]) {
    panic("UNROLLED: Tried  to remove non-existing mapping structure key %u", key);
  }

  node->array_nrfree++;
  node->array[array_idx] = NULL;
}

