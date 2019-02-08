/**
 * @file include/mcube/types.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_TYPES_H__
#define __MCUBE_MCUBE_TYPES_H__

#ifndef __ASSEMBLY__


#if CONFIG_ARCH_SIM
#include <sys/types.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>


#elif CONFIG_ARCH_X86 || CONFIG_ARCH_ARM || CONFIG_ARCH_AXIS


/** 8-bit integer data type. */
typedef signed char int8_t;
/** 16-bit integer data type. */
typedef signed short int16_t;
/** 32-bit integer data type. */
typedef signed int int32_t;
/** 64-bit integer data type. */
typedef signed long long int64_t;

/** 8-bit unsigned integer data type. */
typedef unsigned char uint8_t;
/** 16-bit unsigned integer data type. */
typedef unsigned short uint16_t;
/** 32-bit unsigned integer data type. */
typedef unsigned int uint32_t;
/** 64-bit unsigned integer data type. */
typedef unsigned long long uint64_t;

/** Unsigned integer data type. */
typedef unsigned long size_t;
/** Integer data type. */
typedef long ssize_t;

/** 32-bit unsigned integer data type. */
typedef uint32_t uint;

/** Interger pointer type. */
typedef long int intptr_t;

/** Unsigned interger pointer type. */
typedef unsigned long int uintptr_t;

typedef uint64_t clock_t;

#else
#error "Unknown Architecture"
#endif /* CONFIG_ARCH_SIM || CONFIG_ARCH_X86 */



#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_MCUBE_TYPES_H__ */
