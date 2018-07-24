/**
 * @file user/axis/user_axis.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void test_integer_instructions(void);
void test_transfer_instructions(void);
void test_control_instructions(void);
void test_special_instructions(void);

void *user_func(void *arg)
{
  unsigned long id = *(int *) arg;
  while (1) {
    printk("%lu", id);
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
  set_timer_period(USEC_TO_CPU_CLOCK(100));
  printk("USEC_TO_CPU_CLOCK(100) = %lu\n", USEC_TO_CPU_CLOCK(100));
  run(nr_threads);
  return 0;
}


int timer_main(void)
{
  set_timer_period(USEC_TO_CPU_CLOCK(100));
  printk("USEC_TO_CPU_CLOCK(100) = %lu\n", USEC_TO_CPU_CLOCK(100));
  enable_timer_interrupt();
  enable_timer();
  while (1) {
    printk("get_timer_count() = %lu\n", get_timer_count());
    //    wi();
    //    wait_until_next_interrupt();
  }
  return 0;
}

int dmac_main(void)
{
  uint32_t dst = MEMORY_SIZE / 2;
  //  uint32_t dst = 0x10;
  uint32_t src = 0;
  uint32_t n = 16;
  uint32_t i;
  
  //  do_local_dmac(dst, src, n, DMAC_POLLING);
  //  do_local_dmac(dst, src, n, DMAC_SYNC_INTERRUPT);
  do_local_dmac(dst, src, n, DMAC_ASYNC_INTERRUPT);
  for (i = 0; i < n; i += 4) {
    printk("src 0x%x\n", mmio_in32(i));
    printk("dst 0x%x\n", mmio_in32(dst + i));
  }
  while (1) {
    sync();
    //printk("hoge\n");
  }
  return 0;
}


int test_main(void)
{
#if CONFIG_ARCH_AXIS
  printk("test_main()\n");
  test_integer_instructions();
  test_transfer_instructions();
  test_control_instructions();
  test_special_instructions();
#endif /* CONFIG_ARCH_AXIS */
  return 0;
}


int user_arch_main(void)
{
  //  user_thread_main();
  //  timer_main();
  //  dmac_main();
  //  test_main();
#if 1
  printk("get_cpu_id() = %lu\n", get_cpu_id());
  set_program_counter(1, 0);
  start_cpu(1);
  //  set_program_counter(2, 0);
  //  start_cpu(2);
  volatile int i = 0;
  while (1) {
    i++;
  }
  extern volatile unsigned long cpu_ids[256];
  while (cpu_ids[1] == 0) {
  }
  //  printk("cpu_ids[1] = %lu\n", cpu_ids[1]);
  
#else
  int i = 0;
  while (1) {
    printk("get_time_stamp_counter() = %lu\n", get_time_stamp_counter());
    i++;
    if (i == 10) {
      set_time_stamp_counter(0);
    }
  }
#endif
  return 0;
}
