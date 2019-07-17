/**
 * @file include/mcube/compiler.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_COMPILER_H__
#define __MCUBE_MCUBE_COMPILER_H__


#if defined(__cplusplus) && !defined(__ASSEMBLY__)

/**
 * begin extern "C".
 */
#define BEGIN_EXTERN_C extern "C" {

/**
 * end extern "C".
 */
#define END_EXTERN_C }

#else


/**
 * begin extern "C".
 */
#define BEGIN_EXTERN_C

/**
 * end extern "C".
 */
#define END_EXTERN_C

#endif /* __cplusplus && !__ASSEMBLY__ */

#ifndef __ASSEMBLY__

/*
 * Compiler extensions shorthands.
 */


/**
 * @def __aligned(x)
 * @brief Alignment attribute of @a x.
 * @param x Variable.
 */
#define __aligned(x) __attribute__((aligned(x)))


#ifndef __always_inline
/**
 * @def __always_inline
 * @brief Always inline attribute.
 *
 * Mark the 'always_inline' attributed function as C99
 * 'inline' cause the attribute by itself is worthless.
 * It's "for functions declared inline" -- GCC manual */
#define __always_inline inline __attribute__((always_inline))
#endif

#ifdef __attribute_pure__
#undef __attribute_pure__
#endif

/**
 * @def __attribute_pure__
 * @brief Pure attribute.
 */
#define __attribute_pure__ __attribute__((pure))

#ifdef __attribute_const__
#undef __attribute_const__
#endif

/**
 * @def __attribute_const__
 * @brief Const attribute.
 */
#define __attribute_const__ __attribute__((__const__))

/**
 * @def __deprecated
 * @brief Deprecated attribute.
 */
#define __deprecated __attribute__((deprecated))

/**
 * @def __error(msg)
 * @brief Error message attribute.
 */
#define __error(msg) __attribute__((error(msg)))

/**
 * @def inline
 * @brief Inline attribute.
 */
#define inline inline __attribute__((always_inline))

/**
 * @def __inline__
 * @brief Inline attribute.
 */
#define __inline__  __inline__  __attribute__((always_inline))

/**
 * @def __inline
 * @brief Inline attribute.
 */
#define __inline  __inline  __attribute__((always_inline))

/**
 * @def __likely(exp)
 * @brief Likely expression builtin.
 */
#define __likely(exp)  __builtin_expect((exp), 1)

/**
 * @def __naked
 * @brief Named attribute.
 */
#define __naked __attribute__((naked))

/**
 * @def noinline
 * @brief Noinline attribute.
 */
#define  noinline      __attribute__((noinline))

/**
 * @def __noinline
 * @brief Noinline attribute.
 */
#define __noinline  __attribute__((noinline))

/**
 * @def __noreturn
 * @brief Noreturn attribute.
 */
#define __noreturn      __attribute__((noreturn))

/**
 * @def __packed
 * @brief Packed attribute.
 */
#define __packed      __attribute__((packed))

//#define __printf(a,b)      __attribute__((format(printf,a,b)))

/**
 * @def __pure
 * @brief Pure attribute.
 */
#define __pure        __attribute__((pure))

/**
 * @def __pure_const
 * @brief Const attribute.
 */
#define __pure_const  __attribute__((const))

/**
 * @def __uninitialized(x)
 * @brief Uninitialized.
 */
#define __uninitialized(x)  (x) = (x)

/**
 * @def __unlikely(exp)
 * @brief Unlikely expression builtin.
 */
#define __unlikely(exp)  __builtin_expect((exp), 0)

/**
 * @def __unused
 * @brief Unused attribute.
 */
#define __unused __attribute__((__unused__))

/**
 * @def __used
 * @brief Used attribute.
 */
#define __used __attribute__((__used__))

/**
 * @def __weak
 * @brief Weak attribute.
 */
#define __weak __attribute__((weak))


#if CONFIG_COMPILER_CLANG

/**
 * @def FALLTHROUGH
 * @brief fall through.
 */
#define FALLTHROUGH

#elif CONFIG_COMPILER_GCC

#if __GNUC__ >= 7

/**
 * @def FALLTHROUGH
 * @brief fall through.
 */
#define FALLTHROUGH __attribute__((fallthrough))
#else

/**
 * @def FALLTHROUGH
 * @brief fall through.
 */
#define FALLTHROUGH
#endif

#else
#error "Unknown Compiler"
#endif /* CONFIG_COMPILER_CLANG */

#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_MCUBE_COMPILER_H__ */

