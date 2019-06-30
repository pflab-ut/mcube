/**
 * @file arch/sim/utils.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void arch_switch_to(struct thread_struct *prev,
                    struct thread_struct *next)
{
  __uninitialized(prev);
  __uninitialized(next);
}
