/**
 * @file include/mcube/list.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_LIST_H__
#define __MCUBE_MCUBE_LIST_H__
/*
 * Type-generic doubly-linked lists
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * This API is very close to the Linux-2.6 one, which I've used back in
 * the days and found it to be very flexible. The code itself was
 * "cleanroomed" from Bovet & Cesati's `Understanding the Linux Kernel'
 * book to avoid copyrights mismatch.
 *
 * A good feature of this API is letting us allocate and deallocate both
 * the structure _and_ its linked-list pointers in one-shot. This avoids
 * unnecessary fragmentation in kmalloc() buffers, lessens the chance of
 * memory leaks, and improves locality of reference.
 *
 * Such lists typically look like this:
 *
 *
 *     .--------------------------------------------------------.
 *     |                                                        |
 *     |            struct A        struct B        struct C    |
 *     |           ..........      ..........      ..........   |
 *     |           .        .      .        .      .        .   |
 *     v           .        .      .        .      .        .   |
 *    ---          .  ---   .      .  ---   .      .  ---   .   |
 *    |@| --------->  |@| --------->  |@| --------->  |@| ------.
 *    | |          .  | |   .      .  | |   .      .  | |   .
 *    |*|  <--------- |*|  <--------- |*|  <--------- |*|  <----.
 *    ---          .  ---   .      .  ---   .      .  ---   .   |
 *    `H'          .  `n'   .      .  `n'   .      .  `n'   .   |
 *     |           ..........      ..........      ..........   |
 *     |                                                        |
 *     .--------------------------------------------------------.
 *
 *
 * where 'H' and 'n' are list_node structures, and 'H' is the list's
 * head. '@' is a node's next pointer, while '*' is the same node's
 * prev pointer. All of the next and prev pointers point to _other_
 * list_node objects, not to the super-objects A, B, or C.
 *
 * Check the test-cases for usage examples.
 */

#ifndef __ASSEMBLY__

/**
 * @struct list_node
 * @brief Doubly-linked list node.
 */
struct list_node {
  /**
   * Next list node.
   */
  struct list_node *next;

  /**
   * Previous list node.
   */
  struct list_node *prev;
};

/**
 * @def LIST_INIT(n)
 * @brief Static init, for inside-structure nodes
 *
 * @param n Address.
 */
#define LIST_INIT(n)                            \
  {                                             \
    .next = &(n),                               \
      .prev = &(n),                             \
  }

/**
 * @def LIST_NODE(n)
 * @brief Global declaration with a static init.
 *
 * @param n Address.
 */
#define LIST_NODE(n)                            \
  struct list_node n = LIST_INIT(n)

/**
 * @fn static inline void list_init(struct list_node *node)
 * @brief Dynamic init, for run-time.
 *
 * @param node List node.
 */
static inline void list_init(struct list_node *node)
{
  node->next = node;
  node->prev = node;
}

/**
 * @fn static inline bool list_empty(const struct list_node *node)
 * @brief Is this node connected with any neighbours?
 *
 * @param node List node.
 * @return True if list node is empty.
 */
static inline bool list_empty(const struct list_node *node)
{
  if (node->next == node) {
    assert(node->prev == node);
    return true;
  }

  assert(node->prev != node);
  return false;
}

/**
 * @fn static inline void list_add(struct list_node *node, struct list_node *new)
 * @brief insert @a new right after @a node.
 *
 * @param node Node.
 * @param new New node inserted after @a node.
 */
static inline void list_add(struct list_node *node, struct list_node *new)
{
  new->next = node->next;
  new->next->prev = new;

  node->next = new;
  new->prev = node;
}

/**
 * @fn static inline void list_add_tail(struct list_node *node, struct list_node *new)
 * @brief insert @a new right before @a node.
 *
 * @param node Node.
 * @param new New node inserted before @a node.
 */
static inline void list_add_tail(struct list_node *node, struct list_node *new)
{
  new->prev = node->prev;
  new->prev->next = new;

  node->prev = new;
  new->next = node;
}

/**
 * @def list_entry(node, type, node_name)
 * @brief return the address of the data structure of @a type that includes given @a node.
 * The "useless" pointer assignment is for type-checking.
 * `Make it hard to misuse' -- a golden APIs advice.
 *
 * @param node Node.
 * @param type Type.
 * @param node_name node's name inside that structure declaration.
 */
#define list_entry(node, type, node_name)       \
  ({                                            \
    size_t offset;                              \
    __unused struct list_node *m;               \
                                                \
    m = (node);                                 \
                                                \
    offset = offsetof(type, node_name);         \
    (type *)((uint8_t *)(node) - offset);       \
  })

/**
 * @def list_for_each(node, struc, name)
 * @brief scan the list, beginning from @a node, using the iterator @a struc.
 * NOTE! Don't delete the the iterator's list node inside
 * loop: we use it in the initialization of next iteration.
 *
 * @param node Node.
 * @param struc Ftye pointer to the structure.
 * @param name Node's name.
 */
#define list_for_each(node, struc, name)                            \
  for (struc = list_entry((node)->next, typeof(*struc), name);      \
       &(struc->name) != (node);                                    \
       struc = list_entry(struc->name.next, typeof(*struc), name))

/**
 * @def list_for_each_safe(node, struc, spare_struc, name)
 * @brief Same as list_for_each(), but with making it safe to
 * delete the iterator's list node inside the loop.
 * This is useful for popping-up list elements as you go.
 * You'll need to give us a spare iterator for this.
 *
 * @param node Node.
 * @param struc Structure.
 * @param spare_struc Spare structure.
 * @param name Name.
 */
#define list_for_each_safe(node, struc, spare_struc, name)              \
  for (struc = list_entry((node)->next, typeof(*struc), name),          \
         spare_struc = list_entry(struc->name.next, typeof(*struc), name); \
       &(struc->name) != (node);                                        \
       struc = spare_struc,                                             \
         spare_struc = list_entry(struc->name.next, typeof(*struc), name))

/**
 * @fn static inline void list_del(struct list_node *node)
 * @brief pop @a node out of its connected neighbours.
 *
 * @param node Node.
 */
static inline void list_del(struct list_node *node)
{
  struct list_node *prevn;
  struct list_node *nextn;

  prevn = node->prev;
  nextn = node->next;

  assert(prevn);
  assert(nextn);
  assert(prevn->next == node);
  assert(nextn->prev == node);

  prevn->next = node->next;
  nextn->prev = node->prev;

  node->next = node;
  node->prev = node;
}



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_LIST_H__ */

