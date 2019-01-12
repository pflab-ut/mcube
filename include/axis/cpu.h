/**
 * @file include/axis/cpu.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_CPU_H__
#define __MCUBE_AXIS_CPU_H__

#define USER_LEVEL 0 // compatibility for other architectures
#define KERNEL_LEVEL 1


#ifndef __ASSEMBLY__

/**
 * @struct cluster
 * @brief  AXIS cluster information.
 */
struct cluster {
  unsigned long cluster_id;
  unsigned long x;
  unsigned long y;
  unsigned long local_cpu_id;
};

void read_from_cluster(unsigned long local_cpu_id,
                       unsigned long high_addr,
                       unsigned long low_addr,
                       volatile unsigned long *data,
                       unsigned long qos);

void write_to_cluster(unsigned long local_cpu_id,
                      unsigned long high_addr,
                      unsigned long low_addr,
                      volatile unsigned long *data,
                      unsigned long qos);


void encode_cluster_address(volatile unsigned long *high_addr,
                            volatile unsigned long *low_addr,
                            unsigned long x,
                            unsigned long y,
                            unsigned long ofs);

void set_cpu_id(struct cluster *c, unsigned long cpu_id);

void get_cluster_from_index(struct cluster *c, volatile int index, volatile int cpu);



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_AXIS_CPU_H__ */
