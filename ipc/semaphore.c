/**
 * @file ipc/semaphore.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_sem(struct sem_struct *sem, unsigned int nr_rsrcs, unsigned int ceil)
{
  unsigned long cpu = get_cpu_id();
  sem->counter = nr_rsrcs;
  sem->ceil = ceil;
  sem->ewq = &kernel_th[cpu];
  sem->owner = NULL;
  sem->org_prio = TH_LOWEST_PRIORITY;
}


void sem_down(struct sem_struct *sem)
{
  unsigned long cpu = get_cpu_id();

  if (--sem->counter < 0) {

    dequeue_rq(&run_tq[cpu], current_th[cpu]);
    current_th[cpu]->state = BLOCKED;

    /* allow periodic and soft aperiodic threads to be blocked:
     * note that if the longest blocking time is known,
     * periodic_recycle does not need to be called always */
    sem->ewq = enqueue_thread(sem->ewq,
                              current_th[cpu],
                              offsetof(struct thread_struct, sched.deadline));
  }

  current_th[cpu]->nr_resources++;
  sem->owner = current_th[cpu];
}

void sem_up(struct sem_struct *sem)
{
  struct thread_struct *th;
  unsigned long cpu = get_cpu_id();

  sem->counter++;
  th = sem->ewq;

  if (th != &kernel_th[cpu]) {
    sem->ewq = th->next;
    th->sched.release = th->sched.deadline;

    if (th->sched.deadline < sys_jiffies) {
      th->sched.release += CEIL(sys_jiffies - th->sched.deadline,
                                th->sched.period) * th->sched.period;
    }

    th->sched.deadline = th->sched.release + th->sched.relative_deadline;
    //    chk_nattr(th);

    th->state = NON_INT_SLEEP;
    sleep_tq[cpu] = enqueue_thread(sleep_tq[cpu],
                                   th,
                                   offsetof(struct thread_struct, sched.release));
  }

  current_th[cpu]->nr_resources--;
  sem->owner = NULL;
}
