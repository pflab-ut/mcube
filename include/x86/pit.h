/**
 * @file include/x86/pit.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_PIT_H__
#define __MCUBE_X86_PIT_H__

#ifndef __ASSEMBLY__

extern volatile int pit_ticks_count;

void pit_udelay(uint64_t us);
void pit_oneshot(uint64_t us);
void pit_monotonic(uint64_t us);

void __pit_periodic_handler(void);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_PIT_H__ */
