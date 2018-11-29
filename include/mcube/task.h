/**
 * @file include/mcube/task.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_TASK_H__
#define	__MCUBE_MCUBE_TASK_H__

/*
 * Task means process, which separates the address space each other.
 * In contrast, all threads in each task shares the address space.
 */

/*
 * OFFSETS -- these are softwired. --
 */
#define	TK_MMU_CNTXT_OFFSET	0x0
#define	TK_SBRK_LIMIT_OFFSET	0x20

#define OFFSET_OF_CPU_CONTEXT 0


#define NR_TASKS 0x10


#ifndef __ASSEMBLY__




typedef struct tlb_info tlb_info;

/**
 * @brief Task information
 *
 * The task_struct structure has task information.
 */
struct task_struct {
	/** Task ID. */
	unsigned int id;
	/** Pointer to top thread. */
	struct thread_struct *top_thread;

  /** Task state. */
  long state;
  /** Time slice counter. */
  long counter;
  /** Priority. */
  long priority;
  /** Preempt count. */
  long preempt_count;

};

typedef struct task_struct task_struct;

extern struct task_struct *tasks[NR_TASKS];
extern struct task_struct *current_task;

static inline void preempt_disable(void)
{
  current_task->preempt_count++;
}

static inline void preempt_enable(void)
{
  current_task->preempt_count--;
}


#define	TASK_ID 1

#define INIT_TASK {                                 \
    .id		= TASK_ID,                                \
    .top_thread	= &kernel_th[0],                    \
    .state		= 0,                                  \
    .counter	= 0,                                  \
    .priority	= 1,                                  \
    .preempt_count = 0                              \
  }


/**
 * @brief Request information to external memory controller
 *
 * The emc_req_info structure has request information to external memory controller.
 */
struct emc_req_info {
	/** Semaphore to external memory controller. */
	struct sem_struct sem;
	/** Pointer to thread sending a reqeust to external memory controller. */
	struct thread_struct *th;
};

typedef struct emc_req_info emc_req_info;

extern struct emc_req_info emc_req;
#define	EMC_REQ_INIT	{.sem	= INIT_COUNT_SEM(0), .th = NULL}


void add_thread_to_task(struct thread_struct *);
void delete_thread_from_task(struct thread_struct *);
int exit_task(void);

int move_to_user_level(void);

struct task_struct *do_create_task(struct th_attr *attr);

int copy_process(unsigned long func, unsigned long arg);

/**
 * @fn   copy_arch_process
 * @brief      Copy architecture dependent values in process
 * @param[out] p Pointer to task.
 * @param[in]  func Pointer to function.
 * @param[in]  argument
 */
void copy_arch_process(struct task_struct *p, unsigned long func, unsigned long arg);
void ret_from_fork(void);



#endif	/* !__ASSEMBLY__ */
#endif	/* __MCUBE_MCUBE_TASK_H__ */
