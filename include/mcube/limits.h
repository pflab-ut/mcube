/**
 * @file include/mcube/limits.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_LIMITS_H__
#define __MCUBE_MCUBE_LIMITS_H__

#if CONFIG_ARCH_SIM
#include <limits.h>
#else

#define CHAR_BIT 8

#define SCHAR_MIN (-128)
#define SCHAR_MAX 127

#define UCHAR_MAX 255

#ifdef __CHAR_UNSIGNED__
#define CHAR_MIN 0
#define CHAR_MAX UCHAR_MAX
#else
#define CHAR_MIN SCHAR_MIN
#define CHAR_MAX SCHAR_MAX
#endif /* __CHAR_UNSIGNED__ */

#define SHRT_MIN (-32768)
#define SHRT_MAX 32767

#define USHRT_MAX 65535

#define INT_MAX 214748364
#define INT_MIN (-INT_MAX - 1)
#define UINT_MAX 4294967295U

#if REGISTER_LENGTH == 64
#define LONG_MAX 9223372036854775807L
#else
#define LONG_MAX 2147483647L
#endif /* __WORD_SIZE == 64 */
#define LONG_MIN (-LONG_MAX - 1L)

#if REGISTER_LENGTH == 64
#define ULONG_MAX 18446744073709551615UL
#else
#define ULONG_MAX 4294967295UL
#endif /* REGISTER_LENGTH == 64 */

#define LLONG_MAX 9223372036854775807LL
#define LLONG_MIN (-LLONG_MAX - 1LL)

#define ULLONG_MAX 18446744073709551615ULL

#define DBL_MAX 0x1.fffffffffffffp1023

#ifndef HUGE_VAL
#define HUGE_VAL DBL_MAX
#endif /* !HUGE_VAL */

/*
 * MAXs and MINs
 */

#define INT8_MAX  (0x7f)
#define INT16_MAX  (0x7fff)
#define INT32_MAX  (0x7fffffff)
#define INT64_MAX  (0x7fffffffffffffffL)

#define UINT8_MAX  (0xff)
#define UINT16_MAX  (0xffff)
#define UINT32_MAX  (0xffffffff)
#define UINT64_MAX  (0xffffffffffffffffUL)

/* MAX value of given unsigned variable type */
#define UTYPE_MAXVAL(x) ((typeof(x))UINT64_MAX)


#endif /* CONFIG_ARCH_SIM */


#endif /* __MCUBE_MCUBE_LIMITS_H__ */
