/**
 * @file ipc/semaphore.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

unsigned int nr_resources;

/* for real-time synchronization protocol */
struct sem_struct sync_sem[NR_RESOURCES];


void init_sem(struct sem_struct *sem, unsigned int nr_rsrcs, unsigned int ceil)
{
  int cpu = get_cpu_id();
  assert(nr_resources <= 1);
  sem->counter = nr_rsrcs;
  sem->ceil = ceil;
  sem->ewq = &kernel_th[cpu];
  sem->owner = NULL;
  sem->org_prio = TH_LOWEST_PRIORITY;
}


void sem_down(struct sem_struct *sem)
{
  int cpu = get_cpu_id();
  lock_scheduler();
  if (--sem->counter < 0) {

    dequeue_rq(&run_tq[cpu], current_th[cpu]);
    current_th[cpu]->state = BLOCKED;

    /* allow periodic and soft aperiodic threads to be blocked:
     * note that if the longest blocking time is known,
     * periodic_recycle does not need to be called always */
    sem->ewq = enqueue_thread(sem->ewq,
                              current_th[cpu],
                              offsetof(struct thread_struct, sched.deadline));
    unlock_scheduler();
    lock_scheduler();
  }
  current_th[cpu]->nr_resources++;
  sem->owner = current_th[cpu];
  unlock_scheduler();
}

void sem_up(struct sem_struct *sem)
{
  struct thread_struct *th;
  int cpu = get_cpu_id();
  lock_scheduler();

  sem->counter++;
  th = sem->ewq;
  if (th != &kernel_th[cpu]) {
    sem->ewq = th->next;
    th->sched.release = th->sched.deadline;
    if (th->sched.deadline < sys_jiffies) {
      th->sched.release += CEIL(sys_jiffies - th->sched.deadline, th->sched.period) * th->sched.period;
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
  unlock_scheduler();
}
