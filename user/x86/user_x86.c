/**
 * @file user/x86/user_x86.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void *user_func(void *arg)
{
  unsigned long id = *(int *) arg;
  while (1) {
    print("%lu", id);
    wait_until_next_interrupt();
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
  print("user_thread_main()\n");

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

extern int call_user_sys_write(char *buf);

int user_level_main(void)
{
  int ret;
  print("user_level_main()\n");
  ret = call_user_sys_write("hoge\n");
  print("ret = %d\n", ret);
  //  syscall1(SYS_write, (unsigned long) "hoge\n");
  //  print("user_level_main() exit\n");
  return 0;
}


int user_arch_main(void)
{
  //  user_thread_main();
  user_level_main();
  
  return 0;
}
