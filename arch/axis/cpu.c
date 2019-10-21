/**
 * @file arch/axis/cpu.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void read_from_cluster(unsigned long local_cpu_id,
                       unsigned long high_addr,
                       unsigned long low_addr,
                       volatile unsigned long *data,
                       unsigned long qos)
{
  unsigned long id = local_cpu_id & 0x7;
  volatile unsigned long offset   = NET_BASE | low_addr;
  *((unsigned long *)(SYS_NET_BASE(id))) = high_addr;
  *((unsigned long *)(SYS_NET_CTRL(id))) = (low_addr & NET_BASE) + qos;
  *((volatile unsigned long *) data) = *((volatile unsigned long *) offset);
}


void write_to_cluster(unsigned long local_cpu_id,
                      unsigned long high_addr,
                      unsigned long low_addr,
                      volatile unsigned long *data,
                      unsigned long qos)
{
  unsigned long id = local_cpu_id & 0x7;
  volatile unsigned long offset   = NET_BASE | low_addr;
  *((unsigned long *)(SYS_NET_BASE(id))) = high_addr;
  *((unsigned long *)(SYS_NET_CTRL(id))) = (low_addr & NET_BASE) + qos;
  *((volatile unsigned long *) offset) = *((volatile unsigned long *) data);
}


void encode_cluster_address(volatile unsigned long *high_addr,
                            volatile unsigned long *low_addr,
                            unsigned long x,
                            unsigned long y,
                            unsigned long offset)
{
  *high_addr = (x & 0x3c) >> 2;
  *low_addr = (x & 0x03) << 30;
  *low_addr |= (y & 0x3f) << 24;
  *low_addr |= offset & 0xffffff;
}

/*
 * 3x3 Cluster.
 * Each cluster (x, y) has CPU but cluster (1, 1) does not have CPU and has gateway.
 *
 * (0, 0) - (0, 1) - (0, 2)
 *   |        |        |
 * (1, 0) - (1, 1) - (1, 2)
 *   |        |        |
 * (2, 0) - (2, 1) - (2, 2)
 */
void set_cpu_id(struct cluster *c, unsigned long cpu_id)
{
  c->x = (cpu_id >> 24) & 0xff;
  c->y = (cpu_id >> 16) & 0xff;
  c->local_cpu_id = cpu_id & 0xffff;

  //  printk("c->x = %d c->y = %d\n", c->x, c->y);
  //  printk("c->x = 0x%x c->y = 0x%x\n", c->x, c->y);
  if (c->x == 0 && c->y == 0) {
    c->cluster_id = 0;
  } else if (c->x == 0 && c->y == 1) {
    c->cluster_id = 1;
  } else if (c->x == 0 && c->y == 2) {
    c->cluster_id = 2;
  } else if (c->x == 1 && c->y == 0) {
    c->cluster_id = 3;
  } else if (c->x == 1 && c->y == 2) {
    c->cluster_id = 4;
  } else if (c->x == 2 && c->y == 0) {
    c->cluster_id = 5;
  } else if (c->x == 2 && c->y == 1) {
    c->cluster_id = 6;
  } else if (c->x == 2 && c->y == 2) {
    c->cluster_id = 7;
  } else {
    printk("Error: Unknown (x, y) = (%lu, %lu)\n", c->x, c->y);
    c->cluster_id = UNKNOWN_CPU_ID;
  }
}

void get_cluster_from_index(struct cluster *c, volatile int index,
                            volatile int cpu)
{
  //  printk("index = %d cpu = %d\n", index, cpu);
  /* NOTE: (x, y) = (1, 1) means gateway and does not implement CPU. */
  if (index == 0) {
    c->x = 0;
    c->y = 0;
  } else if (index == 1) {
    c->x = 0;
    c->y = 1;
  } else if (index == 2) {
    c->x = 0;
    c->y = 2;
  } else if (index == 3) {
    c->x = 1;
    c->y = 0;
  } else if (index == 4) {
    c->x = 1;
    c->y = 2;
  } else if (index == 5) {
    c->x = 2;
    c->y = 0;
  } else if (index == 6) {
    c->x = 2;
    c->y = 1;
  } else if (index == 7) {
    c->x = 2;
    c->y = 2;
  } else {
    c->x = UNKNOWN_X_AXIS;
    c->y = UNKNOWN_Y_AXIS;
  }

  c->cluster_id = index;
  c->local_cpu_id = cpu;
  //  printk("c->x = %d c->y = %d\n", c->x, c->y);
}


void set_cpu_clock(void)
{
  /* CPU clock is 333MHz. */
  CPU_CLOCK_MHZ_PER_USEC = 333;
  CPU_CLOCK = CPU_CLOCK_MHZ_PER_USEC * 1000 * 1000;
}


void init_cpu(void)
{
  set_cpu_clock();
}
