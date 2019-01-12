/**
 * @file kernel/task.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

static struct task_struct init_task = INIT_TASK;
struct task_struct *current_task = &(init_task);
struct task_struct *tasks[NR_TASKS] = {&init_task, };

int copy_process(unsigned long func, unsigned long arg)
{
  struct task_struct *p;
  int pid;
  
  preempt_disable();
#if 0
  if (!(p = (struct task_struct *) get_free_page())) {
    print("Error: cannot create task\n");
    return 1;
  }
  p->priority = current_task->priority;
  p->state = RUNNING;
  p->counter = p->priority;
  p->preempt_count = 1; //disable preemtion until schedule_tail

  copy_arch_process(p, func, arg);
  pid = nr_tasks++;
  tasks[pid] = p;
#endif
  preempt_enable();
  return 0;
}

/* add newly created to and release old thread from the task */
void add_thread_to_task(struct thread_struct *th)
{
  if (th != th->tk->top_thread) {
    th->line = th->tk->top_thread->line;
    th->tk->top_thread->line = th;
  }
}

void delete_thread_from_task(struct thread_struct *th)
{
  struct thread_struct *p;

  if ((p = th->tk->top_thread) != th) {
    while (p->line != th) {
      p = p->line;
    }
    p->line = th->line;
  }
  /* the top_thread cannot be released:
   * otherwise we don't know which thread and the task reside in the
   * same memory block. */
}


static inline int alloc_task_id(void)
{
  static unsigned int task_id = TASK_ID;
  return task_id++;
}

struct task_struct *do_create_task(struct th_attr *attr)
{
  struct task_struct *tk;
  static uint32_t task_index = 0;

  if (task_index >= NR_TASKS) {
    print("Error: cannot create task\n");
    return NULL;
  }
  tk = tasks[task_index++];
  tk->id = alloc_task_id();  

  tk->top_thread = do_create_thread(NULL,
                                    NULL,
                                    attr);
  tk->top_thread->line = NULL;

  return tk;

}

int exit_task(void)
{
  unsigned long cpu = get_cpu_id();
  dequeue_rq(&run_tq[cpu], current_th[cpu]);
  current_th[cpu]->state = UNADMITTED;

  return 0;
}
