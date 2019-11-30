/**
 * @file user/axis/pubsub.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
#include "user_axis.h"

static struct cluster own;

struct pubsub_node {
  unsigned long id;
  volatile char buf[4];
};

static struct pubsub_node psnode;

volatile bool is_pubsub_ready = false;

void pubsub_main(void)
{
  unsigned long high_addr;
  unsigned long low_addr;
  struct cluster dst;
  unsigned long cpu_id = get_cluster_and_cpu_ids();
  volatile char flag = 0;
  set_cpu_id(&own, cpu_id);
  psnode.id = own.cluster_id;
  memset((char *) psnode.buf, 0, sizeof(psnode.buf));

  /* NOTE: delay may be changed if using UART. */
  delay(1000 * own.cluster_id);

  if (own.cluster_id == 0) {
    /* create master node in CPU 0 of cluster 0. */

    /* pubsub is ready. */
    is_pubsub_ready = true;

    /* Other clusters' pubsub must be ready. */
    for (int i = 1; i < NR_CLUSTERS; i++) {
      get_cluster_from_index(&dst, i, 0);
      encode_cluster_address(&high_addr, &low_addr, dst.x, dst.y,
                             (unsigned long) &is_pubsub_ready);
      // printk("dst.x = %u dst.y = %u local_cpu_id = %u\n", dst.x, dst.y, dst.local_cpu_id);
      // printk("high_addr = 0x%x low_addr = 0x%x\n", high_addr, low_addr);
      write_to_cluster(dst.local_cpu_id, high_addr, low_addr,
                       (unsigned long *) &is_pubsub_ready, 0);
    }

    printk("master is ready\n");

    while (true) {
      if (flag != psnode.buf[3]) {
        /* send data to subscribers. */
        printk("master node: send data to subscribers\n");

        for (int i = 2; i < NR_CLUSTERS; i++) {
          get_cluster_from_index(&dst, i, 0);
          encode_cluster_address(&high_addr, &low_addr, dst.x, dst.y,
                                 (unsigned long) psnode.buf);
          write_to_cluster(dst.local_cpu_id, high_addr, low_addr,
                           (unsigned long *) psnode.buf, 0);
        }

      }

      flag = psnode.buf[3];
    }
  } else {
    /* create slave nodes in CPU 0 of clusters 1-7. */
    while (!is_pubsub_ready) {
      /* wait until pubsub is ready. */
    }

    /* register slave nodes to master node. */
    printk("slave %u is ready\n", own.cluster_id);

    if (own.cluster_id == 1) {
      /* publisher node. */
      /* send buf data to master node. */
      psnode.buf[0] = 'a';
      psnode.buf[1] = 'b';
      psnode.buf[2] = 'c';
      psnode.buf[3] = 'd';
      printk("slave node 1: send '%c%c%c%c' to master node\n",
             psnode.buf[0], psnode.buf[1], psnode.buf[2], psnode.buf[3]);
      get_cluster_from_index(&dst, 0, 0);
      encode_cluster_address(&high_addr, &low_addr, dst.x, dst.y,
                             (unsigned long) psnode.buf);
      write_to_cluster(dst.local_cpu_id, high_addr, low_addr,
                       (unsigned long *) psnode.buf, 0);

      while (true) {
      }
    } else {
      /* subscriber nodes. */
      while (true) {
        if (flag != psnode.buf[3]) {
          printk("slave %u subscribes data '%c%c%c%c'\n", own.cluster_id,
                 psnode.buf[0], psnode.buf[1], psnode.buf[2], psnode.buf[3]);
        }

        flag = psnode.buf[3];
      }
    }
  }

}

