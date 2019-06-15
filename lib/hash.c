/**
 * @file lib/hash.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * A Simple Hash Table Implementation
 *
 * Copyright (C) 2012 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * The modulo operator is used as the hash function.
 */

#include <mcube/mcube.h>

struct hash *hash_new(uint len)
{
  struct hash *hash;

  if (!(hash = kmalloc(sizeof(*hash)))) {
    panic("Error: cannot allocate memory %lu\n", sizeof(*hash));
  }

  hash->nodes_array = kmalloc(len * sizeof(*hash->nodes_array));

  for (uint i = 0; i < len; i++) {
    list_init(&hash->nodes_array[i]);
  }

  hash->len = len;
  return hash;
}


void hash_free(struct hash *hash)
{
  assert(hash);
  kfree(hash->nodes_array);
  kfree(hash);
}

/*
 * Find the element identified by @elem_id in the given hash
 * repository.  Return NULL in case of non-existence.
 */
static void *hash_find_elem(struct hash *hash, uint elem_id)
{
  struct hash_elem *helem = NULL;
  int idx;

  assert(hash);
  idx = elem_id % hash->len;
  list_for_each(&hash->nodes_array[idx], helem, node) {
    if (helem->id == elem_id) {
      return helem;
    }
  }
  return NULL;
}

void hash_insert(struct hash *hash, void *elem)
{
  struct hash_elem *helem;
  int idx;

  helem = elem;

  if (hash_find_elem(hash, helem->id)) {
    panic("Hash: Inserting element with ID #%lu, which "
          "already exists!", helem->id);
  }

  idx = helem->id % hash->len;
  assert(list_empty(&helem->node));
  list_add(&hash->nodes_array[idx], &helem->node);
}

void *hash_find(struct hash *hash, uint64_t elem_id)
{
  return hash_find_elem(hash, elem_id);
}

void hash_remove(struct hash *hash, uint64_t elem_id)
{
  struct hash_elem *helem;

  helem = hash_find_elem(hash, elem_id);

  if (!helem) {
    panic("Hash: Removing non-existent element identified by #%lu\n", elem_id);
  }

  list_del(&helem->node);
}

