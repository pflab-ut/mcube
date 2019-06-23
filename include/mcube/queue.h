/**
 * @file include/mcube/queue.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_SCHED_QUEUE_H__
#define __MCUBE_MCUBE_SCHED_QUEUE_H__

#ifndef __ASSEMBLY__

/**
 * @struct bheap_node
 * @brief Binominal heap node.
 */
struct bheap_node {
  /**
   * Node ID.
   */
  unsigned long node_id;

  /**
   *  Pointer to parent node.
   */
  struct bheap_node *parent;

  /**
   * Pointer to next node.
   */
  struct bheap_node *next;

  /**
   * Pointer to child node.
   */
  struct bheap_node *child;

  /**
   * Degree.
   */
  unsigned long degree;

  /**
   * Value in node.
   */
  struct thread_struct *value;

  //  struct bheap_node **ref;
};

/**
 * @typedef bheap_node
 * @brief Typedef of @struct bheap_node.
 */
typedef struct bheap_node bheap_node;

/**
 * @struct rt_runqueue
 * @brief Real-time runqueue.
 */
struct rt_runqueue {
  /**
   * Lock to runqueue.
   */
  spinlock_t lock;

  /**
   * Array to manage threads.
   */
  struct thread_struct array[NR_PRIORITIES];

  /**
   * Bitmap.
   */
  uint32_t bitmap[NR_PRIORITY_BITMAPS];

  /**
   * Utilization of runqueue.
   */
  unsigned long util;

  /**
   * Number of threads in runqueue.
   */
  int nr_threads;

#if CONFIG_TQ_BHEAP
  /**
   * Head node.
   */
  struct bheap_node *head;

  /**
   * Minimum node.
   */
  struct bheap_node *min;
#endif /* CONFIG_TQ_BHEAP */
};

/**
 * @typedef rt_runqueue
 * @brief Typedef of @struct rt_runqueue.
 */
typedef struct rt_runqueue rt_runqueue;

/**
 * @def NR_CPU_BITMAPS
 * @brief Number of CPU bitmaps.
 */
#define NR_CPU_BITMAPS (CEIL(NR_CPUS, 32))

/**
 * @var run_tq[NR_CPUS]
 * @brief Array of running task queue.
 */
extern rt_runqueue run_tq[NR_CPUS];

/**
 * @fn void enqueue_rq(struct rt_runqueue *rq, struct thread_struct *th)
 * @brief enqueue runqueue.
 *
 * @param rq Runqueue.
 * @param th Thread.
 */
void enqueue_rq(struct rt_runqueue *rq, struct thread_struct *th);

/**
 * @fn void enqueue_rq_queue(struct rt_runqueue *rq, struct thread_struct *th)
 * @brief enqueue queue-dependent runqueue.
 *
 * @param rq Runqueue.
 * @param th Thread.
 */
void enqueue_rq_queue(struct rt_runqueue *rq, struct thread_struct *th);

/**
 * @fn struct thread_struct *compare_thread(struct thread_struct *th,
 *                                          struct thread_struct *n,
 *                                          unsigned long offset);
 * @brief compare thread
 *
 * @param th Thread 1.
 * @param n Thread 2.
 * @param offset Offset of compared value.
 * @return @a th if value of @a th is smaller than that of @a n.
 */
struct thread_struct *compare_thread(struct thread_struct *th,
                                     struct thread_struct *n,
                                     unsigned long offset);

/**
 * @fn struct thread_struct *enqueue_thread(struct thread_struct *head,
 *                                          struct thread_struct *th,
 *                                          unsigned long offset);
 * @brief enqueue thread.
 *
 * @param head Head.
 * @param th Thread.
 * @param offset Offset of compared value.
 * @return (Updated) head.
 */
struct thread_struct *enqueue_thread(struct thread_struct *head,
                                     struct thread_struct *th,
                                     unsigned long offset);

/**
 * @fn struct thread_struct *enqueue_deadline_thread(struct thread_struct *head,
 *                                                   struct thread_struct *th,
 *                                                   unsigned long offset);
 * @brief enqueue thread by deadline.
 *
 * @param head Head.
 * @param th Thread.
 * @param offset Offset of compared thread.
 * @return (Updated) head.
 */
struct thread_struct *enqueue_deadline_thread(struct thread_struct *head,
                                              struct thread_struct *th,
                                              unsigned long offset);

/**
 * @fn struct thread_struct *dequeue_thread(struct thread_struct *head,
 *                                          struct thread_struct *th);
 * @brief enqueue thread.
 *
 * @param head Head.
 * @param th Thread.
 * @return (Updated) head.
 */
struct thread_struct *dequeue_thread(struct thread_struct *head,
                                     struct thread_struct *th);

/**
 * @fn struct thread_struct *dequeue_deadline_thread(struct thread_struct *head,
 *                                                   struct thread_struct *th);
 * @brief dequeue thread by deadline.
 *
 * @param head Head.
 * @param th Thread.
 * @return (Updated) head.
 */
struct thread_struct *dequeue_deadline_thread(struct thread_struct *head,
                                              struct thread_struct *th);

/**
 * @fn void dequeue_rq(struct rt_runqueue *rq, struct thread_struct *th)
 * @brief dequeue runqueue.
 *
 * @param rq Runqueue.
 * @param th Thread.
 */
void dequeue_rq(struct rt_runqueue *rq, struct thread_struct *th);

/**
 * @fn void dequeue_rq_queue(struct rt_runqueue *rq, struct thread_struct *th)
 * @brief dequeue queue-dependent runqueue.
 *
 * @param rq Runqueue.
 * @param th Thread.
 */
void dequeue_rq_queue(struct rt_runqueue *rq, struct thread_struct *th);

/**
 * @fn struct thread_struct *pick_next_thread(void)
 * @brief pick next thread.
 *
 * @return Thread.
 */
struct thread_struct *pick_next_thread(void);


#if CONFIG_TQ_BHEAP

/**
 * @def NOT_IN_BHEAP
 * @brief Node is not in bheap.
 */
#define NOT_IN_BHEAP ULONG_MAX

/**
 * @typedef int (*thread_prio_t)(struct thread_struct *a, struct thread_struct *b)
 * @brief Typedef of thread priority.
 */
typedef int (*thread_prio_t)(struct thread_struct *a, struct thread_struct *b);

/**
 * @var thread_prio
 * @brief Thread priority.
 */
extern thread_prio_t thread_prio;

/**
 * @var bh_nodes[NR_THREADS]
 * @brief Array of bheap node.
 */
extern struct bheap_node bh_nodes[NR_THREADS];

/**
 * @fn struct bheap_node *merge_bheap(struct bheap_node *p, struct bheap_node *q)
 * @brief merge bheap nodes.
 *
 * @param p Node.
 * @param q Node.
 * @return Merged head node.
 */
struct bheap_node *merge_bheap(struct bheap_node *p, struct bheap_node *q);

/**
 * @fn bool empty_bheap(struct rt_runqueue *rq)
 * @brief Is bheap empty?
 *
 * @param rq Runqueue.
 * @return True if bheap is empty.
 */
bool empty_bheap(struct rt_runqueue *rq);

/**
 * @fn void link_bheap(struct bheap_node *root, struct bheap_node *child)
 * @brief make child a subtree of root.
 *
 * @param root Root node.
 * @param chil Child node.
 */
void link_bheap(struct bheap_node *root, struct bheap_node *child);

/**
 * @fn void union_bheap(struct rt_runqueue *rq, struct bheap_node *h2)
 * @brief union bheap.
 *
 * @param rq Runqueue.
 * @param h2 Node.
 */
void union_bheap(struct rt_runqueue *rq, struct bheap_node *h2);

/**
 * @fn void insert_bheap(struct rt_runqueue *rq, struct bheap_node *node)
 * @brief insert (and reinitialize) a node into the bheap.
 *
 * @param rq Runqueue.
 * @param node Node.
 */
void insert_bheap(struct rt_runqueue *rq, struct bheap_node *node);

/**
 * @fn void min_bheap(struct rt_runqueue *rq, struct bheap_node **prev,
 *                    struct bheap_node **node);
 * @brief get minimum node.
 *
 * @param rq Runqueue.
 * @param prev Previous node to minimum node.
 * @param node Minimum Node.
 */
void min_bheap(struct rt_runqueue *rq, struct bheap_node **prev,
               struct bheap_node **node);

/**
 * @fn struct bheap_node *min_bheap_extract(struct rt_runqueue *rq)
 * @brief get minimum node with extraction.
 *
 * @param rq Runqueue.
 * @return Minimum node.
 */
struct bheap_node *min_bheap_extract(struct rt_runqueue *rq);

/**
 * @fn void min_bheap_uncache(struct rt_runqueue *rq)
 * @brief get minimum node with uncache.
 */
void min_bheap_uncache(struct rt_runqueue *rq);

/**
 * @fn void init_bheap_node(struct bheap_node *h, struct thread_struct *value)
 * @brief initialize bheap node.
 *
 * @param h Node.
 * @param value Thread.
 */
void init_bheap_node(struct bheap_node *h, struct thread_struct *value);

/**
 * @fn void union_bheap_uncache(struct rt_runqueue *target,
 *                              struct rt_runqueue *addition)
 * @brief merge @a addition into @a target.
 *
 * @param target Target runqueue.
 * @param addition Addition runqueue.
 */
void union_bheap_uncache(struct rt_runqueue *target,
                         struct rt_runqueue *addition);

/**
 * @fn struct bheap_node *reverse_bheap(struct bheap_node *h)
 * @brief reverse a linked list of nodes. also clears parent pointer.
 *
 * @param h Node.
 * @return Reversed header node.
 */
struct bheap_node *reverse_bheap(struct bheap_node *h);

/**
 * @fn void decrease_bheap(struct rt_runqueue *rq, struct bheap_node *node)
 * @brief decrease bheap.
 *
 * @param rq Runqueue.
 * @param node Node.
 */
void decrease_bheap(struct rt_runqueue *rq, struct bheap_node *node);

/**
 * @fn void delete_bheap(struct rt_runqueue *rq, struct bheap_node *node)
 * @brief delete bheap.
 *
 * @param rq Runqueue.
 * @param node Node.
 */
void delete_bheap(struct rt_runqueue *rq, struct bheap_node *node);

/**
 * @fn void print_bheap(struct rt_runqueue *rq, struct bheap_node *th)
 * @brief print bheap.
 *
 * @param rq Runqueue.
 * @param th Thread.
 */
void print_bheap(struct rt_runqueue *rq, struct bheap_node *th);

#endif /* CONFIG_TQ_BHEAP */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_SCHED_QUEUE_H__ */
