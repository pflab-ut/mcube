/**
 * @file include/debug/task_debug.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_DEBUG_TASK_DEBUG_H__
#define __MCUBE_DEBUG_TASK_DEBUG_H__

#ifndef __ASSEMBLY__

#if CONFIG_OPTION_DEBUG

/**
 * @fn static inline void print_thread_in_line(void)
 * @brief print thread in line.
 */
static inline void print_thread_in_line(void)
{
  struct thread_struct *tmp;
  unsigned long cpu = get_cpu_id();

  for (tmp = current_th[cpu]->tk->top_thread; tmp; tmp = tmp->line) {
    PDEBUG("%lu-> ", tmp->id);
  }

  PDEBUG("\n");
}

/**
 * @def pdebug_thread_in_line()
 * @brief print debug for thread in line.
 */
#define pdebug_thread_in_line() do {            \
    PDEBUG_WHERE();                             \
    print_thread_in_line();                     \
  } while (0)

/**
 * @fn static inline void print_task(struct task_struct *tk)
 * @brief print task.
 *
 * @param tk Task.
 */
static inline void print_task(struct task_struct *tk)
{
  PDEBUG("tk @ %lx\n", (unsigned long) tk);
}

/**
 * @def pdebug_task(tk)
 * @brief print debug for task.
 *
 * @param tk Task.
 */
#define pdebug_task(tk) do {                    \
    PDEBUG_WHERE();                             \
    print_task(tk);                             \
  } while (0)


/**
 * @fn static inline void print_task_bythid(int thid)
 * @brief print task by thread ID.
 *
 * @param th Thread ID.
 */
static inline void print_task_bythid(int thid)
{
  struct thread_struct *th;
  unsigned long cpu = get_cpu_id();

  if (thid == 0) {
    th = &kernel_th[cpu];
  } else {
    th = &ths[thid];
  }

  if (th && th->tk) {
    pdebug_task(th->tk);
  }
}

/**
 * @def pdebug_task_by_thid(thid)
 * @brief print debug for task by thread ID.
 *
 * @param thid Thread ID.
 */
#define pdebug_task_by_thid(thid) do {          \
    PDEBUG_WHERE();                             \
    print_task_bythid(thid);                    \
  } while (0)

#else

/**
 * @def pdebug_thread_in_line()
 * @brief print debug for thread in line.
 */
#define pdebug_thread_in_line()

/**
 * @def pdebug_task(tk)
 * @brief print debug for task.
 *
 * @param tk Task.
 */
#define pdebug_task(tk)

/**
 * @def pdebug_task_by_thid(thid)
 * @brief print debug for task by thread ID.
 *
 * @param thid Thread ID.
 */
#define pdebug_task_by_thid(id)
#endif /* CONFIG_OPTION_DEBUG */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_DEBUG_TASK_DEBUG_H__ */
