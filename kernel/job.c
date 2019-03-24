/**
 * @file kernel/job.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


/* periodic job */
void end_periodic_job(struct thread_struct *th)
{
  unsigned long abs_release;
  unsigned long cpu = get_cpu_id();
#if 0
  PDEBUG("th = %x\n", th);
  PDEBUG("sys_jiffies = %lu\n", sys_jiffies);
  PDEBUG("->sched.deadline = %lu\n", th->sched.deadline);
#endif
  //  print("end_periodic_job(): current_time() = %llu\n", current_time());
  //  th->sched.earliness = current_time() + 1 - th->sched.deadline;
  //  assert(SU2EU(sys_jiffies) < current_th->sched.deadline);

  deadline_tq[cpu] = dequeue_deadline_thread(deadline_tq[cpu], th);


  abs_release = th->sched.release + th->sched.period;
  //  PDEBUG("abs_release = %llu relative_deadline = %llu\n", abs_release, current_th->sched.relative_deadline);
  th->sched.deadline = abs_release + th->sched.relative_deadline;

  deadline_tq[cpu] = enqueue_deadline_thread(deadline_tq[cpu],
                                             th,
                                             offsetof(struct thread_struct, sched.deadline));


  sleep_until(abs_release, NON_INT_SLEEP);

}


/* aperiodic job */
void end_aperiodic_job(struct thread_struct *th)
{
  unsigned long cpu = get_cpu_id();

  if (th->type & SOFT_REAL_TIME) {
    assert(sys_jiffies <= th->sched.deadline);
  }

  dequeue_rq(&run_tq[cpu], th);
  th->state = UNADMITTED;
  delete_thread_from_task(th);
}

void do_end_job(struct thread_struct *th)
{
  if (th->type & PERIODIC_TH) {
    end_periodic_job(th);
  } else {
    end_aperiodic_job(th);
  }
}

