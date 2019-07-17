/**
 * @file arch/aarch64/cpu.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>



/**
 * @fn static void enable_pmu(void)
 * @brief enable PMU.
 */
static void enable_pmu(void)
{
  unsigned long val = 0;

  /* Enable user-mode access to counters. */
  asm volatile("msr pmuserenr_el0, %0" :: "r"
               ((unsigned long) PMUSERENR_EN | PMUSERENR_ER | PMUSERENR_CR));

  /* Performance Monitors Count Enable Set register bit 30:0 disable, 31 enable.
   * Can also enable other event counters here. */
  asm volatile("msr pmcntenset_el0, %0" :: "r"((unsigned long) PMCNTENSET_C));

  /* Enable counters */
  asm volatile("mrs %0, pmcr_el0" : "=r"(val));
  asm volatile("msr pmcr_el0, %0" : : "r"(val | PMCR_E));
}


void set_cpu_clock(void)
{
  /* Raspberry Pi 3: 1.2 GHz */
  unsigned long clock = get_cntfrq_el0();
  unsigned long begin = get_cntvct_el0();
  unsigned long cpu_clock = get_pmccntr_el0();

  /* wait for 1s */
  while (get_cntvct_el0() - begin <= clock) {
  }

  cpu_clock = get_pmccntr_el0() - cpu_clock;
  //  printk("CPU Clock = %lu\n", cpu_clock);

  CPU_CLOCK = cpu_clock;
  CPU_CLOCK_MHZ_PER_USEC = (CPU_CLOCK + 500 * 1000) / (1000 * 1000);
  CPU_USEC_PER_CLOCK_MHZ = (1.0 * 1000 * 1000) / CPU_CLOCK;
  CPU_CLOCK_MHZ_PER_NSEC = CPU_CLOCK / (1.0 * 1000 * 1000 * 1000);
  CPU_NSEC_PER_CLOCK_MHZ = (1.0 * 1000 * 1000 * 1000) / CPU_CLOCK;
}


void init_cpu(void)
{
  enable_pmu();
  set_cpu_clock();
}
