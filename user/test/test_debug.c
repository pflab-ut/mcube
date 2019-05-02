/**
 * @file user/test/test_debug.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

bool test_debug(void)
{
  /* dummy registers */
  __unused struct full_regs regs;

  pdebug_jiffies();
  pdebug_registers(regs);
  pdebug_array(run_tq[0].array);
  pdebug_bitmap(run_tq[0].bitmap);
  pdebug_deadline_tq();
  pdebug_sleep_tq();
#if CONFIG_TQ_BHEAP
  pdebug_bheap(&run_tq, run_tq[0].head);
#endif /* CONFIG_TQ_BHEAP */
  pdebug_thread_in_line();
  pdebug_task(tasks[0]);
  pdebug_task_by_thid(ths[0].id);

  return true;
}
