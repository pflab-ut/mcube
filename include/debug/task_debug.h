/**
 * @file include/debug/task_debug.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_DEBUG_TASK_DEBUG_H__
#define __MCUBE_DEBUG_TASK_DEBUG_H__

/* Never include this file directly. Include <debug/debug.h> instead. */
#ifndef __ASSEMBLY__

#if CONFIG_OPTION_DEBUG

static inline void print_thread_in_line(void)
{
  struct thread_struct *tmp;
  unsigned long cpu = get_cpu_id();

  for (tmp = current_th[cpu]->tk->top_thread; tmp; tmp = tmp->line) {
    PDEBUG("%lu-> ", tmp->id);
  }

  PDEBUG("\n");
}

#define pdebug_thread_in_line() do {            \
    PDEBUG_WHERE();                             \
  } while (0)

static inline void print_task(struct task_struct *tk)
{
  PDEBUG("tk @ %lx\n", (unsigned long) tk);
}


#define pdebug_task(tk) do {                    \
    PDEBUG_WHERE();                             \
    print_task(tk);                             \
  } while (0)

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


#define pdebug_task_bythid(thid) do {           \
    PDEBUG_WHERE();                             \
    print_task_bythid(thid);                    \
  } while (0)

#else
#define pdebug_thread_in_line()
#define pdebug_task(tk)
#define pdebug_task_bythid(id)
#endif /* CONFIG_OPTION_DEBUG */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_DEBUG_TASK_DEBUG_H__ */
