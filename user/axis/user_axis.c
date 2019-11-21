/**
 * @file user/axis/user_axis.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>
#include "user_axis.h"


void *user_func(void *arg)
{
  unsigned long id = *(int *) arg;

  while (true) {
    print("%lu", id);
    //  do_end_job(&ths[id]);
  }

  return NULL;
}

int user_thread_main(void)
{
  /* execute multiple threads. */

  int i;
  int nr_threads = 2;
  uint32_t ids[NR_THREADS];
  struct th_attr thas[NR_THREADS];

  init_thas(thas);
  sched_time = 5;
  print("user_thread_main()\n");

  /* setup parameters for threads. */
  for (i = 0; i < nr_threads; i++) {
    ids[i] = i + 1;
    thas[i].type = PERIODIC_TH | HARD_REAL_TIME;
    thas[i].phase = 0;
    thas[i].period = 100;
    thas[i].wcet = 10;
    thas[i].relative_deadline = thas[i].period;

    if (do_create_thread(user_func, &ids[i], &thas[i]) == NULL) {
      printk("Error: cannot create thread %d.\n", ids[i]);
      return -1;
    }
  }

  set_timer_period(USEC_TO_CPU_CLOCK(100));
  print("USEC_TO_CPU_CLOCK(100) = %lu\n", USEC_TO_CPU_CLOCK(100));
  run(nr_threads);
  return 0;
}


void timer_main(void)
{
  set_timer_period(USEC_TO_CPU_CLOCK(100));
  print("USEC_TO_CPU_CLOCK(100) = %lu\n", USEC_TO_CPU_CLOCK(100));
  enable_timer_interrupt();
  enable_timer();
  sched_time = 5;

  while (sched_end == false) {
    print("get_timer_count() = %lu\n", get_timer_count());
    //    wi();
    //    wait_until_next_interrupt();
  }
}

#define DST_BUFSIZE 256

int dmac_main(void)
{
  /* Destination address */
  uint32_t dst[DST_BUFSIZE];
  /* Source address */
  uint32_t src = 0;
  /* DMAC transfer size */
  size_t n = 64;
  uint32_t i;

  /* do DMAC. */
  do_local_dmac((uint32_t) dst, src, n, 0, DMAC_POLLING);
  //  do_local_dmac((uint32_t) dst, src, n, 0, DMAC_SYNC_INTERRUPT);
  //  do_local_dmac((uint32_t) dst, src, n, 0, DMAC_ASYNC_INTERRUPT);

  /* print data in src and destination addresses. */
  for (i = 0; i < n; i += 4) {
    printk("src 0x%x\n", mmio_in32(i));
    printk("dst 0x%x\n", mmio_in32((uint32_t) dst + i));
  }

  while (true) {
    sync();
  }

  return 0;
}


int test_main(void)
{
  print("test_main()\n");

  test_integer_instructions();
  test_transfer_instructions();
  test_control_instructions();
  test_special_instructions();

  return 0;
}

void callback_func(void)
{
  printk("callback_func()\n");
}

void callback_main(void)
{
  register_callback_handler(callback_func, 0);
  generate_software_interrupt(0);
}


void multi_cpus_main(__unused int argc, __unused char *argv[])
{
  print("get_cpu_id() = %lu\n", get_cpu_id());

  // start CPU1
  set_program_counter(1, 0);
  start_cpu(1);

  // NOTE: work well if CPU2 is implemented.
  // set_program_counter(2, 0);
  // start_cpu(2);
#if 0
  char str[] = "server";
  argc = 2;
  argv[1] = str;
  test_socket(argc, argv);
#endif

  volatile int i = 0;

  while (true) {
    /* NOTE: this code avoids finishing RTL simulation by infinite loop without operations. */
    i++;
    //    print("i = %d\n", i);
  }
}

void tsc_main(void)
{
  int i = 0;

  while (true) {
    print("get_time_stamp_counter() = %lu\n", get_time_stamp_counter());
    i++;

    if (i == 10) {
      set_time_stamp_counter(0);
    }
  }
}

int user_ap_main(__unused int argc, __unused char *argv[])
{
  unsigned long cpu = get_cpu_id();
  print("get_cpu_id() = %lu\n", cpu);
#if 0
  char str[] = "client";
  argc = 2;
  argv[1] = str;
  test_socket(argc, argv);
#endif
  //  cpu_ids[cpu] = cpu;
  return 0;
}


int user_arch_main(__unused int argc, __unused char *argv[])
{
  /* NOTE: uncomment selected functions for applications. */
  printk("user_arch_main()\n");
  //  user_thread_main();
  //  timer_main();
  //  dmac_main();
  //  test_main();
  //  callback_main();
  multi_cpus_main(argc, argv);
  //  tsc_main();
  //  cluster_main();
  return 0;
}
