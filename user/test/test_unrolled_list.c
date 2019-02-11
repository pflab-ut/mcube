/**
 * @file user/test/test_unrolled_list.c
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


/*
 * -------------------- Testcases! --------------------
 */


/*
 * Assure that list iterator pass on all list values
 */
static void _test_N_elements(struct unrolled_head *head, uint len)
{
  void *val = NULL;
  uintptr_t ret;

  printk("_UNROLLED: _test_N_elements(%u): ", len);
  for (uintptr_t i = 0; i < len; i++) {
    unrolled_insert(head, (void *)(i + 5));
  }
  uint i = 0;
  unrolled_for_each(head, val) {
    if ((ret = (uintptr_t)unrolled_lookup(head, i)) != i + 5)
      panic("_UNROLLED: Value for key %u got corrupted; "
            "returned = %u, actual = %u", i, ret, val);
    i++;
  }
  if (i != len) {
    panic("_UNROLLED: List iterator returned %u element(s) although actual length is %u!", i, len);
  }
  printk("Success!\n");
}

/*
 * Assure that generated keys move from 0 upwards
 */
static void _test_generated_keys(struct unrolled_head *head)
{
  uint idx, nr_elements;
  void *val = NULL;

  printk("_UNROLLED: _test_generated_keys(): ");
  nr_elements = head->array_len * 10;
  for (uintptr_t i = 0; i < nr_elements; i++) {
    idx = unrolled_insert(head, (void *)(i + 5));
    if (idx != i) {
      panic("_UNROLLED: Generated keys does not start from 0 upwards!");
    }
  }
  uint i = 0;
  unrolled_for_each(head, val) {
    if ((uintptr_t) val != i + 5)
      panic("_UNROLLED: Value for key %u got corrupted; returned = %u, actual = %u",
            i, val, i + 5);
    i++;
  }
  if (i != nr_elements)
    panic("_UNROLLED: List iterator returned %u elements although actual length is %u!",
          i, nr_elements);
  printk("Success!\n");
}

/*
 * Assure that deleted keys get properly re-used
 */
static void _test_keys_removal(struct unrolled_head *head)
{
  uint key, nr_elements = 10000;
  void *val = NULL;

  printk("_UNROLLED: _test_keys_removal(): ");
  for (uintptr_t i = 0; i < nr_elements; i++) {
    unrolled_insert(head, (void *)(i + 1));
  }
  for (intptr_t i = nr_elements - 1; i >= 0; i--) {
    val = unrolled_lookup(head, i);
    if (i + 1 != (intptr_t) val) {
      panic("_UNROLLED: Value for key %u got corrupted; returned = %u, actual = %u",
            i, val, i + 1);
    }
    unrolled_remove_key(head, i);

    uint j = 0;
    unrolled_for_each(head, val) {
      j++;
    }
    if (j != nr_elements - 1) {
      panic("_UNROLLED: List iterator returned %u elements although actual len is %u!",
            j, nr_elements - 1);
    }
    key = unrolled_insert(head, (void *)(i + 1));
    if (key != i) {
      panic("_UNROLLED: Returned key should've been %u, but it's %u", i, key);
    }
  }
  printk("Success!\n");
}

/*
 * Like above, but delete several keys at once.
 */
static void _test_keys_removal2(struct unrolled_head *head)
{
  uint key, nr_elements = 10000, nr_deleted_keys;
  void *val = NULL;

  printk("_UNROLLED: _test_keys_removal2(): ");
  nr_deleted_keys = 0;
  for (uintptr_t i = 0; i < nr_elements; i++) {
    unrolled_insert(head, (void *)(i + 1));
  }
  for (uint key = 0; key < nr_elements; key++) {
    if (key % 2 == 0) {
      unrolled_remove_key(head, key);
      nr_deleted_keys++;
    }
  }
  uint j = 0;
  unrolled_for_each(head, val) {
    j++;
  }
  if (j != nr_elements - nr_deleted_keys) {
      panic("_UNROLLED: List iterator returned %u elements although actual len is %u!"
            , j, nr_elements / 2);
  }
  while (nr_deleted_keys--) {
    key = unrolled_insert(head, (void *) 3);
    if (key % 2 != 0 || key >= nr_elements) {
      panic("_UNROLLED: Allocated new key %u, while %u keys were deleted and not yet re-used!",
            key, nr_deleted_keys);
    }
  }
  printk("Success!\n");
}

/*
 * Run all the test-cases using nodes with @array_len cells.
 * @array_len of 1 cell is equivalent to a singly-linked list!
 */
static void _unrolled_run_tests(uint array_len)
{
  struct unrolled_head head;
  uint i;
  void *val = NULL;

  unrolled_init(&head, array_len);
  unrolled_free(&head);

  for (i = 0; i < 1500; i++) {
    unrolled_init(&head, array_len);
    _test_N_elements(&head, i);
    unrolled_free(&head);
  }

  unrolled_init(&head, array_len);
  _test_generated_keys(&head);
  i = 0;
  unrolled_for_each(&head, val) {  /* Delete all keys */
    unrolled_remove_key(&head, i);
    i++;
  }
  _test_generated_keys(&head);  /* Keys should start from 0 again */
  unrolled_free(&head);

  unrolled_init(&head, array_len);
  _test_keys_removal(&head);
  unrolled_free(&head);

  unrolled_init(&head, array_len);
  _test_keys_removal2(&head);
  unrolled_free(&head);
}

bool test_unrolled_list(void)
{
  /* Note the performance increase with bigger node array sizes! */
  for (uint array_len = 1; array_len <= 32; array_len++) {
    _unrolled_run_tests(array_len);
  }
  return true;
}

