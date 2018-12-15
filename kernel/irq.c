/**
 * @file kernel/irq.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

callback_t Callback;

void register_callback_handler(callback_t func)
{
  Callback = func;
}
