/**
 * @file kernel/thread.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

struct thread_struct ths[NR_THREADS];

/* idle should not be in the hash, becuase its const */
static uint32_t th_id = FIRST_DYNAMIC_THREAD_ID;

unsigned char PAGE_ALIGNMENT kernel_stack[NR_CPUS][KERNEL_STACK_SIZE];
unsigned char PAGE_ALIGNMENT user_stack[NR_THREADS][USER_STACK_SIZE];

int get_tq_util(struct thread_struct *head, unsigned long cpu)
{
  struct thread_struct *p;
  int sum_util = 0;

  //  print("get_tq_util()\n");
  for (p = head; p != &kernel_th[cpu]; p = p->next) {
    //    print("p->id = %llu\n", p->id);
    sum_util += p->util;
  }

  return sum_util;
}

int alloc_thread_id(void)
{
  return th_id++;
}


void thread_main(__unused struct thread_struct *th)
{
}


struct thread_struct *do_create_thread(void *(*func)(void *),
                                       void *arg,
                                       struct th_attr *attr)
{
  unsigned long id;
  unsigned long index;
  id = alloc_thread_id();
  index = id - 1;

  if (index < NR_THREADS) {
    ths[index].id = id;
    ths[index].state = UNADMITTED;
    ths[index].tk = current_task;

    if (attr) {
      ths[index].type = attr->type;
    } else {
      ths[index].type = APERIODIC_TH;
    }

    ths[index].thflags = 0;
    ths[index].arg = arg;

    ths[index].sched.release = 0;

    if (attr) {
      ths[index].sched.period = attr->period;
      ths[index].sched.relative_deadline = attr->relative_deadline;
      ths[index].sched.deadline = attr->relative_deadline;
      ths[index].sched.wcet = attr->wcet;
    } else {
      ths[index].sched.period = ULONG_MAX;
      ths[index].sched.relative_deadline = ULONG_MAX;
      ths[index].sched.deadline = ULONG_MAX;
      ths[index].sched.wcet = ULONG_MAX;
    }

    ths[index].stack_top = USER_THREAD_STACK_ADDR(ths[index].id);
    ths[index].run_func = run_user_thread;
    ths[index].run_user_func = func;

    return &ths[index];
  } else {
    print("do_create_thread(): Error: %lu exceed NR_THREADS %d\n", id, NR_THREADS);
    return NULL;
  }
}
