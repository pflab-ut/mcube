/**
 * @file include/mcube/c_header.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_C_HEADER_H__
#define __MCUBE_MCUBE_C_HEADER_H__

/**
 * @def _GNU_SOURCE
 * @brief Definition for GNU source.
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif /* _GNU_SOURCE */

#ifndef __ASSEMBLY__


#if CONFIG_ARCH_SIM
#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <sys/stat.h>
#else

/**
 * @def offsetof(type, elem)
 * calculate offset of element of type.
 *
 * @param type Type.
 * @param elem Element.
 */
#define offsetof(type, elem)  ((unsigned long) &((type *) 0)->elem)
#endif /* CONFIG_ARCH_SIM */

#include <stdarg.h>
#include <stdbool.h>

#if defined(ENABLE_FPU)
#include <float.h>
#endif /* ENABLE_FPU */

#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_MCUBE_C_HEADER_H__ */
