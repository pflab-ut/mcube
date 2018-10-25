/**
 * @file arch/arm/cpu.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void copy_arch_process(struct task_struct *p, unsigned long func, unsigned long arg)
{
  unsigned long cpu = get_cpu_id();
  current_th[cpu]->cpu_context.x19 = func;
  current_th[cpu]->cpu_context.x20 = arg;
  current_th[cpu]->cpu_context.pc = (unsigned long) ret_from_fork;
  current_th[cpu]->cpu_context.sp = (unsigned long) p + THREAD_SIZE;
}


void set_cpu_frequency(void)
{
  CPU_CLOCK = 1.2 * 1000 * 1000 * 1000;
  CPU_CLOCK_MHZ_PER_USEC = (CPU_CLOCK + 500 * 1000) / (1000 * 1000);
  CPU_USEC_PER_CLOCK_MHZ = (1.0 * 1000 * 1000) / CPU_CLOCK;
  CPU_CLOCK_MHZ_PER_NSEC = CPU_CLOCK / (1.0 * 1000 * 1000 * 1000);
  CPU_NSEC_PER_CLOCK_MHZ = (1.0 * 1000 * 1000 * 1000) / CPU_CLOCK;
  print("CPU_CLOCK = %lu\n", CPU_CLOCK);
  print("CPU_CLOCK_MHZ_PER_USEC = %lu\n", CPU_CLOCK_MHZ_PER_USEC);
}


void init_cpu(void)
{
  //  set_cpu_frequency();
}
