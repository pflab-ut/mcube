/**
 * @file include/x86_64/sched.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_SCHED_H__
#define __MCUBE_X86_64_SCHED_H__

#ifndef __ASSEMBLY__

/*
 * Scheduler Configuration
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */

/**
 * @def SCHED_STATS_RATE
 * @brief COM1: print stats (if enabled) each @c SCHED_STATS_RATE ticks.
 */
#define SCHED_STATS_RATE HZ

struct runqueue;

/**
 * @def sched_dbg(fmt, ...)
 * @brief print debug for scheduling.
 *
 * @param fmt Format.
 */
#define  sched_dbg(fmt, ...) PDEBUG(fmt, ##__VA_ARGS__)

/**
 * @fn void rq_dump(struct runqueue *rq)
 * @brief dump runqueue.
 *
 * @param rq Runqueue.
 */
void rq_dump(struct runqueue *rq);

/**
 * @fn void print_process_stats(struct process *process, int prio)
 * @brief print process statistics.
 *
 * @param process Process.
 * @param prio Priority.
 */
void print_process_stats(struct process *process, int prio);

/**
 * @fn void print_sched_stats(void)
 * @brief print scheduling statistics.
 */
void print_sched_stats(void);

/**
 * @fn void ticks_handler(void)
 * @brief Ticks handler.
 */
void ticks_handler(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_SCHED_H__ */
