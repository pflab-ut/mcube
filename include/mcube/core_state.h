/**
 * @file include/mcube/core_state.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_CORE_STATE_H__
#define __MCUBE_MCUBE_CORE_STATE_H__

#ifndef __ASSEMBLY__

/**
 * @struct core_state
 * @brief Core state management for multikernel
 */
struct core_state {
  /**
   * This lock must be held before any operation where there is a possibility of
   * concurrent access to the same binding (e.g. multiple threads, or
   * asynchronous event handlers that use the same binding object). */
  spinlock_t lock;

  /**
   * Transmit message;
   */
  struct ring_buf message_tx[NR_CPUS];
  
  /**
   * Receive message;
   */
  struct ring_buf message_rx[NR_CPUS];

};


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_CORE_STATE_H__ */
