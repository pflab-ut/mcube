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
#if CONFIG_ARCH_SIM || CONFIG_ARCH_ARM
extern double CPU_USEC_PER_CLOCK_MHZ;
extern double CPU_CLOCK_MHZ_PER_NSEC;
extern double CPU_NSEC_PER_CLOCK_MHZ;
#endif /* CONFIG_ARCH_SIM || CONFIG_ARCH_ARM */

extern unsigned long ap_start;

int ap_main(void);
int ap_usermain(void);

void init_fpu(void);
void init_cpu(void);
void init_ap(void);
void init_smp(void);

void vector(void);



#endif /* __ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_CPU_H__ */
