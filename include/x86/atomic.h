/**
 * @file include/x86/atomic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_ATOMIC_H__
#define __MCUBE_X86_ATOMIC_H__

#ifndef __ASSEMBLY__

uint8_t atomic_bit_test_and_set(uint32_t *val);
uint64_t atomic_inc(uint64_t *val);

#if    ATOMIC_TESTS
void atomic_run_tests(void);
#else
static void __unused atomic_run_tests(void) { }
#endif

#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_X86_ATOMIC_H__ */
