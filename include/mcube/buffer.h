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
 * @brief Ring buffer
 */
struct ring_buf {
  uint8_t *buffer;
  size_t head;
  size_t tail;
  size_t max;
  int full;
  spinlock_t lock;
};

typedef struct ring_buf ring_buf_t;


typedef ring_buf_t* cbuf_handle_t;


/**
 * Pass in a storage buffer and size
 * Returns a ring buffer handle
 */
cbuf_handle_t ring_buf_init(uint8_t* buffer, size_t size);

/**
 * Free a ring buffer structure.
 * Does not free data buffer; owner is responsible for that
 */
void ring_buf_free(cbuf_handle_t cbuf);

/**
 * Reset the ring buffer to empty, head == tail
 */
void ring_buf_reset(cbuf_handle_t cbuf);

/**
 * Put version 1 continues to add data if the buffer is full
 * Old data is overwritten
 */
void ring_buf_put(cbuf_handle_t cbuf, uint8_t data);

/**
 * Put Version 2 rejects new data if the buffer is full
 * Returns 0 on success, -1 if buffer is full
 */
int ring_buf_put2(cbuf_handle_t cbuf, uint8_t data);

/**
 * Retrieve a value from the buffer
 * Returns 0 on success, -1 if the buffer is empty
 */
int ring_buf_get(cbuf_handle_t cbuf, uint8_t * data);

/**
 * Returns true if the buffer is empty
 */
bool ring_buf_empty(cbuf_handle_t cbuf);

/**
 * Returns true if the buffer is full
 */
bool ring_buf_full(cbuf_handle_t cbuf);

/**
 * Returns the maximum capacity of the buffer
 */
size_t ring_buf_capacity(cbuf_handle_t cbuf);

/**
 * Returns the current number of elements in the buffer
 */
size_t ring_buf_size(cbuf_handle_t cbuf);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_BUFFER_H__ */
