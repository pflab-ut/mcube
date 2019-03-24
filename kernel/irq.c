/**
 * @file kernel/irq.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

callback_t Callback[NR_CALLBACKS] = {NULL};

void register_callback_handler(callback_t func, unsigned long id)
{
  if (id > NR_CALLBACKS) {
    print("Error: id %lu > NR_CALLBACKS %d\n", id, NR_CALLBACKS);
  }

  Callback[id] = func;
}
