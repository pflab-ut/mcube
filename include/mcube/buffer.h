/**
 * @file include/mcube/buffer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_BUFFER_H__
#define	__MCUBE_MCUBE_BUFFER_H__

#ifndef __ASSEMBLY__

/**
 * @brief FIFO buffer
 *
 * The fifo32_struct structure is used to do shell.
 */
struct fifo32_struct {
	/** Pointer to buffer. */
	uint32_t *buf;
	/** Index to put character. */
	int put_index;
	/** Index to get character. */
	int get_index;
	/** Size of buffer. */
	int size;
	/** Size of free buffer. */
	int free;
	/** Flags of buffer. */
	int flags;
};

typedef struct fifo32_struct fifo32_struct;


#define FIFO32FLAGS_FULL (0x1 << 0)
#define FIFO32_IS_EMPTY -1

#define FIFO32_BUFSIZE 256



extern fifo32_struct fifo32;
extern uint32_t fifo32_buf[FIFO32_BUFSIZE];


void init_fifo32(struct fifo32_struct *fifo, int size, uint32_t *buf);
void put_fifo32(uint32_t data, struct fifo32_struct *fifo);
uint32_t get_fifo32(struct fifo32_struct *fifo);
uint32_t get_fifo32_status(struct fifo32_struct *fifo);

void init_buffer(void);

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_BUFFER_H__ */
