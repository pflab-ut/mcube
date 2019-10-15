/**
 * @file include/mcube/thread.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_THREAD_H__
#define __MCUBE_MCUBE_THREAD_H__

/**
 * @def THREAD_SIZE
 * @brief Alias to PAGE_SIZE.
 */
#define THREAD_SIZE PAGE_SIZE


/* -- Thread State --  */

/**
 * @def RUNNING
 * @brief is currently running.
 */
#define RUNNING (0x1 << 0)

/**
 * @def READY
 * @brief is ready to run at any moment.
 */
#define READY (0x1 << 1)

/**
 * @def INT_SLEEP
 * @brief wait for the wake_up time (interruptible by an event).
 */
#define INT_SLEEP (0x1 << 2)

/**
 * @def NON_INT_SLEEP
 * @brief wait for the wake_up time (not interruptible by any event).
 */
#define NON_INT_SLEEP (0x1 << 3)

/**
 * @def WAIT
 * @brief wait to be woken-up by others (due to self suspend).
 */
#define WAIT (0x1 << 4)

/**
 * @def BLOCKED
 * @brief block for a specific reason (e.g. semaphore).
 */
#define BLOCKED (0x1 << 5)

/**
 * @def UNADMITTED
 * @brief need to go through admission test.
 */
#define UNADMITTED (0x1 << 6)

/* -- Thread Type -- */
/**
 * @def PERIODIC_TH
 * @brief Periodic thread.
 */
#define PERIODIC_TH (0x1 << 0)

/**
 * @def APERIODIC_TH
 * @brief Aperiodic thread.
 */
#define APERIODIC_TH (0x1 << 1)

/**
 * @def SPORADIC_TH
 * @brief Sporadic thread.
 */
#define SPORADIC_TH (0x1 << 2)

/* -- Type of Deadline -- */
/**
 * @def HARD_REAL_TIME
 * @brief Hard real-time.
 */
#define HARD_REAL_TIME (0x1 << 3)

/**
 * @def SOFT_REAL_TIME
 * @brief Soft real-time.
 */
#define SOFT_REAL_TIME (0x1 << 4)

/**
 * @def NON_REAL_TIME
 * @brief Non real-time.
 */
#define NON_REAL_TIME (0x1 << 5)

/* -- Thread Flags -- */
/**
 * @def THFLAGS_START_TH
 * @brief Does thread start?
 */
#define THFLAGS_START_TH (0x1 << 0)

/**
 * @def THFLAGS_DL_MISS
 * @brief Does thread miss deadline?
 */
#define THFLAGS_DL_MISS (0x1 << 1)


#ifndef __ASSEMBLY__


#if CONFIG_TQ_BHEAP
struct bheap_node;
#endif /* CONFIG_TQ_BHEAP */

/**
 * @struct thread_struct
 * @brief Thread information.
 *
 * The thread_struct structure has the sched_info structure to manage scheduling information.
 */
struct thread_struct {
  /**
   * Context of CPU.
   */
  struct cpu_context cpu_context;

  /**
   * Pointer to previous runqueue.
   */
  struct thread_struct *prev;

  /**
   * Pointer to next runqueue.
   */
  struct thread_struct *next;

  /**
   * Pointer to previous deadline queue.
   */
  struct thread_struct *dprev;

  /**
   * Pointer to next deadline queue.
   */
  struct thread_struct *dnext;

  /**
   * Thread state.
   */
  unsigned long state;

  /**
   * Thread type.
   */
  unsigned long type;

  /**
   * Thread flags.
   */
  unsigned long thflags;

  /**
   * Thread ID.
   */
  unsigned long id;

  /**
   * Current job ID.
   */
  unsigned long job_id;

  /**
   * Current CPU ID.
   */
  unsigned long current_cpu;

  /**
   * Previous CPU ID.
   */
  unsigned long prev_cpu;

  /**
   * Pointer to task.
   */
  struct task_struct *tk;

  /**
   * Pointer to the location of thread in task.
   */
  struct thread_struct *line;

  /**
   * Signal port.
   */
  struct signal_port *sig_port;

  /**
   * Event message port.
   */
  struct message_port *evmsg_port;

  /**
   * Priority of thread.
   */
  unsigned long priority;

  /**
   * Utilization of thread.
   */
  unsigned long util;

  /**
   * Number of shared resources.
   */
  unsigned long nr_resources;

  /**
   * Scheduling information.
   */
  struct sched_info sched;

  /**
   * Address of stack top.
   */
  unsigned long stack_top;

#if CONFIG_TQ_BHEAP
  /**
   * Pointer to node.
   */
  struct bheap_node *node;
#endif /* CONFIG_TQ_BHEAP */

  /**
   * Argument.
   */
  void *arg;

#if CONFIG_ARCH_AXIS
  /**
   * Current frame pointer.
   */
  unsigned long current_fp;
#endif /* CONFIG_ARCH_AXIS */

  /**
   * Current stack pointer.
   */
  unsigned long current_sp;

  /**
   * Interrupt program counter.
   */
  unsigned long interrupt_program_counter;

  /**
   * Number of preemptions per job.
   */
  unsigned long preemptions;

  /**
   * Number of migrations per job.
   */
  unsigned long migrations;

  /**
   * Function pointer to running thread function.
   */
  void (*run_func)(void);

  /**
   * Function pointer to running user thread function.
   */
  void *(*run_user_func)(void *);
};

/**
 * @typedef thread_struct
 * @brief Typedef of @struct thread_struct.
 */
typedef struct thread_struct thread_struct;

/**
 * @struct th_attr
 * @brief Thread attributes.
 *
 * The th_attr structure sets up task set for scheduling.
 */
struct th_attr {
  /**
   * Type.
   */
  unsigned long type;

  /**
   * Latency to release.
   */
  unsigned long phase;

  /**
   * Period.
   */
  unsigned long period;

  /**
   * Worst case execution time.
   */
  unsigned long wcet;

  /**
   * Relative deadline.
   */
  unsigned long relative_deadline;
};

/**
 * @typedef th_attr.
 * @brief Typedef of @struct th_attr.
 */
typedef struct th_attr th_attr;

/**
 * @fn void init_sched_info(struct thread_struct *th, struct th_attr *attr)
 * @brief initialize scheduling information.
 *
 * @param th Thread.
 * @param attr Attribute.
 */
void init_sched_info(struct thread_struct *th, struct th_attr *attr);

/**
 * @fn static inline int is_budget_enforced(struct thread_struct *th)
 * @brief Is budget enforced?
 *
 * @param th Thread.
 * @return True if budget is enforced.
 */
static inline bool is_budget_enforced(struct thread_struct *th)
{
  return th->sched.budget_policy == TICK_ENFORCEMENT
         || th->sched.budget_policy == PRECISE_ENFORCEMENT;
}

/**
 * @fn static inline bool is_budget_exhausted(struct thread_struct *th)
 * @brief Is budget exhausted?
 *
 * @param th Thread.
 * @return True if budget is exhausted.
 */
static inline bool is_budget_exhausted(struct thread_struct *th)
{
  if (th->sched.remaining <= 0) {
    return true;
  }

  return false;
}

/**
 * @fn static inline bool is_preempted(struct thread_struct *th)
 * @brief Is thread preempted?
 *
 * @param th Thread.
 * @return True if thread is preempted.
 */
static inline bool is_preempted(struct thread_struct *th)
{
  return th->sched.release != get_current_jiffies() && !is_budget_exhausted(th);
}

/**
 * @fn static inline unsigned long get_util(struct thread_struct *th)
 * @brief get CPU utilization.
 *
 * @param th Thread.
 * @return CPU utilization.
 */
static inline unsigned long get_util(struct thread_struct *th)
{
  return (th->sched.wcet * 100) / th->sched.period;
}

/**
 * @fn void run_user_thread(void)
 * @brief run user thread.
 */
void run_user_thread(void);

/**
 * @fn void set_priority(struct thread_struct *th)
 * @brief set priority.
 *
 * @param th Thread.
 */
void set_priority(struct thread_struct *th);


/**
 * @fn void do_switch_thread(void)
 * @brief switch thread.
 */
void do_switch_thread(void);

/**
 * @fn void thread_main(struct thread_struct *th)
 * @brief Thread main.
 *
 * @param th Thread.
 */
void thread_main(struct thread_struct *th);

/**
 * @var thas[NR_THREADS]
 * @brief Thread attributes.
 */
extern struct th_attr thas[NR_THREADS];

/**
 * @var ths[NR_THREADS]
 * @brief Threads.
 */
extern struct thread_struct ths[NR_THREADS];

/**
 * @def TH_LOWEST_PRIORITY
 * @brief Thread lowest priority.
 */
#define TH_LOWEST_PRIORITY (~0x0UL)

/**
 * @def KERNEL_THREAD_ID
 * @brief Kernel thread ID.
 */
#define KERNEL_THREAD_ID 0

/**
 * @def FIRST_DYNAMIC_THREAD_ID
 * @brief First dynamic thread ID.
 */
#define FIRST_DYNAMIC_THREAD_ID 1

/**
 * @var kernel_th[NR_CPUS]
 * @brief Kernel threads.
 */
extern struct thread_struct kernel_th[NR_CPUS];

/**
 * @fn int alloc_thread_id(void)
 * @brief allocate thread ID.
 */
int alloc_thread_id(void);

/**
 * @fn struct thread_struct *do_create_thread(void *(*func)(void *),
 *                                            void *arg,
 *                                            struct th_attr *attr);
 * @brief do create thread.
 *
 * @param func Function pointer to user defined function.
 * @param arg Thread arguments.
 * @param attr Thread attributes.
 * @return Pointer to created thread if success, and NULL if failure.
 */
struct thread_struct *do_create_thread(void *(*func)(void *),
                                       void *arg,
                                       struct th_attr *attr);


/**
 * @var kernel_stack[NR_CPUS][KERNEL_STACK_SIZE]
 * @brief Kernel stack.
 */
extern unsigned char kernel_stack[NR_CPUS][KERNEL_STACK_SIZE];

/**
 * @var user_stack[NR_THREADS][USER_STACK_SIZE]
 * @brief User stack.
 */
extern unsigned char user_stack[NR_THREADS][USER_STACK_SIZE];


/**
 * @def KERNEL_THREAD_STACK_ADDR(cpu)
 * @brief Stack address for kernel thread.
 *
 * @param cpu CPU.
 */
#define KERNEL_THREAD_STACK_ADDR(cpu) ((unsigned long) &kernel_stack[cpu][KERNEL_STACK_SIZE])

/**
 * @def USER_THREAD_STACK_ADDR(id)
 * @brief Stack address for user thread.
 *
 * @param id ID.
 */
#define USER_THREAD_STACK_ADDR(id) ((unsigned long) &user_stack[id][USER_STACK_SIZE])

/**
 * @fn static inline bool thread_tie_break(__unused struct thread_struct *x,
 *                                         __unused struct thread_struct *y)
 * @brief Is thread tie-break?
 *
 * @param x Thread 1.
 * @param y Thread 2.
 * @return True if @c CONFIG_TIE_BREAK_ID is true and
 * ID of @a x is smaller than that of @a y.
 */
static inline bool thread_tie_break(__unused struct thread_struct *x,
                                    __unused struct thread_struct *y)
{
#if CONFIG_TIE_BREAK_FIFO
  return false;
#elif CONFIG_TIE_BREAK_ID
  return x->id < y->id;
#else
#error "Unknown Thread Tie-Break Policy"
#endif
}

/**
 * @fn static inline bool thread_is_high_prio(struct thread_struct *x,
 *                                            struct thread_struct *y)
 * @param x Thread 1.
 * @param y Thread 2.
 * @return True if priority of @a x is lower than that of @a y.
 */
static inline bool thread_is_high_prio(struct thread_struct *x,
                                       struct thread_struct *y)
{
  return x->priority < y->priority
         || ((x->priority == y->priority) && thread_tie_break(x, y));
}

/**
 * @fn static inline bool thread_is_low_prio(struct thread_struct *x,
 *                                           struct thread_struct *y)
 * @param x Thread 1.
 * @param y Thread 2.
 * @return True if @a x's priority is higher than @a y's priority.
 */
static inline bool thread_is_low_prio(struct thread_struct *x,
                                      struct thread_struct *y)
{
  return (x->priority > y->priority)
         || ((x->priority == y->priority) && thread_tie_break(x, y));
}

/**
 * @fn static inline void begin_budget(struct thread_struct *th)
 * @brief begin budget.
 *
 * @param th Thread.
 */
static inline void begin_budget(struct thread_struct *th)
{
  if (th->id > 0) {
    th->sched.begin_cpu_time = get_current_cpu_time();
  }
}

/**
 * @fn static inline void end_budget(struct thread_struct *th)
 * @brief end budget.
 *
 * @param th Thread.
 */
static inline void end_budget(struct thread_struct *th)
{
  if (th->id > 0) {
    unsigned long ctime = get_current_cpu_time();
    unsigned long exec_time = tsc2nsec(ctime - th->sched.begin_cpu_time);
    printk("end_budget(): th->id = %lu th->sched.remaining = %ld\n", th->id,
           th->sched.remaining);
    th->sched.remaining -= exec_time;
    printk("end_budget(): th->id = %lu th->sched.remaining = %ld\n", th->id,
           th->sched.remaining);
    th->sched.sum_exec_time += exec_time;
  }
}


/**
 * @fn static inline void init_thas(struct th_attr thas[]);
 * @brief initialize thread attributes.
 */
static inline void init_thas(struct th_attr thas[])
{
  for (int i = 0; i < NR_THREADS; i++) {
    thas[i].type = PERIODIC_TH | HARD_REAL_TIME;
    thas[i].period = 100;
    thas[i].wcet = 5;
    thas[i].relative_deadline = 100;
  }
}


/**
 * @fn static inline void init_kernel_thread(struct thread_struct *th, unsigned long cpu)
 * @brief initialize kernel thread.
 *
 * @param th Thread.
 * @param cpu CPU ID.
 */
static inline void init_kernel_thread(struct thread_struct *th,
                                      unsigned long cpu)
{
  th->cpu_context = INIT_CPU_CONTEXT;
  th->prev = (struct thread_struct *) NULL;
  th->next = (struct thread_struct *) NULL;
  th->dprev = (struct thread_struct *) NULL;
  th->dnext = (struct thread_struct *) NULL;
  th->state = READY;
  th->type = APERIODIC_TH | NON_REAL_TIME;
  th->thflags = 0;
  th->id = KERNEL_THREAD_ID;
  th->tk = (struct task_struct *) NULL;
  th->line = (struct thread_struct *) NULL;
  th->sig_port = (struct signal_port *) NULL;
  th->evmsg_port = (struct message_port *) NULL;
  th->nr_resources = 0;
  th->priority = ULONG_MAX;
  th->stack_top = (unsigned long) &kernel_stack[cpu][NR_CPUS];

  th->sched.relative_deadline = ULONG_MAX;
  th->sched.period = ULONG_MAX;
  th->sched.wcet = ULONG_MAX;
  th->sched.remaining = ULONG_MAX;
  th->sched.deadline = ULONG_MAX;
  th->sched.release = ULONG_MAX;
}


/**
 * @fn void wait(unsigned long count)
 * @brief wait for @a count of loop.
 *
 * @param count Count.
 */
void wait(unsigned long count);

/**
 * @fn int get_tq_util(struct thread_struct *head, unsigned long cpu)
 * @brief get task queue's CPU utilization.
 *
 * @param head Head of task queue.
 * @param cpu CPU ID.
 * @return Task queue's CPU utilization.
 */
int get_tq_util(struct thread_struct *head, unsigned long cpu);

/**
 * @fn void arch_switch_to(struct thread_struct *prev, struct thread_struct *next)
 * @brief switch architecture-dependent context.
 *
 * @param prev Previous thread.
 * @param next Next thread.
 */
void arch_switch_to(struct thread_struct *prev, struct thread_struct *next);

/**
 * @def get_context_top(th)
 * @brief get context.
 *
 * @param th Thread.
 */
#define get_context_top(th)                                           \
  ((struct full_regs *)((th)->stack_top - sizeof(struct full_regs)))


#endif /* __ASSEMBLY__ */
#endif /* __MCUBE_MCUBE_THREAD_H__ */
