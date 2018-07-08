/**
 * @file arch/axis/cpu.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void copy_arch_process(struct task_struct *p, unsigned long func, unsigned long arg)
{
}


void init_cpu(void)
{
  CPU_CLOCK_MHZ_PER_USEC = 333;
  CPU_CLOCK = CPU_CLOCK_MHZ_PER_USEC * 1000 * 1000;
}
