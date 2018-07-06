/**
 * @file include/debug/thread_debug.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_DEBUG_THREAD_DEBUG_H__
#define	__MCUBE_DEBUG_THREAD_DEBUG_H__

/* Never include this file directly. Include <debug/debug.h> instead. */
#ifndef __ASSEMBLY__

#if CONFIG_DEBUG

static inline void print_thread_queue(struct thread_struct *th)
{
	PDEBUG("id: %lu, prev: %lx, next: %lx\n",
				 th->id, (unsigned long) th->prev, (unsigned long) th->next);
}

#define pdebug_thread_queue(th) do {																		\
		PDEBUG_WHERE();																											\
		print_thread_queue(th);																							\
	} while (0)

static inline void print_thread(struct thread_struct *th)
{
	PDEBUG("id = %lu, remaining = %ld state = %lx release = %lu deadline = %lu \n",
				 th->id, th->sched.remaining, th->state, th->sched.release, th->sched.deadline);
}


#define pdebug_thread(th) do {																					\
		PDEBUG_WHERE();																											\
		print_thread(th);																										\
	} while (0)


static inline void print_thread_by_id(int id)
{
	struct thread_struct *th;
	if (id == 0) {
		th = &idle_th[0];
	} else {
		th = &ths[id-1];
	}
	if (th) {
		pdebug_thread(th);
	}
}

#define pdebug_thread_by_id(id) do {						\
		PDEBUG_WHERE();															\
		print_thread_by_id(id);											\
	} while (0)

#else
#define	pdebug_thread_queue(th)
#define	pdebug_thread(th)
#define	pdebug_thread_byid(id)

#endif /* CONFIG_DEBUG */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_DEBUG_THREAD_DEBUG_H__ */
