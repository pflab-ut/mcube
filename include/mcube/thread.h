/**
 * @file include/mcube/thread.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_THREAD_H__
#define	__MCUBE_MCUBE_THREAD_H__

#define THREAD_SIZE PAGE_SIZE


/* -- Thread State --
 *	RUNNING STATE: currently running
 *	READY STATE: ready to run at any moment
 *	INT_SLEEP STATE: waiting for the wake_up time (interruptible by an event)
 *	NON_INT_SLEEP STATE: waiting for the wake_up time (not interruptible by any event)
 *	WAIT STATE: waiting to be woken-up by others (due to self suspend)
 *	BLOCKED STATE: blocked for a specific reason (e.g. semaphore)
 *	UNADMITTED STATE: need to go through admission test
 */
#define	RUNNING (0x1 << 0)
#define	READY (0x1 << 1)
#define	INT_SLEEP (0x1 << 2)
#define	NON_INT_SLEEP (0x1 << 3)
#define	WAIT (0x1 << 4)
#define	BLOCKED (0x1 << 5)
#define	UNADMITTED (0x1 << 6)

/*
 * -- Thread Type --
 */
#define	PERIODIC_TH (0x1 << 0)
#define	APERIODIC_TH (0x1 << 1)
#define	SPORADIC_TH (0x1 << 2)
/* type of computation */
#define	IMPRECISE_TH (0x1 << 3)
/* type of deadline */
#define	HARD_REAL_TIME (0x1 << 4)
#define	SOFT_REAL_TIME (0x1 << 5)
#define	NON_REAL_TIME (0x1 << 6)

/* 
 * -- Thread flags --
 */
#define THFLAGS_START_TH (0x1 << 0)
#define THFLAGS_DL_MISS (0x1 << 1)
#define THFLAGS_TERM_OPTIONAL (0x1 << 2)
#define THFLAGS_HIGHEST_PRIORITY (0x1 << 3)


#ifndef	__ASSEMBLY__


#if CONFIG_TQ_BHEAP
struct bheap_node;
#endif /* CONFIG_TQ_BHEAP */

/**
 * @brief Thread information
 *
 * The thread_struct structure has the sched_info structure to manage scheduling information.
 */
struct thread_struct {
  /** Context of CPU. */
  struct cpu_context cpu_context;
	/** Pointer to previous runqueue. */
	struct thread_struct *prev;
	/** Pointer to next runqueue. */
	struct thread_struct *next;
	/** Pointer to previous deadline queue. */
	struct thread_struct *dprev;
	/** Pointer to next deadline queue. */
	struct thread_struct *dnext;
	/** Thread state. */
	unsigned long state;
	/** Thread type. */
	unsigned long type;
	/** Thread flags. */
	unsigned long thflags;
	/** Thread ID. */
	unsigned long id;
	/** Current job ID. */
	unsigned long job_id;

	/** Current CPU ID. */
	unsigned long current_cpu;
	/** Previous CPU ID. */
	unsigned long prev_cpu;

	/** Pointer to task. */
	struct task_struct *tk;
	/** Pointer to the location of thread in task. */
	struct thread_struct *line;

	/** Signal port. */
	struct signal_port *sig_port;
	/** Event message port. */
	struct message_port *evmsg_port;
	/** Priority of thread. */
	unsigned long priority;
	/** Utilization of thread. */
	unsigned long util;
	/** Number of shared resources. */
	unsigned long nr_resources;
	/** Scheduling information. */
	struct sched_info sched;
	/** Address of stack top. */
	unsigned long stack_top;
#if CONFIG_SYNC_SRP
	/** Number of stack resource. */
	unsigned long nr_stack_resources;
#endif /* CONFIG_SYNC_SRP */

#if CONFIG_TQ_BHEAP
	/** Pointer to node. */
	struct bheap_node *node;
#endif /* CONFIG_TQ_BHEAP */

  /** Argument. */
  void *arg;
  
#if CONFIG_ARCH_AXIS
  /** Current frame pointer. */
  unsigned long current_fp;
#endif /* CONFIG_ARCH_AXIS */
#if CONFIG_ARCH_X86
  /* rsp in interrupt_context_t */
  unsigned long rsp;
  /* pointer interrupt_context_t */
  interrupt_context_t *context;
#endif
  /** Current stack pointer. */
  unsigned long current_sp;
  /** Interrupt program counter. */
  unsigned long interrupt_program_counter;
  
	/** Number of preemptions per job. */
	unsigned long preemptions;
	/** Number of migrations per job. */
	unsigned long migrations;
  
  /** Function pointer to running thread function. */
  void (*run_func)(void);
  /** Function pointer to running user thread function. */
  void *(*run_user_func)(void *);
};

typedef struct thread_struct thread_struct;

/**
 * @brief Thread attribute for task set 
 *
 * The th_attr structure sets up task set for scheduling.
 */
struct th_attr {
	/** Type. */
	unsigned long type;
	/** Latency to release. */
	unsigned long phase;
	/** Period. */
	unsigned long period;
	/** Worst case execution time. */
	unsigned long wcet;
	/** Relative deadline. */
	unsigned long relative_deadline;
};

typedef struct th_attr th_attr;


typedef struct th_attr_strip th_attr_strip;

void init_sched_info(struct thread_struct *th, struct th_attr *attr);

static inline unsigned long get_current_jiffies(void);

static inline int is_budget_enforced(struct thread_struct *th)
{
	return th->sched.budget_policy == TICK_ENFORCEMENT || th->sched.budget_policy == PRECISE_ENFORCEMENT;
}

static inline int is_budget_exhausted(struct thread_struct *th)
{
	if (th->sched.remaining <= 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}



static inline int is_preempted(struct thread_struct *th)
{
	return th->sched.release != get_current_jiffies() && !is_budget_exhausted(th)
		&& th->sched.remaining != th->sched.exec_cost;
}



static inline unsigned long get_util(struct thread_struct *th)
{
	return (th->sched.wcet * 100) / th->sched.period;
}

void __attribute__((noreturn)) run_user_thread(void);


void set_priority(struct thread_struct *th);
unsigned long get_priority(struct thread_struct *th);
void do_switch_thread(void);

void thread_main(struct thread_struct *th);

extern struct th_attr thas[NR_THREADS];
extern struct thread_struct ths[NR_THREADS];

extern int is_release[NR_THREADS];
extern int is_finishing[NR_THREADS];


#define TH_LOWEST_PRIORITY ((unsigned long) ~0x0)

/* idle thread ID */
#define	IDLE_THREAD_ID 0

/* first dynamically created thread */
#define	FIRST_DYNAMIC_THREAD_ID 1

extern struct thread_struct idle_th[NR_INTRA_KERNEL_CPUS];

int alloc_thread_id(void);
struct thread_struct *do_create_thread(void *(*func)(void *),
                                       void *arg,
                                       struct th_attr *attr);


extern unsigned char idle_stack[NR_INTRA_KERNEL_CPUS][STACK_SIZE];
extern unsigned char stack[NR_THREADS][STACK_SIZE];

void (*th_mains[NR_THREADS])(void);


#define IDLE_THREAD_STACK_ADDR(cpu) ((unsigned long) &idle_stack[cpu][STACK_SIZE])
#define THREAD_STACK_ADDR(id) ((unsigned long) &stack[id][0])



int thread_tie_break(struct thread_struct *x, struct thread_struct *y);

static inline int thread_is_high_prio(struct thread_struct *x, struct thread_struct *y)
{
	return x->priority < y->priority
		|| ((x->priority == y->priority) && thread_tie_break(x, y));
}

static inline int thread_is_low_prio(struct thread_struct *x, struct thread_struct *y)
{
	return (x->priority > y->priority)
		|| ((x->priority == y->priority) && thread_tie_break(x, y));
}

void wait(unsigned long count);

static inline void begin_budget(struct thread_struct *th)
{
	if (th->id > 0) {
		unsigned long ctime = get_current_cpu_time();
		th->sched.begin_cpu_time = ctime;
	}
}


static inline void end_budget(struct thread_struct *th)
{
	if (th->id > 0) {
		unsigned long ctime = get_current_cpu_time();
		unsigned long exec_time = tsc2nsec(ctime - th->sched.begin_cpu_time);
		printk("end_budget(): th->id = %lu th->sched.remaining = %ld\n", th->id, th->sched.remaining);
		th->sched.remaining -= exec_time;
		printk("end_budget(): th->id = %lu th->sched.remaining = %ld\n", th->id, th->sched.remaining);
		th->sched.sum_exec_time += exec_time;
	}
}



#define INIT_THAS {                                                     \
    [0 ... NR_THREADS - 1] = {																					\
      .type = PERIODIC_TH | IMPRECISE_TH | HARD_REAL_TIME,              \
      .period = 100,																										\
      .wcet = 5,																												\
      .relative_deadline  = 100,																				\
    },                                                                  \
      }


#define INIT_IDLE_THREAD(cpu) {                         \
    .cpu_context = INIT_CPU_CONTEXT,                    \
		.prev = NULL,																				\
			.next = NULL,																			\
			.dprev = NULL,																		\
			.dnext = NULL,																		\
			.state = READY,																		\
			.type = APERIODIC_TH | NON_REAL_TIME,							\
			.thflags = 0,																			\
			.id = IDLE_THREAD_ID,															\
			.tk = NULL,                                       \
			.line = NULL,																			\
			.sig_port = NULL,																	\
			.evmsg_port = NULL,																\
			.nr_resources = 0,																\
			.priority = ULONG_MAX,														\
			.stack_top = (unsigned long) &stack[cpu][NR_INTRA_KERNEL_CPUS],	\
			.sched = {																				\
			.relative_deadline = ULONG_MAX,									\
			.period = ULONG_MAX,															\
			.wcet = ULONG_MAX,                               \
			.remaining = ULONG_MAX,													\
			.exec_cost = ULONG_MAX,													\
			.deadline = ULONG_MAX,														\
			.release = ULONG_MAX,														\
		},																									\
			}


/* make the context and allocate a stack:
 * initially, the stack holds one context area for the initial context. */
//void set_initial_context(struct thread_struct *th, void (*pc)(void *),
//																void *func, unsigned long bottom);


int get_tq_util(struct thread_struct *head, unsigned long cpu);
void switch_to(struct thread_struct *next_thread);
void arch_switch_to(struct thread_struct *prev, struct thread_struct *next);

#define get_context_top(th) \
  ((struct full_regs *)((th)->stack_top - sizeof(struct full_regs)))


#endif	/* __ASSEMBLY__ */
#endif	/* __MCUBE_MCUBE_THREAD_H__ */
