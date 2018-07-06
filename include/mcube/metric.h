/**
 * @file include/mcube/metric.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_METRIC_H__
#define	__MCUBE_MCUBE_METRIC_H__

#ifndef __ASSEMBLY__

struct workload;

extern void save_earliness(struct thread_struct *th);
extern void save_rrj(struct thread_struct *th);
extern void save_rfj(struct thread_struct *th);
extern void save_rta(struct thread_struct *th);
extern void print_overhead(void);
extern void save_metric(struct workload *w);
extern void print_metric(int cutil);
extern void print_option_usage(void);
extern void init_metric(void);
extern void init_overhead(void);


/**
 * @brief Overhead measurement
 *
 * The overhead structure measures overhead.
 */
struct overhead {
	/** Time stamp counter. */
	uint64_t tsc;
	/** Sum overhead time. */
	uint64_t sum_time;
	/** Number of tries. */
	uint64_t nr_tries;
	/** Minimum overhead time. */
	uint64_t min;
	/** Maximum overhead time. */
	uint64_t max;
};

typedef struct overhead overhead;

extern overhead cxs_ovhd[NR_INTRA_KERNEL_CPUS][NR_THREADS];
extern overhead release_ovhd[NR_INTRA_KERNEL_CPUS][NR_THREADS];
extern overhead sched_ovhd[NR_INTRA_KERNEL_CPUS][NR_THREADS];
extern overhead tick_ovhd[NR_INTRA_KERNEL_CPUS][NR_THREADS];
extern overhead end_mandatory_ovhd[NR_INTRA_KERNEL_CPUS][NR_THREADS];
extern overhead end_optional_ovhd[NR_INTRA_KERNEL_CPUS][NR_THREADS];
extern overhead terminate_optional_ovhd[NR_INTRA_KERNEL_CPUS][NR_THREADS];
extern overhead tmu_adjust_ovhd[NR_INTRA_KERNEL_CPUS][NR_THREADS];


/**
 * @brief Performance metrics
 *
 * The performance_metrics structure has scheduling results.
 */
struct performance_metrics {
	/** Number of successes. */
	uint64_t successes;
	/** Average reward ratio per job in task. */
	double reward_per_job_in_task;
	/** Average number of context switches per job in task. */
	double cxs_per_job_in_task;
	/** Average number of preemptions per job in task. */
	double preemption_per_job_in_task;
	/** Average relative relase jitter per job in task. */
	double rrj_per_job_in_task;
	/** Average relative finish jitter per job in task. */
	double rfj_per_job_in_task;
	/** Average number of migrations per job in task. */
	double migration_per_job_in_task;
	/** Average response time per job in task. */
	double rt_per_job_in_task;
};

typedef struct performance_metrics performance_metrics;

/** 
 * @brief File error code.
 */
enum file_error {
  /* file open */
  SUCCESS_ERROR = 16,
  REWARD_ERROR,
  CXS_ERROR,
  PREEMPTION_ERROR,
  RRJ_ERROR,
  RFJ_ERROR,
  MIGRATION_ERROR,
  RT_ERROR,
};

typedef enum file_error file_error;

/**
 * @brief Performance server
 *
 * The performance_server structure has scheduling results in RUN-based algorithms.
 */
struct performance_server {
	/** Number of servers. */
	uint64_t sum_srvs;
	/** Maximum number of servers. */
	uint64_t max_srvs;
	/** Upper bound of number of servers. */
	uint64_t sum_srvs_ub;
	/** Sum of server levels. */
	uint64_t sum_srv_levels;
	/** Maximum server levels. */
	uint64_t max_srv_levels;
	/** Sum of upper bound of server levels. */
	uint64_t sum_srv_levels_ub;
	/** Number of task sets per metric. */
	uint64_t count;
	/** Maximum number of preemptions. */
	uint64_t max_preemption;
	/** Maximum number of migrations. */
	uint64_t max_migration;
};

typedef struct performance_server performance_server;

extern performance_metrics perf_metrics;
extern performance_server perf_th[NR_THREADS];
extern performance_server perf_srv[NR_THREADS];

extern uint64_t onum[NR_THREADS];
extern uint64_t oden[NR_THREADS];

extern uint64_t ospent[NR_THREADS];
extern uint64_t reward[NR_THREADS];

extern uint64_t reward_sum;
extern uint64_t context_switches[NR_THREADS];
extern uint64_t preemptions[NR_THREADS];
extern uint64_t migrations[NR_THREADS];
extern uint64_t rrj[NR_THREADS];
extern uint64_t rfj[NR_THREADS];
extern uint64_t rt_diff[NR_THREADS];
extern uint64_t ft_diff[NR_THREADS];
extern double rta_sum[NR_THREADS];

extern uint64_t ths_release[NR_THREADS];

extern uint64_t start_processor_time;

extern void print_ovhd(struct overhead ovhd[NR_INTRA_KERNEL_CPUS][NR_THREADS]);

extern void save_pre_time(overhead ovhd[NR_INTRA_KERNEL_CPUS][NR_THREADS]);
extern void save_post_time(overhead ovhd[NR_INTRA_KERNEL_CPUS][NR_THREADS]);


#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_METRIC_H__ */
