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
 * System clock ticks per second
 */
#define HZ      250

/*
 * A thread round-robin slice in number of ticks
 */
#define RR_INTERVAL    2

/*
 * Threads priority boundaries
 */
#define    MIN_PRIO  0
#define    MAX_PRIO  19

/*
 * Priorities only affect latency, not CPU usage.
 */
#define DEFAULT_PRIO    10
#define UNDEF_PRIO    -1
#define VALID_PRIO(prio)      \
  (MIN_PRIO <= (prio) && (prio) <= MAX_PRIO)

/*
 * The runqueue: a bucket array holding heads of
 * the lists connecting threads of equal priority.
 */
struct runqueue {
  struct list_node head[MAX_PRIO + 1];
};

static inline void rq_init(struct runqueue *rq)
{
  for (int i = MIN_PRIO; i <= MAX_PRIO; i++)
    list_init(&rq->head[i]);
}

static inline int rq_get_highest_prio(struct runqueue *rq)
{
  for (int i = MAX_PRIO; i >= MIN_PRIO; i--)
    if (!list_empty(&rq->head[i]))
      return i;

  return UNDEF_PRIO;
}

static inline bool rq_empty(struct runqueue *rq)
{
  if (rq_get_highest_prio(rq) == UNDEF_PRIO)
    return true;

  return false;
}

/*
 * Per-CPU area scheduling elements
 *
 * NOTE! Always disable interrupts before accessing any
 * of these elements outside of the timer IRQ context.
 */
struct percpu_sched {
  volatile clock_t sys_ticks;

  struct runqueue rrq[2];
  struct runqueue *rq_active;
  struct runqueue *rq_expired;

  struct list_node just_queued;

  int current_prio;

  int just_queued_turn;
};

struct proc;
enum cpu_type {
  BOOTSTRAP,
  SECONDARY,
};

void sched_percpu_area_init(void);
void schedulify_this_code_path(enum cpu_type);
void sched_init(void);

void sched_enqueue(struct proc *);
struct proc *sched_tick(void);  /* Avoid GCC warning */

void kthread_create(void (* func)(void));
uint64_t kthread_alloc_pid(void);



/* sys_jiffies: the world clock. */
extern unsigned long sys_jiffies;

#if CONFIG_ARCH_SIM

#define SYS_JIFFIES_UNIT        1       /* ratio */
//#define       SYS_JIFFIES_UNIT        100     /* ratio */
#define EXEC_JIFFIES_UNIT       1       /* ratio */

#define SU2EU(s)        ((uint64_t) FLOOR((s) * SYS_JIFFIES_UNIT, EXEC_JIFFIES_UNIT))
#define EU2SU(e)        ((uint64_t) FLOOR((e) * EXEC_JIFFIES_UNIT, SYS_JIFFIES_UNIT))

extern unsigned long exec_jiffies;

#endif /* CONFIG_ARCH_SIM */


static inline unsigned long get_current_jiffies(void)
{
  return sys_jiffies;
}

static inline void update_jiffies(void)
{
#if CONFIG_ARCH_SIM
  exec_jiffies = 0;
#endif
  sys_jiffies++;
}


static inline unsigned long get_current_cpu_time(void);
static inline unsigned long tsc2usec(unsigned long tsc);
static inline unsigned long tsc2nsec(unsigned long tsc);


extern int bindex[NR_CPUS];
extern int try;


extern unsigned long nr_tasks;


extern struct thread_struct *current_th[NR_CPUS];
extern struct thread_struct *prev_th[NR_CPUS];


extern unsigned long sched_time;
extern int is_in_int;

extern volatile uint32_t sched_end;

extern struct thread_struct *sleep_tq[NR_CPUS];
extern struct thread_struct *deadline_tq[NR_CPUS];

unsigned long sys_timer_interrupt(void);


int init_activate(struct thread_struct *th);

void init_rq(void);

int check_deadline_miss(void);



void init_sched(void);
void do_sched_algo(void);
void do_sched(void);

void schedule_tail(void);

void do_release(void);


int wake_up(struct thread_struct *th);
int sys_wake_up(struct thread_struct *th);
int do_wake_up(struct thread_struct *th);

int sleep_for(unsigned int length, unsigned long state);
int sleep_until(unsigned long release, unsigned long state);

int activate(struct thread_struct *th);
int sys_activate(struct thread_struct *th);
int do_activate(struct thread_struct *th);

int sched_test(int nr_tasks, int nr_cpus);

void end_periodic_job(struct thread_struct *th);
void end_aperiodic_job(struct thread_struct *th);
void do_end_job(struct thread_struct *th);
void do_timer_tick(void);


int run(unsigned long nr_threads);


enum budget_policy {
  NO_ENFORCEMENT = 1,
  TICK_ENFORCEMENT,
  PRECISE_ENFORCEMENT
};

typedef enum budget_policy budget_policy;


#define NR_RESOURCES 16


/**
 * @struct sched_info
 * @brief Scheduling information
 */
struct sched_info {

  /** Absolute release time. */
  unsigned long release;
  /** Absolute deadline. */
  unsigned long deadline;
  
  /** Period. */
  /**
   * @note Period for periodic thread,
   * min. inter-arrival time for sporadic thread,
   * or ULONG_MAX for aperiodic thread.
   */
  unsigned long period;

  /** Worst case execution time. */
  unsigned long wcet;

  /** Cpu time to begin execution. */
  unsigned long begin_cpu_time;

  /** Remaining execution time. */
  long remaining;

  /** Sum of execution time in all jobs. */
  unsigned long sum_exec_time;

  /** Flag to check if release. */
  unsigned long is_crelease;
  /** Relative deadline. */
  unsigned long relative_deadline;

  /** Time when entering semaphore. */
  unsigned long enter_sem[NR_RESOURCES];


  /** Budget policy. */
  budget_policy budget_policy;
};

typedef struct sched_info sched_info;

void do_sync(void);


#endif /* !__ASSEMBLY__ */



#endif /* __MCUBE_MCUBE_SCHED_H__ */
