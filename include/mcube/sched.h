/**
 * @file include/mcube/sched.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_SCHED_H__
#define __MCUBE_MCUBE_SCHED_H__

#ifndef __ASSEMBLY__


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

static inline void increment_jiffies(void)
{
  sys_jiffies++;
}


static inline unsigned long get_current_cpu_time(void);
static inline unsigned long tsc2usec(unsigned long tsc);
static inline unsigned long tsc2nsec(unsigned long tsc);


extern int bindex[NR_INTRA_KERNEL_CPUS];
extern int try;


extern unsigned long nr_tasks;


extern struct thread_struct *current_th[NR_INTRA_KERNEL_CPUS];
extern struct thread_struct *prev_th[NR_INTRA_KERNEL_CPUS];


extern unsigned long sched_time;
extern int is_in_int;

extern volatile uint32_t sched_end;

extern struct thread_struct *sleep_tq[NR_INTRA_KERNEL_CPUS];
extern struct thread_struct *deadline_tq[NR_INTRA_KERNEL_CPUS];

extern unsigned long sys_timer_interrupt(void);


extern int init_activate(struct thread_struct *th);

extern void init_rq(void);

extern int check_deadline_miss(void);



extern void init_sched(void);
extern void do_sched_algo(void);
extern void do_sched(void);

extern void do_release(void);


extern int wake_up(struct thread_struct *th);
extern int sys_wake_up(struct thread_struct *th);
extern int do_wake_up(struct thread_struct *th);

extern int sleep_for(unsigned int length, unsigned long state);
extern int sleep_until(unsigned long release, unsigned long state);

extern int activate(struct thread_struct *th);
extern int sys_activate(struct thread_struct *th);
extern int do_activate(struct thread_struct *th);

extern int sched_test(int nr_tasks, int nr_cpus);

extern void end_periodic_job(struct thread_struct *th);
extern void end_aperiodic_job(struct thread_struct *th);
extern void do_end_job(struct thread_struct *th);
extern void do_timer_tick(void);


extern int run(unsigned long nr_threads);


enum budget_policy {
	NO_ENFORCEMENT = 1,
	TICK_ENFORCEMENT,
	PRECISE_ENFORCEMENT
};

typedef enum budget_policy budget_policy;


#define NR_RESOURCES 16


/**
 * @brief Scheduling information
 *
 * The sched_info structure has scheduling information.
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

	/**
	 * Worst case execution time.
	 */
	unsigned long wcet;

	/** Cpu time to begin execution. */
	unsigned long begin_cpu_time;

	/** Remaining execution time. */
	long remaining;

	/** Executing cost in current part. */
	long exec_cost;
	/** Sum of execution time in all jobs. */
	unsigned long sum_exec_time;

	/** Flag to check if release. */
	unsigned long is_crelease;
	/** Relative deadline. */
	unsigned long relative_deadline;

	/** Time when entering semaphore. */
	unsigned long enter_sem[NR_RESOURCES];

#if !CONFIG_SYNC_NONE
	/** Length of accessing semaphore. */
	unsigned int sem[NR_RESOURCES];
	/** Maximum blocking time. */
	unsigned int block;
	/** Preemption level. */
	unsigned int prlevel;
#endif


	/** Budget policy. */
	budget_policy budget_policy;
};

typedef struct sched_info sched_info;

extern void do_sync(void);


#endif	/* !__ASSEMBLY__ */



#endif /* __MCUBE_MCUBE_SCHED_H__ */
