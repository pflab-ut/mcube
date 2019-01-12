/**
 * @file lib/buffer.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>



static void advance_pointer(cbuf_handle_t cbuf)
{
  if (cbuf->full) {
    cbuf->tail = (cbuf->tail + 1) % cbuf->max;
  }

  cbuf->head = (cbuf->head + 1) % cbuf->max;

  // We mark full because we will advance tail on the next time around
  cbuf->full = (cbuf->head == cbuf->tail);
}

static void retreat_pointer(cbuf_handle_t cbuf)
{
  cbuf->full = false;
  cbuf->tail = (cbuf->tail + 1) % cbuf->max;
}


cbuf_handle_t ring_buf_init(uint8_t *buffer, size_t size)
{
  cbuf_handle_t cbuf;
  if (!(cbuf = kmalloc(sizeof(ring_buf_t)))) {
    print("Error: cannot allocate memory %lu\n", sizeof(ring_buf_t));
    return NULL;
  }
  cbuf->buffer = buffer;
  cbuf->max = size;
  cbuf->lock = INIT_SPINLOCK;
  ring_buf_reset(cbuf);

  return cbuf;
}

void ring_buf_free(cbuf_handle_t cbuf)
{
  kfree(cbuf);
}

void ring_buf_reset(cbuf_handle_t cbuf)
{
  spin_lock(&cbuf->lock);
  cbuf->head = 0;
  cbuf->tail = 0;
  cbuf->full = false;
  spin_unlock(&cbuf->lock);
}

size_t ring_buf_size(cbuf_handle_t cbuf)
{
  size_t size;
  spin_lock(&cbuf->lock);
  size = cbuf->max;

  if (!cbuf->full) {
    if (cbuf->head >= cbuf->tail) {
      size = cbuf->head - cbuf->tail;
    } else {
      size = cbuf->max + cbuf->head - cbuf->tail;
    }    
  }
  spin_unlock(&cbuf->lock);  
  return size;
}

size_t ring_buf_capacity(cbuf_handle_t cbuf)
{
  return cbuf->max;
}

void ring_buf_put(cbuf_handle_t cbuf, uint8_t data)
{
  spin_lock(&cbuf->lock);
  cbuf->buffer[cbuf->head] = data;

  advance_pointer(cbuf);
  spin_unlock(&cbuf->lock);  
}

int ring_buf_put2(cbuf_handle_t cbuf, uint8_t data)
{
  int r = -1;
  spin_lock(&cbuf->lock);
  if (!ring_buf_full(cbuf)) {
    cbuf->buffer[cbuf->head] = data;
    advance_pointer(cbuf);
    r = 0;
  }
  spin_unlock(&cbuf->lock);
  return r;
}

int ring_buf_get(cbuf_handle_t cbuf, uint8_t *data)
{
  int r = -1;
  spin_lock(&cbuf->lock);
  if (!ring_buf_empty(cbuf)) {
    *data = cbuf->buffer[cbuf->tail];
    retreat_pointer(cbuf);
    r = 0;
  }
  spin_unlock(&cbuf->lock);  
  return r;
}

bool ring_buf_empty(cbuf_handle_t cbuf)
{
  return (!cbuf->full && (cbuf->head == cbuf->tail));
}

bool ring_buf_full(cbuf_handle_t cbuf)
{
  return cbuf->full;
}
