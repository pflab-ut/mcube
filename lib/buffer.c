/**
 * @file lib/buffer.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

fifo32_struct fifo32;
uint32_t fifo32_buf[FIFO32_BUFSIZE];

void init_fifo32(struct fifo32_struct *fifo, int size, uint32_t *buf)
{
	fifo->size = size;
	fifo->buf = buf;
	fifo->free = size;
	fifo->flags = 0;
	fifo->put_index = 0;
	fifo->get_index = 0;
}

void put_fifo32(uint32_t data, struct fifo32_struct *fifo)
{
	if (fifo->free == 0) {
		fifo->flags |= FIFO32FLAGS_FULL;
		printk("put_fifo32(): fifo is full\n");
		return;
	}
	fifo->buf[fifo->put_index] = data;
	fifo->put_index++;
	if (fifo->put_index == fifo->size) {
		fifo->put_index = 0;
	}
	fifo->free--;
}

uint32_t get_fifo32(struct fifo32_struct *fifo)
{
	uint32_t data;
	if (fifo->free == fifo->size) {
		return -1;
	}
	data = fifo->buf[fifo->get_index];
	fifo->get_index++;
	if (fifo->get_index == fifo->size) {
		fifo->get_index = 0;
	}
	fifo->free++;
	return data;
}


uint32_t get_fifo32_status(struct fifo32_struct *fifo)
{
	return fifo->size - fifo->free;
}

void init_buffer(void)
{
	init_fifo32(&fifo32, FIFO32_BUFSIZE, fifo32_buf);
}
