/**
 * @file include/axis/cpu.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_CPU_H__
#define __MCUBE_AXIS_CPU_H__

/**
 * @def USER_LEVEL
 * @brief User level.
 */
#define USER_LEVEL 0 // compatibility for other architectures

/**
 * @def KERNEL_LEVEL
 * @brief Kernel level.
 */
#define KERNEL_LEVEL 1


#ifndef __ASSEMBLY__

/**
 * @struct cluster
 * @brief AXIS cluster information.
 */
struct cluster {
  /**
   * Cluster ID.
   */
  unsigned long cluster_id;

  /**
   * X-axis.
   */
  unsigned long x;

  /**
   * Y-axis.
   */
  unsigned long y;

  /**
   * Local CPU ID.
   */
  unsigned long local_cpu_id;
};

/**
 * @fn void read_from_cluster(unsigned long local_cpu_id,
 *                            unsigned long high_addr,
 *                            unsigned long low_addr,
 *                            volatile unsigned long *data,
 *                            unsigned long qos)
 * @brief read data from cluster.
 *
 * @param local_cpu_id Local CPU ID.
 * @param high_addr Higher 32-bit address.
 * @param low_addr Lower 32-bit address.
 * @param data Data.
 * @param qos Quality of Service (QoS).
 */
void read_from_cluster(unsigned long local_cpu_id,
                       unsigned long high_addr,
                       unsigned long low_addr,
                       volatile unsigned long *data,
                       unsigned long qos);


/**
 * @fn void write_to_cluster(unsigned long local_cpu_id,
 *                           unsigned long high_addr,
 *                           unsigned long low_addr,
 *                           volatile unsigned long *data,
 *                           unsigned long qos)
 * @brief write data to cluster.
 *
 * @param local_cpu_id Local CPU ID.
 * @param high_addr Higher 32-bit address.
 * @param low_addr Lower 32-bit address.
 * @param data Data.
 * @param qos Quality of Service (QoS).
 */
void write_to_cluster(unsigned long local_cpu_id,
                      unsigned long high_addr,
                      unsigned long low_addr,
                      volatile unsigned long *data,
                      unsigned long qos);

/**
 * @fn void encode_cluster_address(volatile unsigned long *high_addr,
 *                                 volatile unsigned long *low_addr,
 *                                 unsigned long x,
 *                                 unsigned long y,
 *                                 unsigned long offset)
 * @brief encode cluster address.
 *
 * @param high_addr Higher 32-bit address.
 * @param low_addr Lower 32-bit address.
 * @param x X-axis.
 * @param y Y-axis.
 * @param offset Offset.
 */
void encode_cluster_address(volatile unsigned long *high_addr,
                            volatile unsigned long *low_addr,
                            unsigned long x,
                            unsigned long y,
                            unsigned long offset);

/**
 * @fn void set_cpu_id(struct cluster *c, unsigned long cpu_id)
 * @brief set CPU ID.
 *
 * @param c Pointer to cluster.
 * @param cpu_id Cpu ID.
 */
void set_cpu_id(struct cluster *c, unsigned long cpu_id);

/**
 * @fn void get_cluster_from_index(struct cluster *c, volatile int index,
 *                                 volatile int cpu)
 * @brief get cluster from index.
 *
 * @param c Pointer to cluster.
 * @param index Index.
 * @param cpu CPU ID.
 */
void get_cluster_from_index(struct cluster *c, volatile int index,
                            volatile int cpu);



#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_AXIS_CPU_H__ */
