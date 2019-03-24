/**
 * @file kernel/queue.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

/* task queues */
struct thread_struct *sleep_tq[NR_CPUS];
struct thread_struct *deadline_tq[NR_CPUS];
struct rt_runqueue run_tq[NR_CPUS];


void enqueue_rq_head(struct rt_runqueue *rq, struct thread_struct *th)
{
  enqueue_rq_queue(rq, th);
  th->state = READY;
}


void enqueue_rq(struct rt_runqueue *rq, struct thread_struct *th)
{
  enqueue_rq_queue(rq, th);
  th->state = READY;
}


struct thread_struct *compare_thread(struct thread_struct *th,
                                     struct thread_struct *n,
                                     unsigned long offset)
{
  //  PDEBUG("compare_thread()\n");
  uint64_t th_val, n_val;
  th_val = *(uint64_t *)((unsigned long)(th) + offset);
  n_val = *(uint64_t *)((unsigned long)(n) + offset);

  //    PDEBUG("first: th_val = %llu, n_val = %lu\n", th_val, n_val);
  if (th_val < n_val) {
    return th;
  } else if (th_val == n_val) {
    if (thread_tie_break(th, n)) {
      return th;
    }
  }

  return NULL;
}

/* enqueue * offset is used for sorting th */
struct thread_struct *enqueue_thread(struct thread_struct *head,
                                     struct thread_struct *th,
                                     unsigned long offset)
{
  unsigned long cpu = get_cpu_id();
  /* p is necessary because n could be the idle thread */
  struct thread_struct *p, *n;
  //  PDEBUG("enqueue_thread(): head = %x\n", head);

  /* first search the place */
  for (p = NULL, n = head; n; n = n->next) {
    //    PDEBUG("n = %x\n", n);
    if (compare_thread(th, n, offset)) {
      break;
    }

    p = n;
  }

  if (th) {
    th->prev = p;
    th->next = n;

    if (((th->state & (READY | RUNNING)) || n != &kernel_th[cpu]) && n) {
      n->prev = th;
    }
  }

  if (p) {
    p->next = th;
  } else {
    head = th;
  }

  return head;
}

struct thread_struct *enqueue_deadline_thread(struct thread_struct *head,
                                              struct thread_struct *th,
                                              unsigned long offset)
{
  unsigned long cpu = get_cpu_id();
  /* p is necessary because n could be the idle thread */
  struct thread_struct *p, *n;
  //  PDEBUG("enqueue_deadline_thread(): head = %x th->id = %llu\n", head, th->id);


  /* first search the place */
  for (p = NULL, n = head; n; n = n->dnext) {
    //    PDEBUG("n = %x n->id = %llu\n", n, n->id);
    if (compare_thread(th, n, offset)) {
      break;
    }

    p = n;
  }

  if (th) {
    th->dprev = p;
    th->dnext = n;

    //  PDEBUG("p = %x n = %x\n", p, n);
    if (((th->state & (READY | RUNNING)) || n != &kernel_th[cpu]) && n) {
      n->dprev = th;
    }
  }

  if (p) {
    p->dnext = th;
  } else {
    head = th;
  }

  return head;
}

void dequeue_rq(struct rt_runqueue *rq, struct thread_struct *th)
{
  dequeue_rq_queue(rq, th);
}


struct thread_struct *dequeue_thread(struct thread_struct *head,
                                     struct thread_struct *th)
{
  unsigned long cpu = get_cpu_id();

  if (head == th) {
    head = head->next;
  } else {
    th->prev->next = th->next;
  }

  if (th->state & (READY | RUNNING) || th->next != &kernel_th[cpu]) {
    th->next->prev = th->prev;
  }

  return head;
}

struct thread_struct *dequeue_deadline_thread(struct thread_struct *head,
                                              struct thread_struct *th)
{
  unsigned long cpu = get_cpu_id();

  if (head == th) {
    head = head->dnext;
  } else {
    th->dprev->dnext = th->dnext;
  }

  if (th->state & (READY | RUNNING) || th->dnext != &kernel_th[cpu]) {
    th->dnext->dprev = th->dprev;
  }

  return head;
}

