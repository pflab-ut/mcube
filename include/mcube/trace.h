/**
 * @file include/mcube/trace.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_TRACE_H__
#define	__MCUBE_MCUBE_TRACE_H__

#ifndef __ASSEMBLY__


#define SLOT_FREE 0
#define SLOT_BUSY 1
#define SLOT_READY 2


/**
 * @brief Trace buffer
 *
 * The trace_buffer structure has timer information by list.
 */
struct trace_buffer {
	unsigned int slot_count;
	unsigned int slot_size;

	int free_count;
	int write_idx;
	int read_idx;

	char *slots;
	void *buffer_mem;
	int failed_writes;
};

typedef struct trace_buffer trace_buffer;

extern struct trace_buffer ft_buf;


extern int init_trace_buffer(struct trace_buffer *buf,
													unsigned int slot_count,
													unsigned int slot_size,
													char *slots,
													void *buffer_mem);

extern int trace_buffer_start_write(struct trace_buffer *buf, void **ptr);
extern void trace_buffer_finish_write(struct trace_buffer *buf, void *ptr);
extern int trace_buffer_read(struct trace_buffer *buf, void *dest);

extern void init_trace(void);
extern void exit_trace(void);


#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_TRACE_H__ */
