/**
 * @file include/mcube/unrolled_list.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_UNROLLED_LIST_H__
#define __MCUBE_MCUBE_UNROLLED_LIST_H__

/*
 * Unrolled Linked List -- A linked list of small arrays!
 *
 * Copyright (C) 2012 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#ifndef __ASSEMBLY__

/**
 * @struct unrolled_node
 * @brief Singly-linked unrolled list node.
 *
 * NOTE! the data array could've been embedded in the node using
 * declaration @c void @a *array[], and putting that at structure end?
 */
struct unrolled_node {
  /**
   * Array of void pointers to data!
   */
  void **array;

  /**
   * Number of cells in the @a array (Redundant!).
   */
  uint array_len;

  /**
   * Number of free cells in the @a array.
   */
  uint array_nrfree;

  /**
   * Node number in the linked list, from 0.
   */
  uint num;

  /**
   * Next node in this list, or NULL.
   */
  struct unrolled_node *next;
};

/**
 * @struct unrolled_head
 * @brief Unrolled header.
 *
 * To use unrolled lists, embed this descriptor inside the
 * desired kernel structure.
 */
struct unrolled_head {
  /**
   * Singly-linked list of unrolled nodes.
   */
  struct unrolled_node *node;

  /**
   * Number of cells in each node array.
   */
  uint array_len;
};

/*
 * The exported API:
 * -----------------
 * unrolled_init(head, len)     :
 * unrolled_free(head)          :
 * unrolled_insert(head, val)   :
 * unrolled_lookup(head, key)   : given @key, get its '<key, val>' pair
 * unrolled_rm_key(head, key)   : Remove <@key, val> pair from list
 */

/**
 * @fn void unrolled_init(struct unrolled_head *head, uint array_len)
 * @brief initialize given unrolled list.
 *
 * @param head Unrolled linked list head
 * @param array_len Number of cells in each node's array
 */
void unrolled_init(struct unrolled_head *head, uint array_len);

/**
 * @fn void unrolled_free(struct unrolled_head *head)
 * @brief free all list storage.
 *
 * @param head Unrolled head.
 */
void unrolled_free(struct unrolled_head *head);

/**
 * @fn uint unrolled_insert(struct unrolled_head *head, void *val)
 * @brief insert @a val; generate new @a key for it!
 * NOTE! The returned mapping structure key starts from 0 and move upwards.
 * Any deleted key [usually as a result of close()] is directly re-used
 * when inserting new values, avoiding overflows in the keys space as much
 * as possible.
 *
 * @param head Unrolled linked list head.
 * @param val Value to be inserted; cannot be NULL.
 * @return "key" for the inserted value, forming a <key,val> pair.
 */
uint unrolled_insert(struct unrolled_head *head, void *val);


/**
 * @fn void *unrolled_lookup(struct unrolled_head *head, uint key)
 * @brief find the value attached with given key.
 *
 * @param head Unrolled linked list head.
 * @param key Key used to search the mapping structure.
 * @return Desired value, or NULL if key was invalid.
 */
void *unrolled_lookup(struct unrolled_head *head, uint key);


/**
 * @fn void unrolled_remove_key(struct unrolled_head *head, uint key)
 * @brief remove the <key,val> pair identified by @a key.
 *
 * @param head Unrolled linked list head.
 * @param key Key used to search the mapping structure.
 */
void unrolled_remove_key(struct unrolled_head *head, uint key);

/**
 * @def unrolled_for_each(head, val)
 * @brief iterate over all the unrolled list values.
 * Take care while modifying this code, we're quite bending C
 * and GNU extensions syntax to achieve it.
 *
 * @param head Unrolled linked list head.
 * @param val Type of 'void *', this's where the value will be put in each iteration.
 */
#define unrolled_for_each(head, val)                                    \
  for (struct unrolled_node *unrolled_node = (head)->node;              \
       unrolled_node;                                                   \
       unrolled_node = unrolled_node->next)                             \
    for (uint __i = 0,                                                  \
           __unused *_____c =                                           \
           ({                                                           \
             val = NULL;                                                \
             while (__i < unrolled_node->array_len && !(val = unrolled_node->array[__i])) \
               __i++; val;                                              \
           });                                                          \
         __i < unrolled_node->array_len;                                \
         __i++,                                                         \
           ({                                                           \
             val = NULL;                                                \
             while (__i < unrolled_node->array_len && !(val = unrolled_node->array[__i])) \
               __i++; val;                                              \
           }))


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_UNROLLED_LIST_H__ */
