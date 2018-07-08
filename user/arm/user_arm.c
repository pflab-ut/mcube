/**
 * @file user/axis/user_axis.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void *user_func(void *arg)
{
  unsigned long id = *(int *) arg;
  while (1) {
    printk("%lu", id);
    delay(1000);
    //  do_end_job(&ths[id]);
  }
  return NULL;
}

int user_thread_main(void)
{
  int i;
  int nr_threads = 3;
  uint32_t ids[NR_THREADS];
  struct th_attr thas[NR_THREADS] = INIT_THAS;
  sched_time = 5;
  printk("user_thread_main()\n");
  
  for (i = 0; i < nr_threads; i++) {
    ids[i] = i + 1;
    thas[i].type = PERIODIC_TH | HARD_REAL_TIME;
    thas[i].phase = 0;
    thas[i].period = 100;
    thas[i].wcet = 10;
    thas[i].relative_deadline = thas[i].period;
    do_create_thread(user_func, &ids[i], &thas[i]);
  }
  init_timer(TICK_USEC);
  //  set_timer_period(USEC_TO_CPU_CLOCK(100));
  run(nr_threads);
  return 0;
}



int user_arch_main(void)
{
  user_thread_main();
  return 0;
}
