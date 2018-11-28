/**
 * @file include/debug/sched_queue_debug.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_DEBUG_SCHED_QUEUE_DEBUG_H__
#define	__MCUBE_DEBUG_SCHED_QUEUE_DEBUG_H__

/* Never include this file directly. Include <debug/debug.h> instead. */

#ifndef __ASSEMBLY__

#if CONFIG_DEBUG


static inline void print_array(struct thread_struct array[NR_PRIORITIES])
{
	int i;
	struct thread_struct *th;
	PDEBUG("pdebug_array()\n");
	for (i = 0; i < NR_PRIORITIES; i++) {
		if (array[i].next != &array[i]) {
			PDEBUG("\tarray[%d]: ", i);
			for (th = array[i].next; th != &array[i]; th = th->next) {
				PDEBUG("id(%lu)->", th->id);
			}
			PDEBUG("[head]\n");
		}
	}
}


#define pdebug_array(array) do {																				\
		PDEBUG_WHERE();																											\
		print_array(array);																							\
	} while (0)

#define BITMAP_LENGTH (REG_LENGTH + REG_LENGTH / 8)

static inline void print_bitmap(uint32_t bitmap[NR_PRIORITY_BITMAPS])
{
	int i, j;
	char bitstr[BITMAP_LENGTH];
	uint32_t word;
	int len;
	PDEBUG("pdebug_bitmap()\n");
	for (i = 0; i < NR_PRIORITY_BITMAPS; i++) {
		len = BITMAP_LENGTH;
		word = bitmap[i];
		/* priority levels in each queue and output '_' every 8bit. */
		for (j = 0; j < len; j++) {
			bitstr[j] = (char) (word & 0x1) + '0';
			word >>= 1;
				if ((j % 9) == 7) {
					bitstr[++j] = '_';
				}
		}
		bitstr[--j] = '\0';
		PDEBUG("\tbitmap[%d]:%s\n", i, bitstr);
	}
}

#define pdebug_bitmap(bitmap) do {																		\
		PDEBUG_WHERE();																										\
		print_bitmap(bitmap);																					\
	}	while (0)


static inline void print_deadline_tq(void)
{
	struct thread_struct *p;
  unsigned long cpu = get_cpu_id();
	PDEBUG("[deadline_tq]: ");
	for (p = deadline_tq[cpu]; p; p = p->dnext) {
		PDEBUG("id(%lu)[%lu]->", p->id, p->sched.deadline);
	}
	PDEBUG("NULL\n");
}

#define pdebug_deadline_tq() do {																				\
		PDEBUG_WHERE();																								\
		print_deadline_tq();																					\
	} while (0)

static inline void print_sleep_tq(void)
{
	struct thread_struct *p;
  unsigned long cpu = get_cpu_id();
	PDEBUG("[sleep_tq]: ");
	for (p = sleep_tq[cpu]; p; p = p->next) {
		PDEBUG("id(%lu)->", p->id);
	}
	PDEBUG("NULL\n");
}

#define pdebug_sleep_tq() do {																					\
		PDEBUG_WHERE();																											\
		print_sleep_tq();																								\
	} while (0)

static inline void print_queue(struct thread_struct *head, char *name, unsigned long cpu)
{
	struct thread_struct *p;
	PDEBUG("[%s]: ", name);
	for (p = head; p && p != &kernel_th[cpu] && p->id != 0; p = p->next) {
		PDEBUG("id(%lu)->", p->id);
	}
	PDEBUG("NULL\n");
}

#define pdebug_queue(head, name, cpu) do {															\
		PDEBUG_WHERE();																									\
		print_queue(head, name, cpu);																				\
	} while (0)


#if CONFIG_TQ_BHEAP


#define pdebug_bheap(rq, th) do {								\
		PDEBUG_WHERE();															\
		PDEBUG("[bheap]: \n\t");										\
		print_bheap(rq, th);												\
		PDEBUG("\n");																\
	} while (0)
#else
#define pdebug_bheap(rq, th)
#endif /* CONFIG_TQ_BHEAP */

#else

#define pdebug_array(array)
#define pdebug_bitmap(bitmap)
#define pdebug_deadline_tq()
#define pdebug_sleep_tq()
#define pdebug_bheap(rq, th)

#endif /* CONFIG_DEBUG */

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_DEBUG_SCHED_QUEUE_DEBUG_H__ */
