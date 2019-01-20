/**
 * @file include/x86/bitmap.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_BITMAP_H__
#define __MCUBE_X86_BITMAP_H__

#ifndef __ASSEMBLY__

int64_t bitmap_first_set_bit(char *buf, uint len);
int64_t bitmap_first_zero_bit(char *buf, uint len);

void bitmap_set_bit(char *buf, uint bit, uint len);
void bitmap_clear_bit(char *buf, uint bit, uint len);

bool bitmap_bit_is_set(char *buf, uint bit, uint len);
bool bitmap_bit_is_clear(char *buf, uint bit, uint len);

#if BITMAP_TESTS
void bitmap_run_tests(void);
#else
static __unused void bitmap_run_tests(void) { }
#endif /* BITMAP_TESTS */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_BITMAP_H__ */
