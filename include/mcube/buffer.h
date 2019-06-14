/**
 * @file include/mcube/buffer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_BUFFER_H__
#define __MCUBE_MCUBE_BUFFER_H__

#ifndef __ASSEMBLY__

/**
 * @struct ring_buf
 * @brief Ring buffer.
 */
struct ring_buf {
  /**
   * Pointer to buffer.
   */
  uint8_t *buffer;

  /**
   * Head.
   */
  size_t head;

  /**
   * Tail.
   */
  size_t tail;

  /**
   * Max.
   */
  size_t max;

  /**
   * Is buffer full?
   */
  bool full;

  /**
   * Lock for buffer.
   */
  spinlock_t lock;
};

/**
 * @typedef ring_buf_t
 * @brief Typedef of @struct ring_buf.
 */
typedef struct ring_buf ring_buf_t;

/**
 * @typedef cbuf_handle_t
 * @brief Typedef of @typedef ring_buf_t.
 */
typedef ring_buf_t *cbuf_handle_t;

/**
 * @def INIT_RING_BUF
 * @param initialize ring buffer.
 */
#define INIT_RING_BUF (ring_buf_t) {                                    \
    .buffer = NULL, .head = 0, .tail = 0, .max = -1, .full = false, .lock = INIT_SPINLOCK \
      }

/**
 * @fn cbuf_handle_t ring_buf_init(uint8_t *buffer, size_t size)
 * @brief initialize buffer.
 * @param buffer Buffer.
 * @param size Buffer size.
 * @return Handled buffer.
 */
cbuf_handle_t ring_buf_init(uint8_t *buffer, size_t size);

/**
 * @fn void ring_buf_free(cbuf_handle_t cbuf)
 * @brief Free buffer structure.
 * Does not free data buffer; owner is responsible for that.
 * @param cbuf Buffer.
 */
void ring_buf_free(cbuf_handle_t cbuf);

/**
 * @fn void ring_buf_reset(cbuf_handle_t cbuf)
 * @brief Reset buffer to empty, head == tail.
 * @param cbuf Buffer.
 */
void ring_buf_reset(cbuf_handle_t cbuf);

/**
 * @fn void ring_buf_put(cbuf_handle_t cbuf, uint8_t data)
 * @brief Put version 1 continues to add data if the buffer is full.
 * Old data is overwritten.
 * @param cbuf Buffer.
 * @param data Data.
 */
void ring_buf_put(cbuf_handle_t cbuf, uint8_t data);

/**
 * @fn int ring_buf_put2(cbuf_handle_t cbuf, uint8_t data)
 * @brief Put Version 2 rejects new data if buffer is full
 * @param cbuf Buffer.
 * @param data Data.
 * @return 0 on success, -1 if buffer is full.
 */
int ring_buf_put2(cbuf_handle_t cbuf, uint8_t data);

/**
 * @fn int ring_buf_get(cbuf_handle_t cbuf, uint8_t *data)
 * @brief Retrieve a value from buffer.
 * @param cbuf Buffer.
 * @param data Data.
 * @return 0 on success, -1 if buffer is empty.
 */
int ring_buf_get(cbuf_handle_t cbuf, uint8_t *data);

/**
 * @fn bool ring_buf_empty(cbuf_handle_t cbuf)
 * @brief Is buffer empty?
 * @param cbuf Buffer.
 * @return True if buffer is empty.
 */
bool ring_buf_empty(cbuf_handle_t cbuf);

/**
 * @fn bool ring_buf_full(cbuf_handle_t cbuf)
 * @param cbuf Buffer.
 * @return True if the buffer is full.
 */
bool ring_buf_full(cbuf_handle_t cbuf);

/**
 * @fn size_t ring_buf_capacity(cbuf_handle_t cbuf)
 * @param cbuf Buffer.
 * @return Maximum capacity of buffer.
 */
size_t ring_buf_capacity(cbuf_handle_t cbuf);

/**
 * @fn size_t ring_buf_size(cbuf_handle_t cbuf)
 * @param cbuf Buffer.
 * @return Current number of elements in buffer.
 */
size_t ring_buf_size(cbuf_handle_t cbuf);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_BUFFER_H__ */
