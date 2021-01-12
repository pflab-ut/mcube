/**
 * @file include/axis/msg_buf.h
 *
 * @author Akihiro Sakurai
 */
#ifndef __MCUBE_AXIS_MSG_BUF_H_
#define __MCUBE_AXIS_MSG_BUF_H__

#ifndef __ASSEMBLY__

/**
 * @def MSG_BUF_SIZE
 * @brief Max message buffer size.
 */
#define MSG_BUF_SIZE 0x10000

/**
 * @def MAX_MSG_SIZE
 * @brief Max message buffer size.
 */
#define MAX_MSG_SIZE 0x1000

/**
 * @var mbuf_array[NR_THREADS][MSG_BUF_SIZE]
 * @brief Message buffer array.
 */
extern unsigned char mbuf_array[NR_THREADS][MSG_BUF_SIZE];


/**
 * @struct mbuf_init_block
 * @brief Message buf initialization block
 */

struct mbuf_init_block {
  /**
   * Max size of message.
   */
  uint_t max_msg_size;


  /**
   * Max size of control block.
   */
  size_t mbuf_ctrl_block_size;

  /**
   * First address of control block.
   */
  void *mbuf_ctrl_block;
} mbuf_init_block;

extern const struct mbuf_init_block mbfinitb;

/**
 * @struct mbuf_ctrl_block
 * @brief Message buf control block
 */

struct mbuf_ctrl_block {
  /**
   * Message buf wait send queue.
   */

  /**
   * Pointer to init block.
   */

  const struct mbuf_init_block *p_mbuf_init_block;

  /**
   * Message buf wait receive queue.
   */

  /**
   * Free size of message buf.
   */
  size_t f_mbuf_size;

  /**
   * Stored area of first message.
   */
  size_t head;


  /**
   * Stored area of tail message.
   */
  size_t tail;

  /**
   * Number of stored message.
   */
  uint_t msg_cnt;

} mbuf_ctrl_block;

/**
 * @var mbuf_ctrl_block_array[NR_THREADS];
 * @brief Message buffer control block array.
 */
extern struct mbuf_ctrl_block mbuf_ctrl_block_array[NR_THREADS];

/**
 *@fn void init_msg_buf()
 *@brief initialize message buffer function.
 */
void init_msg_buf();

/**
 *@fn int enqueue_msg(mbuf_ctrl_block *p_mbfcb,
 *                    void *msg,
 *                    uint_t msgsz)
 * @brief enqueue msg to message buffer.
 * @param p_mbfcb Pointer to control block.
 * @param msg Message.
 * @param msgsz Message size.
 */
int enqueue_msg(mbuf_ctrl_block *p_mbfcb,
                void *msg,
                uint_t msgsz);

//msg size is written in top of the msg buffer


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_AXIS_MSG_BUF_H__ */
