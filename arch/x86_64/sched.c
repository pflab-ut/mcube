/**
 * @file arch/x86_64/sched.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * Uniprocessor scheduling
 *
 * Copyright (C) 2010-2011 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * For a detailed analysis of general-purpose scheduling, along with a
 * nice historical context, check the research collected in the Cute
 * 'references' tree.
 *
 * This is a multi-level feedback queue with strict fairness; its core
 * ideas are taken from CTSS [classical MLFQ dynamics], Linux O(1) [the
 * two runqueues design], FreeBSD ULE, and Staircase Deadline [fairness]
 * scheduling.
 *
 * I've tried a Solaris/SVR4-style MLFQ earlier, where it just boosted
 * threads priorities starving for a second or more. These schedulers
 * parameter table was scarily heuristic and full of voo-doo constants.
 * Optimizing these constants for certain tests kept introducing corner
 * cases for others; it was like a never-ending game of cat & mouse.
 *
 * Such voo-doos would've been partially acceptable if the designer had
 * the luxury of different machines and workloads to test against,
 * something I OBVIOUSLY lack. So in a search for more theoretically-
 * sound algorithms, I sticked with a strict-fairness design.
 *
 * FIXME: Get highest queue priority with runnable threads in O(1).
 */

#include <mcube/mcube.h>

void sched_percpu_area_init(void)
{

  /* CPU ticks counter; incremented for every tick */
  PS->sys_ticks = 0;

  /*
   * A multi-level feedback queue with strict fairness:
   *
   * A classical problem with MLFQs is starvation: a number of threads
   * can influx the high priority queues, starving the low-priority ones
   * for a considerable time (if not infinitely).
   *
   * To handle this, we add fairness to the design. If a thread finishes
   * its entire slice, it's put into the 'expired' queue  with decreased
   * priority.
   *
   * No thread in the expired queue can run till all other tasks in the
   * 'active' queue had a chance to run. Once the active queue get
   * emptied, we swap it with the expired one, and rerun the algorithm.
   * Thus, we have a starvation upper bound = N * RR_INTERVAL, where N =
   * # of runnable threads.
   *
   * If a task slept during its interval, it's popped from the runqueue.
   * After wakeup, it re-runs with same old priority up to end of the
   * _remaining_ part of its slice. [*]
   *
   * If a runqueue swap occurred during thread sleep, its slice usage is
   * reset after wake-up and it's also put in a priority higher than the
   * new active queue's default one. [+]
   *
   * Above design rationale is to maintain low-latency for interactive
   * threads while providing strict fair treatment, runtime-wise, to all
   * jobs in the system.
   *
   * [*] After wakeup, such task will immediately preempt the currently
   * running thread, or wait one RR_INTERVAL at max.
   *
   * [+] If we put such thread in the new rq with its original old prio-
   * rity, it'd be punished heavily (latency-wise) for its sleep. This
   * is especially true for low-priority tasks where the chance of a rq
   * swap during their sleep is high.
   */
  PS->rq_active = &PS->rrq[0];
  PS->rq_expired = &PS->rrq[1];
  rq_init(PS->rq_active);
  rq_init(PS->rq_expired);

  /*
   * If we just allowed new runnable threads to get added to the active
   * rq, we can starve its lower-prio threads. If we added them to the
   * expired rq instead, we will fsck up their response time up to
   * N*RR_INTERVAL ms, where N = # of runnable threads in the active rq.
   *
   * As a middle solution to this, we add them to a special list, and
   * alternate dispatching between this list and the active runqueue.
   *
   * Once the active rq get emptied and swapped, we move all the tasks
   * of that list to the new active runqueue at the head of its default
   * priority list.
   *
   * Thus, the new scheduler starvation upper bound = 2*N*RR_INTERVAL
   * ms, where N = number of runnable threads in the active rq.
   */
  list_init(&PS->just_queued);
  PS->just_queued_turn = 1;
}

/*
 * Statically allocate the booting-thread descriptor: "current"
 * must be available in all contexts, including early boot.
 */
struct process swapper;

/**
 * @fn static void __rq_add_process(struct runqueue *rq, struct process *process,
 *                                  int prio, enum enqueue_type type)
 * @brief add process.
 *
 * @param rq Runqueue.
 * @param process Process.
 * @param prio Priority.
 * @param type Type.
 */
static void __rq_add_process(struct runqueue *rq, struct process *process,
                             int prio, enum enqueue_type type)
{
  assert(VALID_PRIO(prio));

  process->enter_runqueue_ts = PS->sys_ticks;
  process->state = READY;

  switch (type) {
  case ENQ_NORMAL:
    process->runtime = 0;
    list_add_tail(&rq->head[prio], &process->pnode);
    break;

  case ENQ_RETURN:
    list_add(&rq->head[prio], &process->pnode);
    break;

  default:
    assert(false);
  }
}

/**
 * @fn static void rq_add_process(struct runqueue *rq, struct process *process,
 *                                int prio)
 * @brief add process.
 *
 * @param rq Runqueue.
 * @param process Process.
 * @param prio Priority.
 */
static void rq_add_process(struct runqueue *rq, struct process *process,
                           int prio)
{
  __rq_add_process(rq, process, prio, ENQ_NORMAL);
}

/**
 * @fn static void rq_return_process(struct runqueue *rq, struct process *process,
 *                                   int prio)
 * @brief return process.
 *
 * @param rq Runqueue.
 * @param process Process.
 * @param prio Priority.
 */
static void rq_return_process(struct runqueue *rq, struct process *process,
                              int prio)
{
  __rq_add_process(rq, process, prio, ENQ_RETURN);
}


/*
 * @@@ Scheduling proper: @@@
 */

void sched_enqueue(struct process *process)
{
  union rflags flags;

  save_local_irq(&flags);

  process->enter_runqueue_ts = PS->sys_ticks;
  process->state = READY;
  process->runtime = 0;

  list_add_tail(&PS->just_queued, &process->pnode);

  restore_local_irq(&flags);
  sched_dbg("@@ T%d added\n", process->pid);
}

/**
 * @fn static struct process *dispatch_runnable_process(int *ret_prio)
 * @brief dispatch the most suitable thread from the runqueues.
 * Alternate dispatching between the active runqueue and
 * the just_queued list.
 *
 * @param ret_prio Priority.
 * @return dispatched process if success, and NULL if all relevant queues are empty.
 */
static struct process *dispatch_runnable_process(int *ret_prio)
{
  struct process *process, *spare = NULL;
  int h_prio;

  if (PS->just_queued_turn && !list_empty(&PS->just_queued)) {
    PS->just_queued_turn = 0;

    process = list_entry(PS->just_queued.next, struct process, pnode);
    list_del(&process->pnode);
    *ret_prio = DEFAULT_PRIO;
    return process;
  }

  if (rq_empty(PS->rq_active)) {
    rq_dump(PS->rq_expired);
    SWAP(PS->rq_active, PS->rq_expired);

    /* FIXME: this can be done in O(1) */
    list_for_each_safe(&PS->just_queued, process, spare, pnode) {
      list_del(&process->pnode);
      rq_add_process(PS->rq_active, process, DEFAULT_PRIO);
    }
    rq_dump(PS->rq_active);
  }

  /* The active rq is still empty even after swap and
   * popping the just_queued threads? System is idle! */
  if (rq_empty(PS->rq_active)) {
    *ret_prio = UNDEF_PRIO;
    return NULL;
  }

  /* It's now guaranteed: a thread from the runqueues
   * will get scheduled; try 'just_queued' next time. */
  PS->just_queued_turn = 1;

  h_prio = rq_get_highest_prio(PS->rq_active);
  assert(VALID_PRIO(h_prio));
  assert(!list_empty(&PS->rq_active->head[h_prio]));

  process = list_entry(PS->rq_active->head[h_prio].next, struct process,  pnode);
  list_del(&process->pnode);
  *ret_prio = h_prio;
  return process;
}

/**
 * @fn static struct process *preempt(struct process *new_process, int new_prio)
 * @brief Preempt current thread using given new one.
 * New thread should NOT be in ANY runqueue.
 *
 * @param new_process New process.
 * @param new_prio New priority.
 * @return New process.
 */
static struct process *preempt(struct process *new_process, int new_prio)
{
  assert(new_process != get_current_process());
  assert(list_empty(&new_process->pnode));
  assert(new_process->state == READY);

  assert(VALID_PRIO(new_prio));
  PS->current_prio = new_prio;

  new_process->state = RUNNING;
  new_process->stats.dispatch_count++;
  new_process->stats.rqwait_overall += PS->sys_ticks -
                                       new_process->enter_runqueue_ts;

  sched_dbg("dispatching T%d\n", new_process->pid);
  return new_process;
}

struct process *sched_tick(void)
{
  struct process *new_process;
  int new_prio;

  //  printk("pit_ticks_count = %lu\n", pit_ticks_count);
  //  printk("PS->sys_ticks = %lu\n", PS->sys_ticks);
  //  printk("((1 * 1000 * 1000) / TICK_USEC) = %lu\n", ((1 * 1000 * 1000) / TICK_USEC));
  if (TICK_USEC <= 1 * 1000 * 1000 * 1000
      && PS->sys_ticks % ((1 * 1000 * 1000) / TICK_USEC) == 0) {
    //    printk("One second has elapsed...\n");
    //    stop_timer();
  }

  PS->sys_ticks++;
  get_current_process()->runtime++;

  assert(get_current_process()->state == RUNNING);
  assert(VALID_PRIO(PS->current_prio));

  get_current_process()->stats.runtime_overall++;
  get_current_process()->stats.prio_map[PS->current_prio]++;

  if (PS->sys_ticks % SCHED_STATS_RATE == 0) {
    print_sched_stats();
  }

  /*
   * Only switch queues after finishing the slice, not to introduce
   * fairness regression for last task standing in the active queue.
   */
  if (get_current_process()->runtime >= RR_INTERVAL) {
    get_current_process()->stats.preempt_slice_end++;

    new_process = dispatch_runnable_process(&new_prio);

    if (!new_process) {
      return get_current_process();
    }

    PS->current_prio = MAX(MIN_PRIO, PS->current_prio - 1);
    rq_add_process(PS->rq_expired, get_current_process(), PS->current_prio);
    return preempt(new_process, new_prio);
  }

  /*
   * If a higher priority task appeared, it must be an old sleeping
   * thread that has just woken up; dispatch it.
   * FIXME: what about the just_queued threads response time?
   */
  new_prio = rq_get_highest_prio(PS->rq_active);

  if (new_prio > PS->current_prio) {
    get_current_process()->stats.preempt_high_prio++;
    panic("Sleep support in not yet in the kernel; how "
          "did we reach here?");

    new_process = list_entry(PS->rq_active->head[new_prio].next,
                             struct process, pnode);
    list_del(&new_process->pnode);

    rq_return_process(PS->rq_active, get_current_process(), PS->current_prio);
    return preempt(new_process, new_prio);
  }

  /*
   * No higher priority tasks in the horizon, and our slice usage
   * is not yet complete. Peace of cake, continue running.
   */
  return get_current_process();
}

void schedulify_this_code_path(enum cpu_type type)
{
  /* 'Current' is a per-CPU structure */
  percpu_area_init(type);

  /*
   * We tell GCC to cache 'current' as much as possible since
   * it does not change for the lifetime of a thread, even if
   * that thread moved to another CPU.
   *
   * Thus GCC usually dereferences %gs:0 and cache the result
   * ('current' address) in a general-purpose register before
   * executing _any_ of the original function code.
   *
   * But in this case, getting 'current' address before
   * initializing the per-CPU area will just return a garbage
   * value (invalid/un-initialized %gs); thus the barrier.
   */
  barrier();

  process_init(get_current_process());
  get_current_process()->state = RUNNING;
  PS->current_prio = DEFAULT_PRIO;
}

void sched_init(void)
{
  uint8_t vector;

  pcb_validate_offsets();

  /*
   * Setup the timer ticks handler
   *
   * It's likely that the PIT will trigger before we enable
   * interrupts, but even if this was the case, the vector
   * will get 'latched' in the bootstrap local APIC IRR
   * register and get serviced once interrupts are enabled.
   */
  vector = TICKS_IRQ_VECTOR;
  set_idt_gate(vector, (void *) ticks_handler);
  ioapic_setup_isairq(0, vector, IRQ_BROADCAST);

  /*
   * We can program the PIT as one-shot and re-arm it in the
   * handler, or let it trigger IRQs monotonically. The arm
   * method sounds a bit risky: if a single edge trigger got
   * lost, the entire kernel will halt.
   */
  //  pit_monotonic((1000 * 1000) / HZ);
  init_pit_timer(TICK_USEC);
  start_pit_timer();
}


/*
 * @@@ Statistics: @@@
 */

spinlock_t printstats_lock;

void print_process_stats(struct process *process, int prio)
{
  __uninitialized(process);
  __uninitialized(prio);
#if 0
  uint dispatch_count;
  clock_t rqwait_overall;

  dispatch_count = process->stats.dispatch_count;
  dispatch_count = max(1u, dispatch_count);

  rqwait_overall = process->stats.rqwait_overall;

  if (process != get_current_process()) {
    rqwait_overall += PS->sys_ticks - process->enter_runqueue_ts;
  }

  printk("%lu:%d:%lu:%lu:%lu:%lu:%u:%u ", process->pid, prio,
         process->stats.runtime_overall,
         process->stats.runtime_overall / dispatch_count,
         rqwait_overall,
         rqwait_overall / dispatch_count,
         process->stats.preempt_high_prio,
         process->stats.preempt_slice_end);
#endif
}


void print_sched_stats(void)
{
#if 0
  /* NOTE: print sched stats information. */
  struct process *process;

  spin_lock(&printstats_lock);

  printk("%lu ", PS->sys_ticks);
  print_process_stats(get_current_process(), PS->current_prio);

  for (int i = MIN_PRIO; i <= MAX_PRIO; i++) {
    list_for_each(&PS->rq_active->head[i], process, pnode) {
      print_process_stats(process, i);
    }
    list_for_each(&PS->rq_expired->head[i], process, pnode) {
      print_process_stats(process, i);
    }
  }

  list_for_each(&PS->just_queued, process, pnode) {
    print_process_stats(process, DEFAULT_PRIO);
  }
  printk("\n");

  spin_unlock(&printstats_lock);
#endif
}


/*
 * @@@ Tracing: @@@
 */

void rq_dump(struct runqueue *rq)
{
  __uninitialized(rq);
#if 0
  /* FIXME: not work well */
  struct process *process;
  const char *name;

  //  name = (rq == rq_active) ? "active" : "expired";
  name = (rq == PS->rq_active) ? "active" : "expired";
  printk("Dumping %s table:\n", name);

  for (int i = MAX_PRIO; i >= MIN_PRIO; i--) {
    if (!list_empty(&rq->head[i])) {
      list_for_each(&rq->head[i], process, pnode)
      printk("%lu ", process->pid);
    }
  }

  printk("\n");
#endif
}


void process_init(struct process *process)
{
  memset(process, 0, sizeof(struct process));

  process->pid = kthread_alloc_pid();
  pcb_init(&process->pcb);
  process->state = UNADMITTED;
  list_init(&process->pnode);

#if CONFIG_OPTION_FS_EXT2
  process->working_dir = EXT2_ROOT_INODE;
#else
  process->working_dir = 0;
#endif
  unrolled_init(&process->fdtable, 32);
}
