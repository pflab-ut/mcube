/**
 * @file include/mcube/metric.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_METRIC_H__
#define	__MCUBE_MCUBE_METRIC_H__

#ifndef __ASSEMBLY__


void print_overhead(void);
void print_metric(int cutil);
void print_option_usage(void);
void init_metric(void);
void init_overhead(void);


/**
 * @struct overhead
 * @brief Overhead measurement
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


/**
 * @struct performance_metrics
 * @brief Performance metrics
 */
struct performance_metrics {
	/** Number of successes. */
	uint64_t successes;
	/** Average number of context switches per job in task. */
	double cxs_per_job_in_task;
	/** Average number of preemptions per job in task. */
	double preemption_per_job_in_task;
	/** Average number of migrations per job in task. */
	double migration_per_job_in_task;
};

typedef struct performance_metrics performance_metrics;

/** 
 * @brief File error code.
 */
enum file_error {
  /* file open */
  SUCCESS_ERROR = 16,
  CXS_ERROR,
  PREEMPTION_ERROR,
  MIGRATION_ERROR,
};

typedef enum file_error file_error;


extern performance_metrics perf_metrics;

extern uint64_t context_switches[NR_THREADS];
extern uint64_t preemptions[NR_THREADS];
extern uint64_t migrations[NR_THREADS];

extern uint64_t ths_release[NR_THREADS];

void print_ovhd(struct overhead ovhd[NR_INTRA_KERNEL_CPUS][NR_THREADS]);

void save_pre_time(overhead ovhd[NR_INTRA_KERNEL_CPUS][NR_THREADS]);
void save_post_time(overhead ovhd[NR_INTRA_KERNEL_CPUS][NR_THREADS]);


#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_METRIC_H__ */
