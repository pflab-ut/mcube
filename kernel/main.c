/**
 * @file kernel/main.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if CONFIG_ARCH_AXIS && CONFIG_COMPILER_CLANG
/* NOTE: padding to separate banks of text and data. */
#define PADDING_SIZE 0x1000
#define PADDING_DATA 0xff
char PADDING[PADDING_SIZE] = {PADDING_DATA};
#endif

unsigned int Debug = false;
//unsigned int Debug = true;

unsigned long CPU_CLOCK;
unsigned long CPU_CLOCK_MHZ_PER_USEC;
#if defined(ENABLE_FPU)
double CPU_USEC_PER_CLOCK_MHZ;
double CPU_CLOCK_MHZ_PER_NSEC;
double CPU_NSEC_PER_CLOCK_MHZ;
#endif /* ENABLE_FPU */

static volatile bool IsInitialized = false;

int main(int argc, char *argv[])
{
  unsigned long cpu = get_cpu_id();

  if (cpu == 0) {
    init_arch();
    print("main()\n");
    init_rq();
    init_sched();
    IsInitialized = true;
    user_main(argc, argv);
    exit_arch();
    print("main() end\n");
  } else {
    //    print("get_cpu_id() = %lu\n", get_cpu_id());
    init_arch_ap();

    /* wait until init_arch() is finished. */
    while (IsInitialized == false) {
    }

    /* execute the specific code of application processors (except bootstrap processor). */
    ap_main();
    exit_arch_ap();
  }

  return 0;
}
