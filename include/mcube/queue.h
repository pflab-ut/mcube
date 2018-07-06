/**
 * @file include/mcube/queue.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_SCHED_QUEUE_H__
#define __MCUBE_MCUBE_SCHED_QUEUE_H__

#ifndef __ASSEMBLY__

/**
 * @brief Binominal heap node
 *
 * The bheap_node structure has binominal heap node information.
 */
struct bheap_node {
	/** Node ID. */
  unsigned long node_id;
	/** Pointer to parent node. */
  struct bheap_node *parent;
	/** Pointer to next node. */
  struct bheap_node *next;
	/** Pointer to child node. */
  struct bheap_node *child;

	/** Degree. */
  unsigned long degree;
	/** Value in node. */
  struct thread_struct *value;
  //  struct bheap_node **ref;
};

typedef struct bheap_node bheap_node;

/**
 * @brief Runqueue information
 *
 * The runqueue structure has runqueue information.
 */
struct runqueue {
	/** Lock to runqueue. */
  spinlock lock;
	/** Array to manage threads. */
  struct thread_struct array[NR_PRIORITIES];
	/** Bitmap. */
  uint32_t bitmap[NR_PRIORITY_BITMAPS];
	/** Utilization of runqueue. */
  unsigned long util;
	/** Number of threads in runqueue */
	int nr_threads;
#if CONFIG_TQ_BHEAP
  struct bheap_node *head;
  struct bheap_node *min;
#endif /* CONFIG_TQ_BHEAP */
};

typedef struct runqueue runqueue;

#define NR_CPU_BITMAPS (CEIL(NR_INTRA_KERNEL_CPUS, 32))

/**
 * @brief Cpurunqueue information
 *
 * The cpurunqueue structure has cpurunqueue information.
 */
struct cpurunqueue {
	/** Runqueue. */
  struct runqueue rq;
	/** Bitmap to check if processor is idle. */
  uint32_t idlebitmap[NR_CPU_BITMAPS];
};

typedef struct cpurunqueue cpurunqueue;

extern runqueue run_tq[NR_INTRA_KERNEL_CPUS];
extern struct cpurunqueue cpu_run_tq;

/* partitioned scheduling */
extern unsigned long nr_ptasks[NR_INTRA_KERNEL_CPUS];

extern void enqueue_rq_head(struct runqueue *rq, struct thread_struct *th);
extern void enqueue_rq_queue_head(struct runqueue *rq, struct thread_struct *th);
extern void enqueue_rq(struct runqueue *rq, struct thread_struct *th);
extern void enqueue_rq_queue(struct runqueue *rq, struct thread_struct *th);
extern void enqueue_cpu_rq(struct cpurunqueue *rq, struct thread_struct *th, int cpuindex);
extern void enqueue_cpu_rq_head(struct cpurunqueue *rq, struct thread_struct *th, int cpuindex);


extern struct thread_struct *compare_thread(struct thread_struct *th,
                                     struct thread_struct *n,
                                     unsigned long offset);
extern struct thread_struct *enqueue_thread(struct thread_struct *head,
                                     struct thread_struct *th,
                                     unsigned long offset);
extern struct thread_struct *enqueue_deadline_thread(struct thread_struct *head,
																							struct thread_struct *th,
																							unsigned long offset);
extern struct thread_struct *dequeue_thread(struct thread_struct *head, struct thread_struct *th);
extern struct thread_struct *dequeue_deadline_thread(struct thread_struct *head, struct thread_struct *th);
extern void dequeue_rq(struct runqueue *rq, struct thread_struct *th);
extern void dequeue_rq_queue(struct runqueue *rq, struct thread_struct *th);
extern void dequeue_cpu_rq(struct cpurunqueue *cpu_rq, struct thread_struct *th);


extern struct thread_struct *pick_next_task(void);
extern int do_partitioned(struct thread_struct *th, int (*func)(struct thread_struct *, struct thread_struct *, int));
extern void init_global(void);
extern void assign_global(void);
extern int partitioned_test(struct thread_struct *head, struct thread_struct *th, int cpu);

#if CONFIG_TQ_BHEAP
#define NOT_IN_BHEAP ULONG_MAX

typedef int (*thread_prio_t)(struct thread_struct *a, struct thread_struct *b);
extern thread_prio_t thread_prio;

extern struct bheap_node bh_nodes[NR_THREADS];

extern struct bheap_node *merge_bheap(struct bheap_node *p, struct bheap_node *q);
extern int empty_bheap(struct runqueue *rq);
extern void link_bheap(struct bheap_node *root, struct bheap_node *child);
extern void union_bheap(struct runqueue *rq, struct bheap_node *h2);
extern void insert_bheap(struct runqueue *rq, struct bheap_node *node);
extern void min_bheap(struct runqueue *rq, struct bheap_node **prev, struct bheap_node **node);
extern void min_uncache(struct runqueue *rq);
extern void union_bheap_uncache(struct runqueue *target, struct runqueue *addition);
extern struct bheap_node *reverse_bheap(struct bheap_node *h);
extern struct bheap_node *extract_min_bheap(struct runqueue *rq);
extern struct bheap_node *take_bheap(struct runqueue *rq);
extern struct bheap_node *peek_bheap(struct runqueue *rq);
extern void decrease_bheap(struct runqueue *rq, struct bheap_node *node);
extern void delete_bheap(struct runqueue *rq, struct bheap_node *node);

extern void print_bheap(struct runqueue *rq, struct bheap_node *th);

#endif /* CONFIG_TQ_BHEAP */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_SCHED_QUEUE_H__ */
