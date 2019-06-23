/**
 * @file include/debug/sched_queue_debug.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_DEBUG_SCHED_QUEUE_DEBUG_H__
#define __MCUBE_DEBUG_SCHED_QUEUE_DEBUG_H__

#ifndef __ASSEMBLY__

#if CONFIG_OPTION_DEBUG

/**
 * @fn static inline void print_array(struct thread_struct array[NR_PRIORITIES])
 * @brief print array.
 *
 * @param array Array.
 */
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


/**
 * @def pdebug_array(array)
 * @brief print debug for array.
 *
 * @param array Array.
 */
#define pdebug_array(array) do {                \
    PDEBUG_WHERE();                             \
    print_array(array);                         \
  } while (0)

/**
 * @def BITMAP_LENGTH
 * @brief Bitmap length.
 */
#define BITMAP_LENGTH (REG_LENGTH + REG_LENGTH / 8)

/**
 * @fn static inline void print_bitmap(uint32_t bitmap[NR_PRIORITY_BITMAPS])
 * @brief print bitmap.
 *
 * @param bitmap Bitmap.
 */
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
      bitstr[j] = (char)(word & 0x1) + '0';
      word >>= 1;

      if ((j % 9) == 7) {
        bitstr[++j] = '_';
      }
    }

    bitstr[--j] = '\0';
    PDEBUG("\tbitmap[%d]:%s\n", i, bitstr);
  }
}

/**
 * @def pdebug_bitmap(bitmap)
 * @brief print debug for bitmap.
 *
 * @param bitmap Bitmap.
 */
#define pdebug_bitmap(bitmap) do {              \
    PDEBUG_WHERE();                             \
    print_bitmap(bitmap);                       \
  }  while (0)


/**
 * @fn static inline void print_deadline_tq(void)
 * @brief print deadline task queue.
 */
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

/**
 * @def pdebug_deadline_tq()
 * @brief print debug for deadline task queue.
 */
#define pdebug_deadline_tq() do {               \
    PDEBUG_WHERE();                             \
    print_deadline_tq();                        \
  } while (0)

/**
 * @fn static inline void print_sleep_tq(void)
 * @brief print sleep task queue.
 */
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

/**
 * @def pdebug_sleep_tq()
 * @brief print debug for sleep task queue.
 */
#define pdebug_sleep_tq() do {                  \
    PDEBUG_WHERE();                             \
    print_sleep_tq();                           \
  } while (0)

/**
 * @fn static inline void print_queue(struct thread_struct *head, char *name,
 *                                    unsigned long cpu)
 * @param print queue.
 *
 * @param head Head.
 * @param name Name.
 * @param cpu CPU.
 */
static inline void print_queue(struct thread_struct *head, char *name,
                               unsigned long cpu)
{
  struct thread_struct *p;
  PDEBUG("[%s]: ", name);

  for (p = head; p && p != &kernel_th[cpu] && p->id != 0; p = p->next) {
    PDEBUG("id(%lu)->", p->id);
  }

  PDEBUG("NULL\n");
}

/**
 * @def pdebug_queue(head, name, cpu)
 * @brief print debug for queue.
 *
 * @param head Head.
 * @param name Name.
 * @param cpu CPU.
 */
#define pdebug_queue(head, name, cpu) do {      \
    PDEBUG_WHERE();                             \
    print_queue(head, name, cpu);               \
  } while (0)


#if CONFIG_TQ_BHEAP

/**
 * @def pdebug_bheap(rq, th)
 * @brief print debug for binominal heap.
 *
 * @param rq Runqueue.
 * @param th Thread.
 */
#define pdebug_bheap(rq, th) do {               \
    PDEBUG_WHERE();                             \
    PDEBUG("[bheap]: \n\t");                    \
    print_bheap(rq, th);                        \
    PDEBUG("\n");                               \
  } while (0)
#else
/**
 * @def pdebug_bheap(rq, th)
 * @brief print debug for binominal heap.
 *
 * @param rq Runqueue.
 * @param th Thread.
 */
#define pdebug_bheap(rq, th)
#endif /* CONFIG_TQ_BHEAP */

#else

/**
 * @def pdebug_array(array)
 * @brief print debug for array.
 *
 * @param array Array.
 */
#define pdebug_array(array)

/**
 * @def pdebug_bitmap(bitmap)
 * @brief print debug for bitmap.
 *
 * @param bitmap Bitmap.
 */
#define pdebug_bitmap(bitmap)

/**
 * @def pdebug_deadline_tq()
 * @brief print debug for deadline task queue.
 */
#define pdebug_deadline_tq()

/**
 * @def pdebug_sleep_tq()
 * @brief print debug for sleep task queue.
 */
#define pdebug_sleep_tq()

/**
 * @def pdebug_bheap(rq, th)
 * @brief print debug for binominal heap.
 *
 * @param rq Runqueue.
 * @param th Thread.
 */
#define pdebug_bheap(rq, th)

#endif /* CONFIG_OPTION_DEBUG */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_DEBUG_SCHED_QUEUE_DEBUG_H__ */
