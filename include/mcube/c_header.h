/**
 * @file include/mcube/c_header.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_C_HEADER_H__
#define __MCUBE_MCUBE_C_HEADER_H__

#define _GNU_SOURCE

#ifndef __ASSEMBLY__


#if CONFIG_ARCH_SIM
#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#else
//#define offsetof(T, F) ((unsigned int)((char *)&((T *) 0L)->F - (char *) 0L))
#define offsetof(type, elem)  ((unsigned long) &((type *) 0)->elem)
#endif /* CONFIG_ARCH_SIM */

#include <stdarg.h>
#include <stdbool.h>

#if defined(ENABLE_FPU)
#include <float.h>
#endif /* ENABLE_FPU */

#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_MCUBE_C_HEADER_H__ */
