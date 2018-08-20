/**
 * @file kernel/main.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

unsigned int Debug = FALSE;
//unsigned int Debug = TRUE;

unsigned long CPU_CLOCK;
unsigned long CPU_CLOCK_MHZ_PER_USEC;
#if !CONFIG_ARCH_AXIS
double CPU_USEC_PER_CLOCK_MHZ;
double CPU_CLOCK_MHZ_PER_NSEC;
double CPU_NSEC_PER_CLOCK_MHZ;
#endif /* !CONFIG_ARCH_AXIS */

static unsigned int is_initialized = FALSE;

int main(int argc, char *argv[])
{
  
  if (get_cpu_id() == 0) {
    init_arch();
    is_initialized = TRUE;
    printk("main()\n");
    init_rq();
    printk("main()2\n");
    init_sched();
    printk("main()3\n");
    //    printk("%016lx\n", 0x1234);
    //    volatile int i = 1 / 0;
    
    user_main();
    exit_arch();
    //    printk("main()4\n");
  } else {
    printk("get_cpu_id() = %lu\n", get_cpu_id());
    /* wait until init_arch() is finished. */
    while (is_initialized == FALSE) {
    }
    /* do application processor's specific code. */
    ap_main();
  }
  return 0;
}
