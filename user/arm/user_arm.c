/**
 * @file user/arm/user_arm.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void kernel_level_main(void);
void *user_func(void *arg);
int user_thread_main(void);
void process(const char *array);
int user_dmac_main(void);
void user_process(void);
void user_atomic_main(void);
void user_raspi3_main(void);


void kernel_level_main(void)
{
  unsigned long ret;
  unsigned long sp;
  asm volatile("mov %0, x30" : "=r"(ret));
  print("ret = 0x%lx\n", ret);
  asm volatile("mov %0, sp" : "=r"(sp));
  print("sp = 0x%x\n", sp);
  //  print("Kernel level started. EL %d\n", get_el());
  print("kernel_level_main(): EL %d\n", call_sys_get_mode_level());
  //  call_sys_move_to_kernel_level();
  move_to_user_level();
  print("kernel_level_main(): EL %d\n", call_sys_get_mode_level());
  asm volatile("mov %0, x30" : "=r"(ret));
  print("ret = 0x%lx\n", ret);
  asm volatile("mov %0, sp" : "=r"(sp));
  print("sp = 0x%x\n", sp);
  call_sys_move_to_kernel_level();
  print("kernel_level_main(): EL %d\n", call_sys_get_mode_level());
  asm volatile("mov %0, x30" : "=r"(ret));
  print("ret = 0x%lx\n", ret);
  asm volatile("mov %0, sp" : "=r"(sp));
  print("sp = 0x%x\n", sp);
  move_to_user_level();
  print("kernel_level_main(): EL %d\n", call_sys_get_mode_level());
  call_sys_move_to_kernel_level();
  print("kernel_level_main(): EL %d\n", call_sys_get_mode_level());

}


void *user_func(void *arg)
{
  unsigned long id = *(int *) arg;

  while (true) {
    print("%lu\n", id);
    //    delay(1000);
  }

  return NULL;
}

int user_thread_main(void)
{
  int i;
  int nr_threads = 1;
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


void process(const char *array)
{
  while (true) {
    print("%s", array);
    //    delay(100);
  }
}


#define BUFSIZE 32

int user_dmac_main(void)
{
  char src[BUFSIZE] = "hello world\n";
  char dst[BUFSIZE] = {0};

  print("user_dmac_main()\n");
  print("dst = 0x%lx\n", (unsigned long) dst);
#if 1
  do_local_dmac((unsigned long) dst, (unsigned long) src, sizeof(dst),
                0, DMAC_SYNC_INTERRUPT);
#else
  do_local_dmac((unsigned long) dst, (unsigned long) src, sizeof(dst),
                0, DMAC_POLLING);
#endif

  while (true) {
    print("dst = %s\n", dst);
    wfi();
  }

  return 0;
}



void user_process(void)
{

}


int user_ap_main(__unused int argc, __unused char *argv[])
{
#if 1
  int ret;
  print("user_ap_main()\n");
  ret = copy_process((unsigned long) &process, (unsigned long) "12345");

  if (ret != 0) {
    print("Error while starting process1\n");
    return 1;
  }

  ret = copy_process((unsigned long) &process, (unsigned long) "abcde");

  if (ret != 0) {
    print("Error while starting process 2");
    return 2;
  }

  return 0;
#else
  unsigned long cpu = get_cpu_id();
  delay(100000 * cpu);
  printk("user_ap_main()\n");

  for (;;)
    ;

  return 0;
#endif
}


int user_arch_main(__unused int argc, __unused char *argv[])
{
  //  print("user_arch_main()\n");
  //  user_thread_main();
  // user_ap_main();
  // user_dmac_main();
  //  kernel_level_main();
  barrier();
  //  test_ring_buf();
#if CONFIG_ARCH_ARM_RASPI3
  //  user_raspi3_main();
#endif /* CONFIG_ARCH_ARM_RASPI3 */
  print("user_arch_main(): end\n");
  return 0;
}
