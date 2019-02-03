/**
 * @file include/x86/conf_sched.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_CONF_SCHED_H__
#define __MCUBE_X86_CONF_SCHED_H__

#ifndef __ASSEMBLY__

/*
 * Scheduler Configuration
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */

/*
 * COM1: log the main scheduler operations?
 */
#define SCHED_TRACE 0

/*
 * COM1: print scheduling statistics?
 */
#define SCHED_STATS 0

/*
 * COM1: print stats (if enabled) each @SCHED_STATS_RATE ticks.
 */
#define SCHED_STATS_RATE HZ

struct runqueue;

#if SCHED_TRACE
#define  sched_dbg(fmt, ...)    print_uart(fmt, ##__VA_ARGS__)
static void rq_dump(struct runqueue *rq);
#else
#define sched_dbg(fmt, ...)    { }
static void __unused rq_dump(struct runqueue __unused *rq) { }
#endif /* SCHED_TRACE */

#if SCHED_STATS
static void print_sched_stats(void);
#else
static void __unused print_sched_stats(void) { }
#endif /* SCHED_STATS */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_CONF_SCHED_H__ */
