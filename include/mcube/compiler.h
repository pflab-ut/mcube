/**
 * @file include/mcube/compiler.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_COMPILER_H__
#define	__MCUBE_MCUBE_COMPILER_H__

#ifndef __ASSEMBLY__

#if CONFIG_COMPILER_CLANG
#include <mcube/compiler/compiler-clang.h>
#elif CONFIG_COMPILER_GCC
#include <mcube/compiler/compiler-gcc.h>
#else
#error "Unknown Compiler"
#endif /* CONFIG_COMPILER_CLANG */

#endif /* !__ASSEMBLY__ */


#endif /*	__MCUBE_MCUBE_COMPILER_H__ */

