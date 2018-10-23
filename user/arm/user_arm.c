/**
 * @file user/arm/user_arm.c
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


void process(const char *array)
{
  while (1) {
    printk("%s", array);
    //    delay(100);
  }
}

int user_ap_main(void)
{
  int ret;
  printk("user_ap_main()\n");
  ret = copy_process((unsigned long) &process, (unsigned long) "12345");
  if (ret != 0) {
    printk("Error while starting process1\n");
    return 1;
  }
  ret = copy_process((unsigned long) &process, (unsigned long) "abcde");
  if (ret != 0) {
    printk("Error while starting process 2");
    return 2;
  }
  return 0;
}




void user_process(void)
{
  
}

int user_mode_main(void)
{
  printk("Kernel process started. EL %d\r\n", get_el());
  move_to_user_mode();
  call_sys_write("Now process is user mode.\n");
  for (;;)
    ;
  return 0;
}


int user_arch_main(void)
{
  //  user_thread_main();
  //  user_ap_main();
  user_mode_main();
  
  return 0;
}
