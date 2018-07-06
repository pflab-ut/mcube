/**
 * @file kernel/main.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

unsigned int Debug = FALSE;

unsigned long CPU_CLOCK;
unsigned long CPU_CLOCK_MHZ_PER_USEC;
#if !CONFIG_ARCH_AXIS
double CPU_USEC_PER_CLOCK_MHZ;
double CPU_CLOCK_MHZ_PER_NSEC;
double CPU_NSEC_PER_CLOCK_MHZ;
#endif /* !CONFIG_ARCH_AXIS */


int main(int argc, char *argv[])
{
  init_arch();
  //  putchar('b');
  //  console_write("hoge", 4, NULL);
  //  putchar('c');
  printk("main()\n");
#if 1
  printk("init_rq()\n");
  init_rq();
  printk("init_sched()\n");
  init_sched();
  
  printk("user_main()\n");
  user_main();
#endif
  exit_arch();
  return 0;
}

