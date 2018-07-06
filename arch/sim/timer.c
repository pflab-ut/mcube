/**
 * @file arch/sim/timer.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void start_timer(unsigned int ch)
{
}

void stop_timer(unsigned int ch)
{
}


void update_jiffies(void)
{
	exec_jiffies = 0;
	sys_jiffies++;
}

