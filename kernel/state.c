/**
 * @file kernel/state.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

int sleep_until(unsigned long release, unsigned long state)
{
  unsigned long cpu = get_cpu_id();

  dequeue_rq(&run_tq[cpu], current_th[cpu]);

  current_th[cpu]->sched.release = release;
  current_th[cpu]->state = state;

  sleep_tq[cpu] = enqueue_thread(sleep_tq[cpu],
                                 current_th[cpu],
                                 offsetof(struct thread_struct, sched.release));
  //  PDEBUG("sleep_tq->id = %d\n", sleep_tq->id);
  //  pdebug_tq(sleep_tq);
  return 0;
}

int activate(struct thread_struct *th)
{
  unsigned long cpu = get_cpu_id();
  //  print("activate()\n");
  th->sched.release = 0;
  th->sched.deadline = th->sched.release + th->sched.relative_deadline;
  //  PDEBUG("th->cpu = %d\n", th->cpu);

  th->state = NON_INT_SLEEP;
  //  print("enqueue th %d to %d\n", th->id, th->cpu);
  sleep_tq[cpu] = enqueue_thread(sleep_tq[cpu],
                                 th,
                                 offsetof(struct thread_struct, sched.release));
  //  print("sleep_tq[%d]->id = %llu\n", th->cpu, sleep_tq[th->cpu]->id);
  deadline_tq[cpu] = enqueue_deadline_thread(deadline_tq[cpu],
                                             th,
                                             offsetof(struct thread_struct, sched.deadline));

  //  PDEBUG("sys_activate() end\n");
  return 0;
}

int do_activate(struct thread_struct *th)
{
  int ret;

  if (!(th->state & UNADMITTED)) {
    ret = -1;
  } else {
    print("do_activate()\n");
    set_priority(th);
    add_thread_to_task(th);
    ret = activate(th);
    print("ret = %d\n", ret);
  }

  return ret;
}
