/**
 * @file include/mcube/hash.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_HASH_H__
#define __MCUBE_MCUBE_HASH_H__

#ifndef __ASSEMBLY__

/**
 * @struct hash_elem
 * @param Hash element.
 */
struct hash_elem {
  /**
   * Unique ID for each hash element.
   */
  unsigned long id;

  /**
   * List node for hash fn collision.
   */
  struct list_node node;
};

/**
 * @struct hash
 * @param Hash information.
 */
struct hash {
  /**
   * Number of buckets in table.
   */
  int len;

  /**
   * Array (table) of list nodes.
   */
  struct list_node *nodes_array;
};



/**
 * @fn struct hash *hash_new(uint len)
 * @brief allocate hash table.
 * Assuming balanced distribution, a bigger @a len means less prabability for collision.
 *
 * @param len Hash table's length.
 */
struct hash *hash_new(uint len);

/**
 * @fn void hash_free(struct hash *hash)
 * @brief deallocate hash table.
 *
 * @param hash Hash table.
 */
void hash_free(struct hash *hash);


/**
 * @fn void hash_insert(struct hash *hash, void *elem)
 * @brief insert given element in the hash repository.
 *
 * @param hash Hash table.
 * @param elem Pointer to the structure to be inserted.
 */
void hash_insert(struct hash *hash, void *elem);

/**
 * @fn void *hash_find(struct hash *hash, uint64_t elem_id)
 * @brief find the element.
 *
 * @param hash Hash table.
 * @param elem_id Element ID.
 * @return NULL in case of non-existence.
 */
void *hash_find(struct hash *hash, uint64_t elem_id);

/**
 * @fn void hash_remove(struct hash *hash, uint64_t elem_id)
 * @brief remove element.
 *
 * @param hash Hash table.
 * @param elem_id Element ID.
 */
void hash_remove(struct hash *hash, uint64_t elem_id);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_HASH_H__ */
