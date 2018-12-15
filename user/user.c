/**
 * @file user/user.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#define EXAMPLE_BUFFER_SIZE 10

int test_ring_buf(void)
{
  uint8_t *buffer;
  if (!(buffer = malloc(EXAMPLE_BUFFER_SIZE * sizeof(uint8_t)))) {
    printf("Error: cannot allocate memory %lu\n", EXAMPLE_BUFFER_SIZE * sizeof(uint8_t));
    return 1;
  }
  print("buffer = 0x%x\n", buffer);

  printf("\n=== C Ring Buffer Check ===\n");

  cbuf_handle_t cbuf = ring_buf_init(buffer, EXAMPLE_BUFFER_SIZE);

  printf("Buffer initialized. Full: %d, empty: %d, size: %zu\n",
         ring_buf_full(cbuf),
         ring_buf_empty(cbuf),
         ring_buf_size(cbuf));

  printf("\n******\nAdding %d values\n", EXAMPLE_BUFFER_SIZE - 1);
  for (uint8_t i = 0; i < (EXAMPLE_BUFFER_SIZE - 1); i++) {
    ring_buf_put(cbuf, i);
    printf("Added %u, Size now: %zu\n", i, ring_buf_size(cbuf));
  }
  
  printf("Full: %d, empty: %d, size: %zu\n",
         ring_buf_full(cbuf),
         ring_buf_empty(cbuf),
         ring_buf_size(cbuf));

  printf("\n******\nAdding %d values\n", EXAMPLE_BUFFER_SIZE);
  for (uint8_t i = 0; i < EXAMPLE_BUFFER_SIZE; i++) {
    ring_buf_put(cbuf, i);
    printf("Added %u, Size now: %zu\n", i, ring_buf_size(cbuf));
  }

  printf("Full: %d, empty: %d, size: %zu\n",
         ring_buf_full(cbuf),
         ring_buf_empty(cbuf),
         ring_buf_size(cbuf));

  printf("\n******\nReading back values: ");
  while (!ring_buf_empty(cbuf)) {
    uint8_t data;
    ring_buf_get(cbuf, &data);
    printf("%u ", data);
  }
  printf("\n");

  printf("Full: %d, empty: %d, size: %zu\n",
         ring_buf_full(cbuf),
         ring_buf_empty(cbuf),
         ring_buf_size(cbuf));

  printf("\n******\nAdding %d values\n", EXAMPLE_BUFFER_SIZE + 5);
  for (uint8_t i = 0; i < EXAMPLE_BUFFER_SIZE + 5; i++) {
    ring_buf_put(cbuf, i);
    printf("Added %u, Size now: %zu\n", i, ring_buf_size(cbuf));
  }

  printf("Full: %d, empty: %d, size: %zu\n",
         ring_buf_full(cbuf),
         ring_buf_empty(cbuf),
         ring_buf_size(cbuf));

  printf("\n******\nReading back values: ");
  while (!ring_buf_empty(cbuf)) {
    uint8_t data;
    ring_buf_get(cbuf, &data);
    printf("%u ", data);
  }
  printf("\n");

  printf("\n******\nAdding %d values using non-overwrite version\n", EXAMPLE_BUFFER_SIZE + 5);
  for (uint8_t i = 0; i < EXAMPLE_BUFFER_SIZE + 5; i++) {
    ring_buf_put2(cbuf, i);
  }
  
  printf("Full: %d, empty: %d, size: %zu\n",
         ring_buf_full(cbuf),
         ring_buf_empty(cbuf),
         ring_buf_size(cbuf));

  printf("\n******\nReading back values: ");
  while (!ring_buf_empty(cbuf)) {
    uint8_t data;
    ring_buf_get(cbuf, &data);
    printf("%u ", data);
  }
  printf("\n");

  free(buffer);
  ring_buf_free(cbuf);

  return 0;
}


int user_arch_main(void);


#if CONFIG_ARCH_SIM || CONFIG_ARCH_AXIS

int ap_main(void)
{
  unsigned long cpu = get_cpu_id();
  putchar(cpu + '0');
  print("ap_main()\n");
  print("get_cpu_id() = %lu\n", get_cpu_id());
  //  print("ap_main()\n");
  //  cpu_ids[cpu] = cpu;
  return 0;
}
#endif /* CONFIG_ARCH_SIM || CONFIG_ARCH_AXIS */


int user_main(void)
{
  user_arch_main();
  return 0;
}
