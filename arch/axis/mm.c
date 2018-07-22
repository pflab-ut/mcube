/**
 * @file arch/axis/mm.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void do_local_dmac(volatile uint32_t dst,
                   volatile uint32_t src,
                   volatile size_t n,
                   enum dmac policy)
{
  if (policy >= DMAC_END) {
    printk("Error: unknown DMAC transfer policy %d\n", policy);
    return;
  }
  if (n == 0) {
    printk("Error: transfer size is zero\n");
    return;
  }
  set_dmac_destination_address(dst);
  set_dmac_source_address(src);
  set_dmac_transfer_size(n);
  if (policy == DMAC_SYNC_INTERRUPT || policy == DMAC_ASYNC_INTERRUPT) {
    enable_dmac_interrupt();
  }
  enable_dmac();
  if (policy == DMAC_SYNC_INTERRUPT) {
    wait_until_next_interrupt();
  } else if (policy == DMAC_POLLING) {
    while (get_dmac_status() == 0) {
      /* wait until DMAC transfer is completed by polling. */
    }
    set_dmac_status_clear(1);
  }
}
