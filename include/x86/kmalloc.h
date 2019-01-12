#ifndef _KMALLOC_H
#define _KMALLOC_H

/*
 * Kernel Memory Allocator
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2.
 *
 * Please see comments on top of kmalloc.c first ..
 */

/*
 * Terminology:
 * 'Bucket' - all bookkeeping information for one of the power-of
 * 2 lists, including its head.
 *
 * 'Buffer' - Each allocated page frame is tokenized to a group
 * of equality sized buffers. Sizes ranges from 16 to 2048 bytes.
 */

/*
 * A Buffer only exists in two states, either allocated or free:
 * - If allocated, the caller can consume all of its size.
 * - If free, it must be in one of the power of two free bufs
 * lists, or it's simply lost forever.
 *
 * Buffers does not include their own metadata. When a buf is
 * free, we let its first 8-byts act as a a pointer to the next
 * free buf: buffer sizes must be large enough to hold a pointer.
 *
 * Looking at a Linux system slab statistics running a typical
 * desktop workload, we see 6144 8-byte objects, 5376 16-byte
 * objects, and 3328 32-byte objects. So, a minimum buffer size
 * of 16-bytes sounds sensible. Max size is set to one 4K page.
 *
 * Bucket index '4' represents a buf size of (2 << 4) = 16 bytes.
 */
#define MINBUCKET_IDX	4		/* 16 bytes */
#define MAXBUCKET_IDX	12		/* 4096 bytes = 1 page */
#define MINALLOC_SZ	(1 << MINBUCKET_IDX)
#define MAXALLOC_SZ	(1 << MAXBUCKET_IDX)

/*
 * For sanity checks, we sign buffers as either free or
 * allocated. Memorable hex strings are chosen to easily
 * distinguish those signatures in memory dumps.
 */

#define FREEBUF_SIG	0xcafebabe	/* Hot free babe (buffer) */
#define ALLOCBUF_SIG	0xdeadbeef	/* Allocated beef (buffer) */

#ifndef __ASSEMBLY__

/*
 * Take care not to mess with the first 8-byte pointer
 * area while signing the buffer
 */
static inline void sign_buf(void *buf, uint32_t signature)
{
	buf = (char *)buf + sizeof(void *);
	*(uint32_t *)buf = signature;
}

static inline int is_free_buf(void *buf)
{
	buf = (char *)buf + sizeof(void *);
	if (*(uint32_t *)buf == FREEBUF_SIG)
		return 1;

	return 0;
}

void *__kmalloc(int bucket_idx);

void kmalloc_init(void);

/*
 * Test cases driver
 */

#if	KMALLOC_TESTS

void kmalloc_run_tests(void);

#else

static void __unused kmalloc_run_tests(void) { }

#endif /* !KMALLOC_TESTS */

#endif /* !__ASSEMBLY__ */

#endif /* _KMALLOC_H */

