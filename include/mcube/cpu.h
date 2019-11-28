/**
 * @file include/mcube/cpu.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_CPU_H__
#define __MCUBE_MCUBE_CPU_H__

/**
 * @def NR_CPUS
 * @brief Number of CPUs.
 */
#if CONFIG_ARCH_AXIS
#define NR_CPUS 64
#else
#define NR_CPUS 4
#endif /* CONFIG_ARCH_AXIS */

/**
 * @def UNKNOWN_CPU_ID
 * @brief Unknown CPU ID.
 */
#define UNKNOWN_CPU_ID 0xffffffff

/**
 * @def CPU_CLOCK_TO_SEC(clock)
 * @brief CPU clock to second.
 *
 * @param clock CPU clock.
 */
#define CPU_CLOCK_TO_SEC(clock) ((clock) / CPU_CLOCK)

/**
 * @def CPU_CLOCK_TO_MSEC(clock)
 * @brief CPU clock to millisecond.
 *
 * @param clock CPU clock.
 */
#define CPU_CLOCK_TO_MSEC(clock) ((clock) / (CPU_CLOCK / 1000))

/**
 * @def CPU_CLOCK_TO_USEC(clock)
 * @brief CPU clock to microsecond.
 *
 * @param clock CPU clock.
 */
#define CPU_CLOCK_TO_USEC(clock) ((clock) / (CPU_CLOCK / (1000 * 1000)))

/**
 * @def SEC_TO_CPU_CLOCK(sec)
 * @brief Second to CPU clock.
 *
 * @param sec Second.
 */
#define SEC_TO_CPU_CLOCK(sec) ((sec) * CPU_CLOCK)

/**
 * @def MSEC_TO_CPU_CLOCK(msec)
 * @brief Millisecond to CPU clock.
 *
 * @param msec Millisecond.
 */
#define MSEC_TO_CPU_CLOCK(msec) ((msec) * (CPU_CLOCK / 1000))

/**
 * @def USEC_TO_CPU_CLOCK(usec)
 * @brief Microsecond to CPU clock.
 *
 * @param usec Microsecond.
 */
#define USEC_TO_CPU_CLOCK(usec) ((usec) * (CPU_CLOCK / (1000 * 1000)))

#ifndef __ASSEMBLY__


/**
 * @var CPU_CLOCK
 * @brief CPU clock [usec/clock].
 */
extern unsigned long CPU_CLOCK;

/**
 * @var CPU_CLOCK_MHZ_PER_USEC
 * @brief CPU clock MHz per usec (MHz] = [clock/usec]).
 */
extern unsigned long CPU_CLOCK_MHZ_PER_USEC;

#if defined(ENABLE_FPU)

/**
 * @var CPU_USEC_PER_CLOCK_MHZ
 * @brief CPU microsecond per clock MHz.
 */
extern double CPU_USEC_PER_CLOCK_MHZ;

/**
 * @var CPU_CLOCK_MHZ_PER_NSEC
 * @brief CPU clock MHz per nanosecond.
 */
extern double CPU_CLOCK_MHZ_PER_NSEC;

/**
 * @var CPU_NSEC_PER_CLOCK_MHZ
 * @brief CPU nanosecond per clock MHz.
 */
extern double CPU_NSEC_PER_CLOCK_MHZ;

#endif /* ENABLE_FPU */

/**
 * @fn void set_cpu_clock(void)
 * @brief set CPU clock.
 */
void set_cpu_clock(void);

/**
 * @fn void init_fpu(void)
 * @brief initialize FPU.
 */
void init_fpu(void);

/**
 * @fn void init_cpu(void)
 * @brief initialize CPU.
 */
void init_cpu(void);

/**
 * @fn void init_ap(void)
 * @brief initialize Application Processors (APs).
 */
void init_ap(void);

/**
 * @fn void init_smp(void)
 * @brief initialize SMP.
 */
void init_smp(void);

struct full_regs;

/**
 * @fn void dump_registers(struct full_regs *regs)
 * @brief dump full registers.
 *
 * @param regs Full registers.
 */
void dump_registers(struct full_regs *regs);


#if CONFIG_ARCH_SIM || CONFIG_ARCH_X86_64
/**
 * @struct cpuid_info
 * @brief CPU ID information.
 */
struct cpuid_info {
  /**
   * EAX register.
   */
  uint64_t rax;

  /**
   * EBX register.
   */
  uint64_t rbx;

  /**
   * ECX register.
   */
  uint64_t rcx;

  /**
   * EDX register.
   */
  uint64_t rdx;
};

/**
 * @typedef cpuid_info_t
 * @brief Typdef of @struct cpuid_info.
 */
typedef struct cpuid_info cpuid_info_t;

/**
 * @fn static inline void cpuid(uint64_t op, cpuid_info_t *cinfo)
 * @brief wrap @c cpuid operation.
 */
static inline void cpuid(uint64_t op, cpuid_info_t *cinfo)
{
  cinfo->rax = op;
  cinfo->rcx = 0;
  asm volatile("cpuid"
               : "=a"(cinfo->rax),
               "=b"(cinfo->rbx),
               "=c"(cinfo->rcx),
               "=d"(cinfo->rdx)
               : "0"(cinfo->rax), "2"(cinfo->rcx));
}

/**
 * @fn static inline int get_nr_cpu_cores(void)
 * @brief get number of CPU cores.
 *
 * @return Number of CPU cores.
 */
static inline int get_nr_cpu_cores(void)
{
  cpuid_info_t cinfo;
  cpuid(0x4, &cinfo);

  if (cinfo.rax & 0x1f) {
    return ((cinfo.rax >> 26) + 1);
  }

  return 1;
}


#endif /* CONFIG_ARCH_SIM || CONFIG_ARCH_X86_64 */


#endif /* __ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_CPU_H__ */
