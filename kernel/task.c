/**
 * @file kernel/task.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

/**
 * @var init_task
 * @brief Initialized task.
 */
static struct task_struct init_task = INIT_TASK;

struct task_struct *current_task = &(init_task);
struct task_struct *tasks[NR_TASKS] = {&init_task, };


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

/**
 * @fn static inline int alloc_task_id(void)
 * @brief allocate task ID.
 *
 * @return Task ID.
 */
static inline int alloc_task_id(void)
{
  static unsigned int task_id = INIT_TASK_ID;
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

  tk->top_thread = do_create_thread(NULL, NULL, attr);
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
