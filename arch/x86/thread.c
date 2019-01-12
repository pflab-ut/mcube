/**
 * @file arch/x86/thread.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void set_initial_context(struct thread_struct *th,
                         void (*pc)(void *),
                         void *func,
                         unsigned long bottom)
{
}


void __attribute__((noreturn)) run_user_thread(void)
{
#if 0
  unsigned long cpu = get_cpu_id();
  void *ret = NULL;
  unsigned long rsp;
  printk("run_user_thread()\n");
  printk("current_th[cpu]->id = %lu\n", current_th[cpu]->id);
  asm volatile("mov %%rsp, %0" : "=r"(rsp));
  printk("rsp = 0x%lx\n", rsp);
  if (current_th[cpu]->run_user_func) {
    ret = (*current_th[cpu]->run_user_func)(current_th[cpu]->arg);
  }
  if (ret) {
    printk("ret = 0x%lx\n", (unsigned long) ret);
  }
  hlt();
  inf_loop();
#endif
}

void wait(unsigned long count)
{
#if 0
  while (get_current_cpu_time() < count)
    ;
#endif
}


void __attribute__((noreturn)) exit(int status)
{
#if 0
  cli();
  hlt();
  inf_loop();
#endif
}
