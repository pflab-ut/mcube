/**
 * @file arch/sim/simulator.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


#if CONFIG_USER_TASK_PTHREAD
#include <pthread.h>
#endif /* CONFIG_USER_TASK_PTHREAD */


/* NOTE: current_cpu == get_cpu_id() */
unsigned long current_cpu = 0;

