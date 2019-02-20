/**
 * @file include/x86/pit.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_PIT_H__
#define __MCUBE_X86_PIT_H__

#ifndef __ASSEMBLY__

extern volatile int pit_ticks_count;

void pit_mdelay(int ms);
void pit_oneshot(int ms);
void pit_monotonic(int ms);

void __pit_periodic_handler(void);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_PIT_H__ */
