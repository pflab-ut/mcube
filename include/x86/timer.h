/**
 * @file include/x86/timer.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_TIMER_H__
#define __MCUBE_X86_TIMER_H__

#define PIT_HZ 1193180 /* 1.19318MHz */

#define PIT_CNT0 0x0040
#define PIT_CTRL 0x0043

#ifndef __ASSEMBLY__


static inline unsigned long tsc2usec(unsigned long tsc)
{
	return ((unsigned long) ((tsc) * CPU_USEC_PER_CLOCK_MHZ + 0.5));
}

static inline unsigned long tsc2nsec(unsigned long tsc)
{
	return ((unsigned long) ((tsc) * CPU_NSEC_PER_CLOCK_MHZ + 0.5));
}

static inline unsigned long usec2tsc(unsigned long usec)
{
	return usec * CPU_CLOCK_MHZ_PER_USEC;
}

static inline unsigned long nsec2tsc(unsigned long nsec)
{
	return nsec * CPU_CLOCK_MHZ_PER_NSEC;
}

static inline void delay(unsigned long us)
{
  volatile unsigned long cur = rdtsc();
  while (tsc2usec(rdtsc() - cur) < us)
    pause();
}


static inline unsigned long get_current_cpu_time(void)
{
	return rdtscp();
	//	return read_HPET_counter();
}

extern void init_pit_timer(unsigned long tick_us);
extern void start_pit_timer(unsigned int ch);
extern void stop_pit_timer(unsigned int ch);

extern unsigned long cpu_bus_freq_mhz;
extern unsigned long sys_tsc;

extern unsigned long timer_count[NR_INTRA_KERNEL_CPUS];
extern unsigned long tcur[NR_INTRA_KERNEL_CPUS], tprev[NR_INTRA_KERNEL_CPUS];

#else

/* wait macro */
.macro  wait time
_wait:
   pause
	 subw $0x1, \time
	 cmpw $0x0, \time
	 jne _wait
.endm

.macro  wait2 time
_wait2:
   pause
	 subw $0x1, \time
	 cmpw $0x0, \time
	 jne _wait2
.endm

.macro  wait3 time
_wait3:
   pause
	 subw $0x1, \time
	 cmpw $0x0, \time
	 jne _wait3
.endm

.macro  wait4 time
_wait4:
   pause
	 subw $0x1, \time
	 cmpw $0x0, \time
	 jne _wait4
.endm

.macro  wait5 time
_wait5:
   pause
	 subw $0x1, \time
	 cmpw $0x0, \time
	 jne _wait5
.endm

#endif	/* !__ASSEMBLY__ */

#endif	/* __MCUBE_X86_TIMER_H__ */
