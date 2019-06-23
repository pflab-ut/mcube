/**
 * @file include/debug/thread_debug.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_DEBUG_THREAD_DEBUG_H__
#define __MCUBE_DEBUG_THREAD_DEBUG_H__

#ifndef __ASSEMBLY__

#if CONFIG_OPTION_DEBUG

/**
 * @fn static inline void print_thread_queue(struct thread_struct *th)
 * @brief print thread queue.
 *
 * @param th Thread.
 */
static inline void print_thread_queue(struct thread_struct *th)
{
  PDEBUG("id: %lu, prev: %lx, next: %lx\n",
         th->id, (unsigned long) th->prev, (unsigned long) th->next);
}

/**
 * @def pdebug_thread_queue(th)
 * @brief print debug for thread queue.
 *
 * @param th Thread.
 */
#define pdebug_thread_queue(th) do {            \
    PDEBUG_WHERE();                             \
    print_thread_queue(th);                     \
  } while (0)

/**
 * @fn static inline void print_thread(struct thread_struct *th)
 * @brief print thread.
 *
 * @param th Thread.
 */
static inline void print_thread(struct thread_struct *th)
{
  PDEBUG("id = %lu, remaining = %ld state = %lx release = %lu deadline = %lu \n",
         th->id, th->sched.remaining, th->state, th->sched.release, th->sched.deadline);
}

/**
 * @def pdebug_thread(th)
 * @brief print debug for thread.
 */
#define pdebug_thread(th) do {                  \
    PDEBUG_WHERE();                             \
    print_thread(th);                           \
  } while (0)


/**
 * @fn static inline void print_thread_by_id(int id)
 * @brief print thread by ID.
 */
static inline void print_thread_by_id(int id)
{
  struct thread_struct *th;

  if (id == 0) {
    th = &kernel_th[0];
  } else {
    th = &ths[id - 1];
  }

  if (th) {
    pdebug_thread(th);
  }
}

/**
 * @def pdebug_thread_by_id(id)
 * @brief print debug for thread by ID.
 *
 * @param id ID.
 */
#define pdebug_thread_by_id(id) do {            \
    PDEBUG_WHERE();                             \
    print_thread_by_id(id);                     \
  } while (0)

#else
/**
 * @def pdebug_thread_queue(th)
 * @brief print debug for thread queue.
 *
 * @param th Thread.
 */
#define pdebug_thread_queue(th)

/**
 * @def pdebug_thread(th)
 * @brief print debug for thread.
 */
#define pdebug_thread(th)

/**
 * @def pdebug_thread_by_id(id)
 * @brief print debug for thread by ID.
 *
 * @param id ID.
 */
#define pdebug_thread_by_id(id)

#endif /* CONFIG_OPTION_DEBUG */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_DEBUG_THREAD_DEBUG_H__ */
