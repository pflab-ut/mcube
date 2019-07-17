/**
 * @file kernel/main.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

#if CONFIG_ARCH_AXIS && CONFIG_COMPILER_CLANG

/* NOTE: padding to separate banks of text and data. */
/**
 * @def PADDING_SIZE
 * @brief Padding size.
 */
#define PADDING_SIZE 0x1000

/**
 * @def PADDING_DATA
 * @brief Padding data.
 */
#define PADDING_DATA 0xff

/**
 * @var PADDING[PADDING_SIZE]
 * @brief Padding.
 */
unsigned char PADDING[PADDING_SIZE] = {PADDING_DATA};

#endif

#if CONFIG_OPTION_DEBUG
bool Debug = true;
#else
bool Debug = false;
#endif /* CONFIG_OPTION_DEBUG */

unsigned long CPU_CLOCK;
unsigned long CPU_CLOCK_MHZ_PER_USEC;
#if defined(ENABLE_FPU)
double CPU_USEC_PER_CLOCK_MHZ;
double CPU_CLOCK_MHZ_PER_NSEC;
double CPU_NSEC_PER_CLOCK_MHZ;
#endif /* ENABLE_FPU */

/**
 * @var IsInitialized
 * @brief Is processor initialized?
 */
static volatile bool IsInitialized = false;

#if !CONFIG_ARCH_SIM
int errno;
#endif /* !CONFIG_ARCH_SIM */


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
  } else {
    /* Application Processors (APs) on ARM and AXIS.
     * APs on x86_64 is arch/x86_64/smpboot.c.
     */
    //    print("get_cpu_id() = %lu\n", get_cpu_id());
    init_arch_ap();

    /* wait until init_arch() is finished. */
    while (IsInitialized == false) {
    }

    /* execute the specific code of application processors (except bootstrap processor). */
    user_ap_main(argc, argv);
    exit_arch_ap();
  }

  return 0;
}
