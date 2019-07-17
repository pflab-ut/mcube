/**
 * @file arch/sim/init.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


void init_arch(void)
{
  init_spinlock();
  init_cpu();
  init_kmalloc();
}

void init_arch_ap(void)
{
}

void exit_arch(void)
{
}

void exit_arch_ap(void)
{
}

