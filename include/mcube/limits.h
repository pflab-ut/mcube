/**
 * @file include/mcube/limits.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_LIMITS_H__
#define __MCUBE_MCUBE_LIMITS_H__

#if !CONFIG_ARCH_SIM

/**
 * @def CHAR_BIT
 * @brief Character bitwidth.
 */
#define CHAR_BIT 8

/**
 * @def SCHAR_MIN
 * @brief Signed character minimum.
 */
#define SCHAR_MIN (-128)

/**
 * @def SCHAR_MAX
 * @brief Signed character maximum.
 */
#define SCHAR_MAX 127

/**
 * @def UCHAR_MAX
 * @brief Unsigned character maximum.
 */
#define UCHAR_MAX 255

#ifdef __CHAR_UNSIGNED__

/**
 * @def CHAR_MIN
 * @brief Character minimum.
 */
#define CHAR_MIN 0

/**
 * @def CHAR_MAX
 * @brief Character maximum.
 */
#define CHAR_MAX UCHAR_MAX
#else

/**
 * @def CHAR_MIN
 * @brief Character minimum.
 */
#define CHAR_MIN SCHAR_MIN

/**
 * @def CHAR_MAX
 * @brief Character maximum.
 */
#define CHAR_MAX SCHAR_MAX
#endif /* __CHAR_UNSIGNED__ */

/**
 * @def SHRT_MIN
 * @brief Short minimum.
 */
#define SHRT_MIN (-32768)

/**
 * @def SHRT_MAX
 * @brief Short maximum.
 */
#define SHRT_MAX 32767

/**
 * @def USHRT_MAX
 * @brief Unsigned short maximum.
 */
#define USHRT_MAX 65535

/**
 * @def INT_MAX
 * @brief Integer maximum.
 */
#define INT_MAX 214748364

/**
 * @def INT_MIN
 * @brief Integer minimum.
 */
#define INT_MIN (-INT_MAX - 1)

/**
 * @def UINT_MAX
 * @brief Unsigned integer maximum.
 */
#define UINT_MAX 4294967295U

#if !CONFIG_ARCH_AXIS

/**
 * @def LONG_MAX
 * @brief Long maximum.
 */
#define LONG_MAX 9223372036854775807L
#else

/**
 * @def LONG_MAX
 * @brief Long maximum.
 */
#define LONG_MAX 2147483647L
#endif /* !CONFIG_ARCH_AXIS */

/**
 * @def LONG_MIN
 * @brief Long minimum.
 */
#define LONG_MIN (-LONG_MAX - 1L)

#if !CONFIG_ARCH_AXIS

/**
 * @def ULONG_MAX
 * @brief Unsigned long maximum.
 */
#define ULONG_MAX 18446744073709551615UL
#else

/**
 * @def ULONG_MAX
 * @brief Unsigned long maximum.
 */
#define ULONG_MAX 4294967295UL
#endif /* REGISTER_LENGTH == 64 */

/**
 * @def LLONG_MAX
 * @brief Long long maximum.
 */
#define LLONG_MAX 9223372036854775807LL

/**
 * @def LLONG_MIN
 * @brief Long long minimum.
 */
#define LLONG_MIN (-LLONG_MAX - 1LL)

/**
 * @def ULLONG_MAX
 * @brief Unsigned long long maximum.
 */
#define ULLONG_MAX 18446744073709551615ULL

#ifndef DBL_MAX

/**
 * @def DBL_MAX
 * @brief Double maximum.
 */
#define DBL_MAX 0x1.fffffffffffffp1023
#endif /* !DBL_MAX */

#ifndef HUGE_VAL

/**
 * @def HUGE_VAL
 * @brief Huge value.
 */
#define HUGE_VAL DBL_MAX
#endif /* !HUGE_VAL */

/*
 * MAXs and MINs
 */

/**
 * @def INT8_MAX
 * @brief 8-bit integer maximum.
 */
#define INT8_MAX  0x7f

/**
 * @def INT16_MAX
 * @brief 16-bit integer maximum.
 */
#define INT16_MAX 0x7fff

/**
 * @def INT32_MAX
 * @brief 32-bit integer maximum.
 */
#define INT32_MAX 0x7fffffff

/**
 * @def INT64_MAX
 * @brief 64-bit integer maximum.
 */
#define INT64_MAX 0x7fffffffffffffffL

/**
 * @def UINT8_MAX
 * @brief 8-bit unsigned integer maximum.
 */
#define UINT8_MAX  0xff

/**
 * @def UINT16_MAX
 * @brief 16-bit unsigned integer maximum.
 */
#define UINT16_MAX 0xffff

/**
 * @def UINT32_MAX
 * @brief 32-bit unsigned integer maximum.
 */
#define UINT32_MAX 0xffffffff

/**
 * @def UINT64_MAX
 * @brief 64-bit unsigned integer maximum.
 */
#define UINT64_MAX 0xffffffffffffffffUL

/**
 * @def UTYPE_MAXVAL(x)
 * @brief MAX value of given unsigned variable type.
 *
 * @param x Value.
 */
#define UTYPE_MAXVAL(x) ((typeof(x)) UINT64_MAX)


#endif /* !CONFIG_ARCH_SIM */


#endif /* __MCUBE_MCUBE_LIMITS_H__ */
