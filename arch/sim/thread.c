/**
 * @file arch/sim/thread.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#include <pthread.h>



/**
 * The run_user_thread() function runs user thread.
 */
__noreturn void run_user_thread(void)
{
  unsigned long cpu = get_cpu_id();
  void *ret = NULL;

  //  prinf("begin_user_thread()\n");
  if (current_th[cpu]->run_user_func) {
    ret = (*current_th[cpu]->run_user_func)(current_th[cpu]->arg);
  }

  if (ret) {
    printf("ret = 0x%lx\n", (unsigned long) ret);
  }

  halt();
  inf_loop();
}

