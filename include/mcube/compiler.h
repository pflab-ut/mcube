/**
 * @file include/mcube/compiler.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_COMPILER_H__
#define __MCUBE_MCUBE_COMPILER_H__

#ifndef __ASSEMBLY__

/* Optimization barrier */
#define barrier() asm volatile("":::"memory")

/*
 * Versions of the ppc64 compiler before 4.1 had a bug where use of
 * RELOC_HIDE could trash r30. The bug can be worked around by changing
 * the inline assembly constraint from =g to =r, in this particular
 * case either is valid.
 */
#define RELOC_HIDE(ptr, off)                        \
  ({ unsigned long __ptr;                           \
    asm volatile("" : "=r"(__ptr) : "0"(ptr));      \
    (typeof(ptr)) (__ptr + (off)); })


//#define inline    inline    __attribute__((always_inline))
#define __inline__  __inline__  __attribute__((always_inline))
#define __inline  __inline  __attribute__((always_inline))
#define __deprecated      __attribute__((deprecated))
#define __packed      __attribute__((packed))
#define __weak        __attribute__((weak))
#define __naked        __attribute__((naked))
#define __noreturn      __attribute__((noreturn))
#define __pure        __attribute__((pure))
#define __aligned(x)      __attribute__((aligned(x)))
//#define __printf(a,b)      __attribute__((format(printf,a,b)))
#define  noinline      __attribute__((noinline))
//#define __attribute_pure__    __attribute__((pure))
#ifdef __attribute_const__
#undef __attribute_const__
#endif
#define __attribute_const__    __attribute__((__const__))


#if CONFIG_COMPILER_CLANG

#define FALLTHROUGH /* fall through */

#elif CONFIG_COMPILER_GCC

#if __GNUC__ >= 7
#define FALLTHROUGH __attribute__((fallthrough))
#else
#define FALLTHROUGH /* fall through */
#endif

#else
#error "Unknown Compiler"
#endif /* CONFIG_COMPILER_CLANG */

#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_MCUBE_COMPILER_H__ */

