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
  ring_buf_t msg;
};

static struct pubsub_node pnodes[NR_CLUSTERS];

static bool is_pubsub_ready = false;

void pubsub_main(void)
{
  unsigned long high_addr;
  unsigned long low_addr;
  struct cluster dst;
  unsigned long cpu_id = get_cluster_and_cpu_ids();
  unsigned long cpu = get_cpu_id();
  set_cpu_id(&own, cpu_id);

  
  if (own.x == 0 && own.y == 0) {
    /* create master node in CPU 0 of cluster 0. */
    for (int i = 0; i < NR_CLUSTERS; i++) {
      pnodes[i].id = i;
      ring_buf_init(&pnodes[i].msg, NULL, 0);
    }
    /* pubsub is ready. */
    is_pubsub_ready = true;

    /* Other clusters' pubsub must be ready. */
    for (int i = 1; i < NR_CLUSTERS; i++) {
      get_cluster_from_index(&dst, i, 0);
      encode_cluster_address(&high_addr, &low_addr, dst.x, dst.y,
                             (unsigned long) &is_pubsub_ready);
      write_to_cluster(dst.local_cpu_id, high_addr, low_addr,
                       (unsigned long *) &is_pubsub_ready, 0);
    }
    
    printk("master is ready\n");
    while (true) {
    }
  } else {
    printk("slave %d\n", cpu);
    /* create slave nodes in CPU 0 of clusters 1-7. */
    while (!is_pubsub_ready) {
      /* wait until pubsub is ready. */
    }
    /* register slave nodes to master node. */
    printk("slave %d is ready\n", cpu);
  }



  /*
   * publish data to master node.
   * Slave nodes subscribe data from master node.
   */
}

