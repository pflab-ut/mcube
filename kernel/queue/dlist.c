/**
 * @file kernel/queue/dlist.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


static void enqueue_rq_dlist(struct runqueue *rq,
														 struct thread_struct *th,
														 unsigned long offset)
{
	//	PDEBUG("enqueue_rq_dlist()\n");
	/* p is necessary because n could be the idle thread */
	struct thread_struct *p, *n;
  unsigned long cpu = get_cpu_id();
  
	/* first search the place */
	for (p = &rq->array[0], n = rq->array[0].next; n != &rq->array[0]; n = n->next) {
    if (compare_thread(th, n, offset)) {
      break;
		}
		p = n;
	}

	/* enqueue */
	th->prev = p;
	th->next = n;

	/* head == ready_tq */
	if (((th->state & (READY | RUNNING)) || n != &idle_th[cpu]) && n) {
		n->prev = th;
	}

	p->next = th;

}


/* same */
void enqueue_rq_queue_head(struct runqueue *rq, struct thread_struct *th)
{
	set_bit(rq->bitmap, 0);
	enqueue_rq_dlist(rq,
									 th,
									 offsetof(struct thread_struct, priority));
}


void enqueue_rq_queue(struct runqueue *rq, struct thread_struct *th)
{
	set_bit(rq->bitmap, 0);
	enqueue_rq_dlist(rq,
									 th,
									 offsetof(struct thread_struct, priority));
}


void dequeue_rq_queue(struct runqueue *rq, struct thread_struct *th)
{
  //  pdebug_array(run_tq[0].array);
  if (rq->array[0].next == th) {
    rq->array[0].next = th->next;
  } else {
    th->prev->next = th->next;
  }

  th->next->prev = th->prev;
  if (rq->array[0].next == &rq->array[0]) {
    clear_bit(rq->bitmap, 0);
  }   
}

struct thread_struct *pick_next_task(void)
{
	struct thread_struct *th = NULL;
  unsigned long cpu = get_cpu_id();
	/* use only the highest priority */
	if (run_tq[cpu].array[0].next != &run_tq[cpu].array[0]) {
		//    th = run_tq[cpu].array[bindex].next;
    th = run_tq[cpu].array[0].next;
  }
	return th;
}


void init_rq(void)
{
	int i, j;
  /* NOTE: other members in run_tq are initialized at compile time.  */
  for (i = 0; i < NR_INTRA_KERNEL_CPUS; i++) {
    for (j = 0; j < NR_PRIORITIES; j++) {
      run_tq[i].array[j].prev = run_tq[i].array[j].next = &run_tq[i].array[j];
    }
  }
}
