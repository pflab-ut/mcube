/**
 * @file arch/aarch64/thread.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


/**
 * The run_user_thread() function runs user thread.
 */
__noreturn void run_user_thread(void)
{
  unsigned long cpu = get_cpu_id();
  void *ret = NULL;

  //  printk("begin_user_thread()\n");
  if (current_th[cpu]->run_user_func) {
    ret = (*current_th[cpu]->run_user_func)(current_th[cpu]->arg);
  }

  if (ret) {
    printk("ret = 0x%lx\n", (unsigned long) ret);
  }

  wfi();
  inf_loop();
}

void exit(int status)
{
  __uninitialized(status);

  for (;;)
    ;
}

void wait(unsigned long count)
{
  while (get_current_cpu_time() < count)
    ;
}
