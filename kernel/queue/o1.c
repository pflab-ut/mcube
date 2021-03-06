/**
 * @file kernel/queue/o1.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


/*
 * O(1) scheduler inspired by Linux.
 * https://en.wikipedia.org/wiki/O(1)_scheduler
 * http://lkml.iu.edu/hypermail/linux/kernel/0705.1/3017.html
 */


void enqueue_rq_queue(struct rt_runqueue *rq, struct thread_struct *th)
{
  //  PDEBUG("th = 0x%x th->priority = %llu\n", th, th->priority);
  //  PDEBUG("rq->array[%llu].prev = %x\n", th->priority, rq->array[th->priority].prev);
  set_bit32(rq->bitmap, th->priority);
  rq->array[th->priority].prev->next = th;
  th->prev = rq->array[th->priority].prev;
  th->next = &rq->array[th->priority];
  rq->array[th->priority].prev = th;
}



void dequeue_rq_queue(struct rt_runqueue *rq, struct thread_struct *th)
{
  pdebug_array(rq->array);

  if (rq->array[th->priority].next == th) {
    rq->array[th->priority].next = th->next;
  } else {
    th->prev->next = th->next;
  }

  th->next->prev = th->prev;

  if (rq->array[th->priority].next == &rq->array[th->priority]) {
    clear_bit32(rq->bitmap, th->priority);
  }
}

struct thread_struct *pick_next_thread(void)
{
  unsigned long cpu = get_cpu_id();
  struct thread_struct *th = (struct thread_struct *) NULL;
  bindex[cpu] = find_first_bit32(run_tq[cpu].bitmap, NR_PRIORITY_BITMAPS);

  //  PDEBUG("bindex[%d] = %d\n", cpu, bindex);
  if (bindex[cpu] != NR_PRIORITIES) {
    th = run_tq[cpu].array[bindex[cpu]].next;
    //    th->state = RUNNING;
  }

  return th;
}



void init_rq(void)
{
  int i, j;

  for (i = 0; i < NR_CPUS; i++) {
    run_tq[i].util = 0;
    run_tq[i].nr_threads = 0;

    for (j = 0; j < NR_PRIORITY_BITMAPS; j++) {
      run_tq[i].bitmap[j] = 0;
    }

    for (j = 0; j < NR_PRIORITIES; j++) {
      //      run_tq[i].array[j] = (struct thread_struct) INIT_KERNEL_THREAD(i);
      init_kernel_thread(&run_tq[i].array[j], i);
      run_tq[i].array[j].prev = run_tq[i].array[j].next = &run_tq[i].array[j];
    }
  }
}
