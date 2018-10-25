/**
 * @file arch/axis/thread.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

/**
 * The run_user_thread() function runs user thread.
 */
void __attribute__((noreturn)) run_user_thread(void)
{
  size_t cpu = get_cpu_id();
  void *ret = NULL;
  //  print("begin_user_thread()\n");
  if (current_th[cpu]->run_user_func) {
    ret = (*current_th[cpu]->run_user_func)(current_th[cpu]->arg);
  }
  if (ret) {
    print("ret = 0x%lx\n", (unsigned long) ret);
  }
  wi();
  inf_loop();
}

void __attribute__((noreturn)) exit(int status)
{
	disable_local_irq();
	stop_cpu(0);
	inf_loop();
}
