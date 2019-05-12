/**
 * @file include/mcube/cpu.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_CPU_H__
#define __MCUBE_MCUBE_CPU_H__

#define NR_CPUS 4

/** CPU clock to second. */
#define CPU_CLOCK_TO_SEC(clock) ((clock) / CPU_CLOCK)
/** CPU clock to millisecond. */
#define CPU_CLOCK_TO_MSEC(clock) ((clock) / (CPU_CLOCK / 1000))
/** CPU clock to microsecond. */
#define CPU_CLOCK_TO_USEC(clock) ((clock) / (CPU_CLOCK / (1000 * 1000)))

/** Second to CPU clock. */
#define SEC_TO_CPU_CLOCK(sec) ((sec) * CPU_CLOCK)
/** Millisecond to CPU clock. */
#define MSEC_TO_CPU_CLOCK(msec) ((msec) * (CPU_CLOCK / 1000))
/** Microsecond to CPU clock. */
#define USEC_TO_CPU_CLOCK(usec) ((usec) * (CPU_CLOCK / (1000 * 1000)))

#ifndef __ASSEMBLY__


/* CPU clock. */

/* [usec/clock] */
extern unsigned long CPU_CLOCK;
extern unsigned long CPU_CLOCK_MHZ_PER_USEC; /* [MHz]=[clock/usec] */
#if defined(ENABLE_FPU)
extern double CPU_USEC_PER_CLOCK_MHZ;
extern double CPU_CLOCK_MHZ_PER_NSEC;
extern double CPU_NSEC_PER_CLOCK_MHZ;
#endif /* ENABLE_FPU */


#if CONFIG_ARCH_SIM || CONFIG_ARCH_X86
/**
 * @struct cpuid_info
 * @brief CPU ID information
 */
struct cpuid_info {
  /** EAX register. */
  uint64_t rax;
  /** EBX register. */
  uint64_t rbx;
  /** ECX register. */
  uint64_t rcx;
  /** EDX register. */
  uint64_t rdx;
};

typedef struct cpuid_info cpuid_info_t;

static inline void cpuid(uint64_t op,
                         cpuid_info_t *cinfo)
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

static inline int get_nr_cpu_cores(void)
{
  cpuid_info_t cinfo;
  cpuid(0x4, &cinfo);

  if (cinfo.rax & 0x1f) {
    return ((cinfo.rax >> 26) + 1);
  }

  return 1;
}

#endif /* CONFIG_ARCH_SIM || CONFIG_ARCH_X86 */


void set_cpu_clock(void);

void init_fpu(void);
void init_cpu(void);
void init_ap(void);
void init_smp(void);

struct full_regs;
void dump_registers(struct full_regs *regs);


#endif /* __ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_CPU_H__ */
