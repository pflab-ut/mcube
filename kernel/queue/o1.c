/**
 * @file kernel/queue/o1.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


/* for global scheduling and synchronization */
void enqueue_rq_queue_head(struct runqueue *rq, struct thread_struct *th)
{
	/* enqueue the head of runqueue */
	//	PDEBUG("th->priority = %llu\n", th->priority);
	set_bit(rq->bitmap, th->priority);
	rq->array[th->priority].next->prev = th;
	th->next = rq->array[th->priority].next;
	th->prev = &rq->array[th->priority];
	rq->array[th->priority].next = th;
}


void enqueue_rq_queue(struct runqueue *rq, struct thread_struct *th)
{
	//	PDEBUG("th = 0x%x th->priority = %llu\n", th, th->priority);
	//	PDEBUG("rq->array[%llu].prev = %x\n", th->priority, rq->array[th->priority].prev);
	set_bit(rq->bitmap, th->priority);
	rq->array[th->priority].prev->next = th;
	th->prev = rq->array[th->priority].prev;
	th->next = &rq->array[th->priority];
	rq->array[th->priority].prev = th;
}



void dequeue_rq_queue(struct runqueue *rq, struct thread_struct *th)
{
  pdebug_array(rq->array);
  if (rq->array[th->priority].next == th) {
    rq->array[th->priority].next = th->next;
  } else {
    th->prev->next = th->next;
  }

  th->next->prev = th->prev;
	if (rq->array[th->priority].next == &rq->array[th->priority]) {
    clear_bit(rq->bitmap, th->priority);
  }
}

struct thread_struct *pick_next_task(void)
{
  unsigned long cpu = get_cpu_id();
	struct thread_struct *th = NULL;
  bindex[cpu] = find_first_bit(run_tq[cpu].bitmap, NR_PRIORITY_BITMAPS);
	//	PDEBUG("bindex[%d] = %d\n", cpu, bindex);
	if (bindex[cpu] != NR_PRIORITIES) {
    th = run_tq[cpu].array[bindex[cpu]].next;
		//		th->state = RUNNING;
  }
	return th;
}



void init_rq(void)
{
	int i, j;
  for (i = 0; i < NR_INTRA_KERNEL_CPUS; i++) {
    run_tq[i].util = 0;
    run_tq[i].nr_threads = 0;
    for (j = 0; j < NR_PRIORITY_BITMAPS; j++) {
      run_tq[i].bitmap[j] = 0;
    }
    for (j = 0; j < NR_PRIORITIES; j++) {
      run_tq[i].array[j] = INIT_IDLE_THREAD(i);
      run_tq[i].array[j].prev = run_tq[i].array[j].next = &run_tq[i].array[j];
    }
  }
}
