/**
 * @file user/test/test_ring_buf.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#define EXAMPLE_BUFFER_SIZE 10

bool test_ring_buf(void)
{
  uint8_t *buffer;

  if (!(buffer = (uint8_t *) kmalloc(EXAMPLE_BUFFER_SIZE * sizeof(uint8_t)))) {
    printk("Error: cannot allocate memory %lu\n",
           EXAMPLE_BUFFER_SIZE * sizeof(uint8_t));
    return false;
  }

  printk("\n=== C Ring Buffer Check ===\n");

  cbuf_handle_t cbuf;
  if (!(cbuf = (cbuf_handle_t) kmalloc(sizeof(ring_buf_t)))) {
    print("Error: cannot allocate memory %lu\n", sizeof(ring_buf_t));
    return false;
  }
  ring_buf_init(cbuf, buffer, EXAMPLE_BUFFER_SIZE);
  
  printk("Buffer initialized. Full: %d, empty: %d, size: %zu\n",
         ring_buf_full(cbuf),
         ring_buf_empty(cbuf),
         ring_buf_size(cbuf));

  printk("\n******\nAdding %d values\n", EXAMPLE_BUFFER_SIZE - 1);

  for (uint8_t i = 0; i < (EXAMPLE_BUFFER_SIZE - 1); i++) {
    ring_buf_put(cbuf, i);
    printk("Added %u, Size now: %zu\n", i, ring_buf_size(cbuf));
  }

  printk("Full: %d, empty: %d, size: %zu\n",
         ring_buf_full(cbuf),
         ring_buf_empty(cbuf),
         ring_buf_size(cbuf));

  printk("\n******\nAdding %d values\n", EXAMPLE_BUFFER_SIZE);

  for (uint8_t i = 0; i < EXAMPLE_BUFFER_SIZE; i++) {
    ring_buf_put(cbuf, i);
    printk("Added %u, Size now: %zu\n", i, ring_buf_size(cbuf));
  }

  printk("Full: %d, empty: %d, size: %zu\n",
         ring_buf_full(cbuf),
         ring_buf_empty(cbuf),
         ring_buf_size(cbuf));

  printk("\n******\nReading back values: ");

  while (!ring_buf_empty(cbuf)) {
    uint8_t data;
    ring_buf_get(cbuf, &data);
    printk("%u ", data);
  }

  printk("\n");

  printk("Full: %d, empty: %d, size: %zu\n",
         ring_buf_full(cbuf),
         ring_buf_empty(cbuf),
         ring_buf_size(cbuf));

  printk("\n******\nAdding %d values\n", EXAMPLE_BUFFER_SIZE + 5);

  for (uint8_t i = 0; i < EXAMPLE_BUFFER_SIZE + 5; i++) {
    ring_buf_put(cbuf, i);
    printk("Added %u, Size now: %zu\n", i, ring_buf_size(cbuf));
  }

  printk("Full: %d, empty: %d, size: %zu\n",
         ring_buf_full(cbuf),
         ring_buf_empty(cbuf),
         ring_buf_size(cbuf));

  printk("\n******\nReading back values: ");

  while (!ring_buf_empty(cbuf)) {
    uint8_t data;
    ring_buf_get(cbuf, &data);
    printk("%u ", data);
  }

  printk("\n");

  printk("\n******\nAdding %d values using non-overwrite version\n",
         EXAMPLE_BUFFER_SIZE + 5);

  for (uint8_t i = 0; i < EXAMPLE_BUFFER_SIZE + 5; i++) {
    ring_buf_put2(cbuf, i);
  }

  printk("Full: %d, empty: %d, size: %zu\n",
         ring_buf_full(cbuf),
         ring_buf_empty(cbuf),
         ring_buf_size(cbuf));

  printk("\n******\nReading back values: ");

  while (!ring_buf_empty(cbuf)) {
    uint8_t data;
    ring_buf_get(cbuf, &data);
    printk("%u ", data);
  }

  printk("\n");

  kfree(buffer);
  ring_buf_free(cbuf);

  return true;
}


