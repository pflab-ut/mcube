/**
 * @file include/x86/sched.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_SCHED_H__
#define __MCUBE_X86_SCHED_H__

#ifndef __ASSEMBLY__

/*
 * Scheduler Configuration
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */

/*
 * COM1: print stats (if enabled) each @SCHED_STATS_RATE ticks.
 */
#define SCHED_STATS_RATE HZ

struct runqueue;

#define  sched_dbg(fmt, ...)    print_uart(fmt, ##__VA_ARGS__)
void rq_dump(struct runqueue *rq);

void print_proc_stats(struct proc *proc, int prio);
void print_sched_stats(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_SCHED_H__ */
