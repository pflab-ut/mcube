/**
 * @file arch/axis/dmac.c
 *
 * @author Hiroyuki Chishiro
 *         Akihiro Sakurai
 */
#include <mcube/mcube.h>

void do_local_dmac(uint32_t dst,
                   uint32_t src,
                   size_t n,
                   unsigned int ch,
                   enum dmac policy)
{
  if (policy >= DMAC_END) {
    printk("Error: Unknown DMAC transfer policy %d.\n", policy);
    return;
  }

  if (n == 0) {
    printk("Error: Transfer size is zero.\n");
    return;
  }

  if (ch != 0) {
    printk("Error: Channel must be zero.\n");
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

void dmac_read_from_cluster(uint32_t dst,
                            uint32_t high_addr,
                            uint32_t low_addr,
                            size_t n,
                            uint32_t qos,
                            unsigned int ch,
                            enum dmac policy)
{
  if (policy >= DMAC_END) {
    printk("Error: Unknown DMAC transfer policy %d.\n", policy);
    return;
  }

  if (n == 0) {
    printk("Error: Transfer size is zero.\n");
    return;
  }

  if (ch != 0) {
    printk("Error: Channel must be zero.\n");
    return;
  }

  set_dmac_destination_address(dst);
  uint32_t offset = NET_BASE | low_addr;
  set_dmac_source_address(offset);
  set_dmac_transfer_size(n);
  set_dmac_net_base(high_addr);
  uint32_t net_ctrl = (low_addr & NET_BASE) + qos;
  set_dmac_net_ctrl(net_ctrl);

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

void dmac_write_to_cluster(uint32_t src,
                           uint32_t high_addr,
                           uint32_t low_addr,
                           size_t n,
                           uint32_t qos,
                           unsigned int ch,
                           enum dmac policy)
{
  if (policy >= DMAC_END) {
    printk("Error: Unknown DMAC transfer policy %d.\n", policy);
    return;
  }

  if (n == 0) {
    printk("Error: Transfer size is zero.\n");
    return;
  }

  if (ch != 0) {
    printk("Error: Channel must be zero.\n");
    return;
  }

  set_dmac_source_address(src);
  uint32_t offset = NET_BASE | low_addr;
  set_dmac_source_address(offset);
  set_dmac_destination_address(offset);
  set_dmac_transfer_size(n);
  set_dmac_net_base(high_addr);
  uint32_t net_ctrl = (low_addr & NET_BASE) + qos;
  set_dmac_net_ctrl(net_ctrl);

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

