/**
 * @file user/test/test_hash.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Copyright (C) 2012 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#include <mcube/mcube.h>


static void hash_print_info(struct hash *hash)
{
  struct hash_elem *helem = NULL;
  uint64_t count;

  print_uart("Printing Hash info:\n");
  print_uart("Hash Address: 0x%lx\n", hash);
  print_uart("Hash Array Length: %d\n", hash->len);

  for (int i = 0; i < hash->len; i++) {
    print_uart("Number of Elemenets in Hash List #%d = ", i);
    count = 0;
    list_for_each(&hash->nodes_array[i], helem, node) {
      count++;
    }
    print_uart("%lu\n", count);
  }
}

struct test_struct {
  uint64_t num;
  struct list_node node;
  int payload;
};

static void test_hash_op(int hash_size)
{
  struct hash *hash;
  struct test_struct *array;
  struct test_struct *elem;
  int count;

  count = 128;
  hash = hash_new(hash_size);

  if ((array = kmalloc(count * sizeof(*array))) == NULL) {
    panic("Error: cannot allocate memory %lu\n", sizeof(*array));
  }

  for (int i = 0; i < count; i++) {
    array[i].num = i;
    array[i].payload = i;
    list_init(&array[i].node);
    hash_insert(hash, &array[i]);
  }

  for (int i = count - 1; i >= 0; i--) {
    elem = hash_find(hash, i);

    if (elem == NULL) {
      panic("_Hash: Cannot find element #%u, although "
            "it was previously inserted!", i);
    }

    if (elem->num != (unsigned) i) {
      panic("_Hash: Element returned by searching hash for "
            "id #%lu returned element with id #%lu!", i,
            elem->num);
    }

    if (elem->payload != i) {
      panic("_Hash: Element returned by searching hash for "
            "id #%lu has a valid id, but its payload got "
            "corrupted from %d to %d!", i, i, elem->payload);
    }
  }

  if ((elem = hash_find(hash, INT32_MAX)) != NULL) {
    panic("_Hash: Returning non-existing element with id %d as "
          "found, with payload = %d", INT32_MAX, elem->payload);
  }

  hash_print_info(hash);

  hash_free(hash);
  kfree(array);
}

bool test_hash(void)
{
  for (int i = 1; i <= 256; i++) {
    printk("_Hash: Testing hash with size '%d': ", i);
    test_hash_op(i);
    printk("Success!\n");
  }

  /* Should panic: */
  /* struct hash *hash = hash_new(5); */
  /* struct test_struct element; element.num = 5; */
  /* list_init(&element.node); */
  /* hash_insert(hash, &element); hash_insert(hash, &element); */
  return true;
}

