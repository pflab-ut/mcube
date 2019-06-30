/**
 * @file include/mcube/sched.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_SCHED_H__
#define __MCUBE_MCUBE_SCHED_H__

#ifndef __ASSEMBLY__

/*
 * Thread Scheduling
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 */

/*
 * NOTE: This implementation is from x86_64 codes in cuteOS.
 * Both Mcube kernel and cuteOS codes exist.
 *
 * TODO: integrate both codes.
 */


/**
 * @def HZ
 * @brief System clock ticks per second.
 */
#define HZ 250

/**
 * @def RR_INTERVAL
 * @brief A thread round-robin slice in number of ticks.
 */
#define RR_INTERVAL 2

/**
 * @def MIN_PRIO
 * @brief Minimum thread priority.
 */
#define MIN_PRIO 0

/**
 * @def MAX_PRIO
 * @brief Maximum thread priority.
 */
#define MAX_PRIO 19

/*
 * Priorities only affect latency, not CPU usage.
 */

/**
 * @def DEFAULT_PRIO
 * @brief Default priority.
 */
#define DEFAULT_PRIO 10

/**
 * @def UNDEF_PRIO
 * @brief Undefined priority.
 */
#define UNDEF_PRIO -1

/**
 * @def VALID_PRIO(prio)
 * @brief Valid priority
 *
 * @param prio Priority.
 */
#define VALID_PRIO(prio) (MIN_PRIO <= (prio) && (prio) <= MAX_PRIO)

/**
 * @struct runqueue
 * @brief Runqueue information.
 *
 * The runqueue: a bucket array holding heads of
 * the lists connecting threads of equal priority.
 */
struct runqueue {
  /**
   * Array of heads of lists.
   */
  struct list_node head[MAX_PRIO + 1];
};

/**
 * @fn static inline void rq_init(struct runqueue *rq)
 * @brief initialize runqueue
 *
 * @param rq Runqueue.
 */
static inline void rq_init(struct runqueue *rq)
{
  for (int i = MIN_PRIO; i <= MAX_PRIO; i++) {
    list_init(&rq->head[i]);
  }
}

/**
 * @fn static inline int rq_get_highest_prio(struct runqueue *rq)
 * @brief get highest priority head index.
 *
 * @param rq Runqueue.
 * @brief Highest priority head index.
 */
static inline int rq_get_highest_prio(struct runqueue *rq)
{
  for (int i = MAX_PRIO; i >= MIN_PRIO; i--) {
    if (!list_empty(&rq->head[i])) {
      return i;
    }
  }

  return UNDEF_PRIO;
}

/**
 * @fn static inline bool rq_empty(struct runqueue *rq)
 * @brief Is runqueue empty?
 *
 * @param rq Runqueue.
 * @return True if runqueue is empty.
 */
static inline bool rq_empty(struct runqueue *rq)
{
  if (rq_get_highest_prio(rq) == UNDEF_PRIO) {
    return true;
  }

  return false;
}

/**
 * @struct percpu_sched
 * @brief Per-CPU area scheduling elements.
 *
 * NOTE! Always disable interrupts before accessing any
 * of these elements outside of the timer IRQ context.
 */
struct percpu_sched {
  /**
   * System ticks.
   */
  volatile clock_t sys_ticks;

  /**
   * Runnable runqueues.
   */
  struct runqueue rrq[2];

  /**
   * Active runqueue.
   */
  struct runqueue *rq_active;

  /**
   * Expired runqueue.
   */
  struct runqueue *rq_expired;

  /**
   * Just queued node.
   */
  struct list_node just_queued;

  /**
   * Current priority.
   */
  int current_prio;

  /**
   * Just queued turn.
   */
  int just_queued_turn;
};


/**
 * @enum cpu_type
 * @brief CPU type.
 */
enum cpu_type {
  BOOTSTRAP,
  SECONDARY,
};

/**
 * @enum enqueue_type
 * @brief Enqueue type.
 * @a ENQ_NORMAL: Enqueue given thread as if it's a newly created
 * thread in the system (clear its state).
 *
 * @a ENQ_RETURN: Return a thread to its original queue priority.
 * Don't mess with ANY of its CPU usage counters state. This is
 * for threads preempted by higher-priroty tasks.
 */
enum enqueue_type {
  ENQ_NORMAL,
  ENQ_RETURN,
};

struct process;


/**
 * @fn void sched_percpu_area_init(void)
 * @brief initialize all scheduler globals (runqueues + book-keeping).
 * Every core has its own unique version of such memory.
 *
 * NOTE! Disable interrupts for any code accessing such globals
 * outside of the timer IRQ context. Check include/x86_64/percpu.h
 */
void sched_percpu_area_init(void);

/**
 * @fn void schedulify_this_code_path(enum cpu_type type)
 * @brief let current CPU-init code path be a schedulable entity.
 *
 * Once the scheduler's timer starts ticking, every code path
 * should be in the context of a Schedulable Entity so it can,
 * once interrupted, get later executed. All cores (bootstrap
 * and secondary) initialization path should call this method.
 *
 * A unique 'current' descriptor, representing the initializa-
 * tion path, should be already allocated.
 *
 * @param type CPU type.
 */
void schedulify_this_code_path(enum cpu_type type);

/**
 * @fn void sched_init(void)
 * @brief initialize scheduling.
 */
void sched_init(void);

/**
 * @fn void sched_enqueue(struct process *)
 * @brief enqueue process in runqueue.
 *
 * @param process Process.
 */
void sched_enqueue(struct process *process);

/**
 * @fn struct process *sched_tick(void)
 * @brief gets invoked HZ times per second.
 *
 * @return Running process.
 */
struct process *sched_tick(void);

struct th_attr;

/**
 * @fn void kthread_create(void (* func)(void *), void *arg, struct th_attr *attr)
 * @brief create a new kernel thread running given function code,
 * and attach it to the runqueue.
 * NOTE! given function must never exit!
 *
 * @param func Function pointer to run
 * @param arg Arguments.
 * @param attr Thread attributes.
 */
void kthread_create(void (*func)(void *), void *arg, struct th_attr *attr);

/**
 * @fn uint64_t kthread_alloc_pid(void)
 * @brief allocate a unique thread ID.
 */
uint64_t kthread_alloc_pid(void);

/**
 * @var sys_jiffies
 * @brief World clock.
 */
extern unsigned long sys_jiffies;

#if CONFIG_ARCH_SIM

/**
 * @def SYS_JIFFIES_UNIT
 * @brief System jiffies unit.
 */
#define SYS_JIFFIES_UNIT        1       /* ratio */

//#define SYS_JIFFIES_UNIT       100       /* ratio */
/**
 * @def EXEC_JIFFIES_UNIT
 * @brief Execution jiffies unit.
 */
#define EXEC_JIFFIES_UNIT       1       /* ratio */

/**
 * @def SU2EU(s)
 * @brief System to execution jiffies unit.
 */
#define SU2EU(s)        ((uint64_t) FLOOR((s) * SYS_JIFFIES_UNIT, EXEC_JIFFIES_UNIT))

/**
 * @def EU2SU(e)
 * @brief Execution to system jiffies unit.
 */
#define EU2SU(e)        ((uint64_t) FLOOR((e) * EXEC_JIFFIES_UNIT, SYS_JIFFIES_UNIT))

/**
 * @var exec_jiffies
 * @brief Execution jiffies.
 */
extern unsigned long exec_jiffies;

#endif /* CONFIG_ARCH_SIM */

/**
 * @fn static inline unsigned long get_current_jiffies(void)
 * @brief get system jiffies.
 *
 * @return System jiffies.
 */
static inline unsigned long get_current_jiffies(void)
{
  return sys_jiffies;
}

/**
 * @fn static inline void update_jiffies(void)
 * @brief update jiffies.
 */
static inline void update_jiffies(void)
{
#if CONFIG_ARCH_SIM
  exec_jiffies = 0;
#endif
  sys_jiffies++;
}

/**
 * @fn static inline unsigned long get_current_cpu_time(void)
 * @brief get current CPU time.
 *
 * @return Current CPU time.
 */
static inline unsigned long get_current_cpu_time(void);

/**
 * @fn static inline unsigned long tsc2usec(unsigned long tsc)
 * @brief change time stamp counter to microsecond.
 *
 * @param tsc Time stamp counter.
 * @return Microsecond.
 */
static inline unsigned long tsc2usec(unsigned long tsc);

/**
 * @fn static inline unsigned long tsc2nsec(unsigned long tsc)
 * @brief change time stamp counter to nanosecond.
 *
 * @param tsc Time stamp counter.
 * @return Nanosecond.
 */
static inline unsigned long tsc2nsec(unsigned long tsc);

/**
 * @var bindex[NR_CPUS]
 * @brief Bitmap index.
 */
extern int bindex[NR_CPUS];


/**
 * @var nr_tasks
 * @brief Number of tasks.
 */
extern unsigned long nr_tasks;

/**
 * @var current_th[NR_CPUS]
 * @brief Current threads.
 */
extern struct thread_struct *current_th[NR_CPUS];

/**
 * @var prev_th[NR_CPUS]
 * @brief Previous threads.
 */
extern struct thread_struct *prev_th[NR_CPUS];

/**
 * @var sched_time
 * @brief Time to run scheduler.
 */
extern unsigned long sched_time;

/**
 * @var sched_end
 * @brief Is scheduler expired?
 */
extern volatile bool sched_end;

/**
 * @var sleep_tq[NR_CPUS]
 * @brief Sleep task queue.
 */
extern struct thread_struct *sleep_tq[NR_CPUS];

/**
 * @var deadline_tq[NR_CPUS]
 * @brief Deadline task queue.
 */
extern struct thread_struct *deadline_tq[NR_CPUS];

/**
 * @fn void init_rq(void)
 * @brief initialize runqueue.
 */
void init_rq(void);

/**
 * @fn bool check_deadline_miss(void)
 * @brief check deadline miss.
 */
bool check_deadline_miss(void);

/**
 * @fn void init_sched(void)
 * @brief initialize scheduling.
 */
void init_sched(void);

/**
 * @fn void do_sched(void)
 * @brief do scheduling.
 */
void do_sched(void);

/**
 * @fn void schedule_tail(void)
 * @brief do tail operation in scheduling.
 */
void schedule_tail(void);

/**
 * @fn void do_release(void)
 * @brief do release threads.
 */
void do_release(void);

/**
 * @fn int sleep_until(unsigned long release, unsigned long state)
 * @brief sleep until @a release time.
 *
 * @param release Release time.
 * @param state Sleep state.
 * @return Zero if success.
 */
int sleep_until(unsigned long release, unsigned long state);

/**
 * @fn int activate(struct thread_struct *th)
 * @brief activate thread.
 *
 * @param th Thread.
 * @return Zero if success.
 */
int activate(struct thread_struct *th);

/**
 * @fn void end_periodic_job(struct thread_struct *th)
 * @brief end periodic job.
 *
 * @param th Thread.
 */
void end_periodic_job(struct thread_struct *th);

/**
 * @fn void end_aperiodic_job(struct thread_struct *th)
 * @brief end aperiodic job.
 *
 * @param th Thread.
 */
void end_aperiodic_job(struct thread_struct *th);

/**
 * @fn void do_end_job(struct thread_struct *th)
 * @brief end job.
 *
 * @param th Thread.
 */
void do_end_job(struct thread_struct *th);

/**
 * @fn int run(unsigned long nr_threads)
 * @brief run threads.
 *
 * @param nr_threads Number of threads.
 * @return Zero if success, and nonzero if failure.
 */
int run(unsigned long nr_threads);

/**
 * @enum budget_policy
 * @brief Budget policy.
 */
enum budget_policy {
  NO_ENFORCEMENT = 1,
  TICK_ENFORCEMENT,
  PRECISE_ENFORCEMENT
};



/**
 * @struct sched_info
 * @brief Scheduling information.
 */
struct sched_info {

  /**
   * Absolute release time.
   */
  unsigned long release;

  /**
   * Absolute deadline.
   */
  unsigned long deadline;

  /*
   * NOTE: Period for periodic thread,
   * min. inter-arrival time for sporadic thread,
   * or ULONG_MAX for aperiodic thread.
   */
  /**
   * Period.
   */
  unsigned long period;

  /**
   * Worst case execution time.
   */
  unsigned long wcet;

  /**
   * CPU time to begin execution.
   */
  unsigned long begin_cpu_time;

  /**
   * Remaining execution time.
   */
  long remaining;

  /**
   * Sum of execution time in all jobs.
   */
  unsigned long sum_exec_time;

  /**
   * Flag to check if release.
   */
  unsigned long is_crelease;

  /**
   * Relative deadline.
   */
  unsigned long relative_deadline;

  /**
   * Budget policy.
   */
  enum budget_policy budget_policy;
};

/**
 * @typedef sched_info
 * @brief Typedef of @struct sched_info.
 */
typedef struct sched_info sched_info;


#endif /* !__ASSEMBLY__ */



#endif /* __MCUBE_MCUBE_SCHED_H__ */
