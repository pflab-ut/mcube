/**
 * @file include/x86/pit.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_PIT_H__
#define __MCUBE_X86_PIT_H__

#ifndef __ASSEMBLY__

void pit_mdelay(int ms);
void pit_oneshot(int ms);
void pit_monotonic(int ms);


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_PIT_H__ */
