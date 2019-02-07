/**
 * @file include/mcube/hash.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_HASH_H__
#define __MCUBE_MCUBE_HASH_H__

#ifndef __ASSEMBLY__

struct hash_elem {
  uint64_t id; /* Unique ID for each hash element */
  struct list_node node; /* List node for hash fn collision */
};

struct hash {
  int len; /* Number of buckets in table */
  struct list_node *nodes_array; /* Array (table) of list nodes */
};


struct hash *hash_new(uint len);
void hash_free(struct hash *hash);
void hash_insert(struct hash *hash, void *elem);
void *hash_find(struct hash *hash, uint64_t elem_id);
void hash_remove(struct hash *hash, uint64_t elem_id);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_HASH_H__ */
