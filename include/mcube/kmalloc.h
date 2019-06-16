/**
 * @file include/mcube/kmalloc.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_KMALLOC_H__
#define __MCUBE_MCUBE_KMALLOC_H__

/*
 * Kernel Memory Allocator
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
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

/**
 * @def MINBUCKET_IDX
 * @brief Minimum bucket index.
 */
#define MINBUCKET_IDX 4

/**
 * @def MAXBUCKET_IDX
 * @brief Maximum bucket index.
 */
#define MAXBUCKET_IDX 12

/**
 * @def MINALLOC_SZ
 * @brief Minimum allocation index.
 */
#define MINALLOC_SZ (1 << MINBUCKET_IDX)

/**
 * @def MAXALLOC_SZ
 * @brief Maximum allocation index.
 */
#define MAXALLOC_SZ (1 << MAXBUCKET_IDX)

/*
 * For sanity checks, we sign buffers as either free or
 * allocated. Memorable hex strings are chosen to easily
 * distinguish those signatures in memory dumps.
 */

/**
 * @def FREEBUF_SIG
 * @brief Hot free babe (buffer).
 */
#define FREEBUF_SIG 0xcafebabe

/**
 * @def ALLOCBUF_SIG
 * @brief Allocated beef (buffer).
 */
#define ALLOCBUF_SIG 0xdeadbeef  /*  */

#ifndef __ASSEMBLY__

/**
 * @struct bucket
 * @brief A kernel memory bucket for each power-of-2 list
 *
 * A Bucket with index 'x' holds all the information for the
 * free bufs list of size (1 << x) bytes, including its head.
 */
struct bucket {
  /**
   * Bucket lock.
   */
  spinlock_t lock;

  /**
   * Free blocks list head.
   */
  void *head;

  /**
   * # of pages requested from page allocator.
   */
  int totalpages;

  /**
   * # of free buffers.
   */
  int totalfree;
};

/**
 * @var kmembuckets[MAXBUCKET_IDX + 1]
 * @brief Kernel memory buckets.
 */
extern struct bucket kmembuckets[MAXBUCKET_IDX + 1];

/**
 * @fn static inline void sign_buf(void *buf, uint32_t signature)
 * @brief sign buffer.
 * Take care not to mess with the first 8-byte pointer
 * area while signing the buffer
 */
static inline void sign_buf(void *buf, uint32_t signature)
{
  buf = (char *) buf + sizeof(void *);
  *(uint32_t *) buf = signature;
}

/**
 * @fn static inline bool is_free_buf(void *buf)
 * @brief Is buffer free?
 */
static inline bool is_free_buf(void *buf)
{
  buf = (char *) buf + sizeof(void *);

  if (*(uint32_t *) buf == FREEBUF_SIG) {
    return true;
  }

  return false;
}


/**
 * @fn void *__kmalloc(int bucket_idx)
 * @brief Kernel memory allocator.
 *
 * @param bucket_idx Bucket index.
 * @return Address to allocated memory.
 */
void *__kmalloc(int bucket_idx);

/**
 * @fn void init_kmalloc(void)
 * @brief initialize kernel memory allocator.
 */
void init_kmalloc(void);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_KMALLOC_H__ */

