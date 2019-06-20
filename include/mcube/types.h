/**
 * @file include/mcube/types.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_TYPES_H__
#define __MCUBE_MCUBE_TYPES_H__

#ifndef __ASSEMBLY__


#if !CONFIG_ARCH_SIM

/**
 * @typedef int8_t
 * @brief 8-bit integer data type.
 */
typedef signed char int8_t;

/**
 * @typedef int16_t
 * @brief 16-bit integer data type.
 */
typedef signed short int16_t;

/**
 * @typedef int32_t
 * @brief 32-bit integer data type.
 */
typedef signed int int32_t;

/**
 * @typedef int64_t
 * @brief 64-bit integer data type.
 */
typedef signed long long int64_t;

/**
 * @typedef uint8_t
 * @brief 8-bit unsigned integer data type.
 */
typedef unsigned char uint8_t;

/**
 * @typedef uint16_t
 * @brief 16-bit unsigned integer data type.
 */
typedef unsigned short uint16_t;

/**
 * @typedef uint32_t
 * @brief 32-bit unsigned integer data type.
 */
typedef unsigned int uint32_t;

/**
 * @typedef uint64_t
 * @brief 64-bit unsigned integer data type.
 */
typedef unsigned long long uint64_t;

/**
 * @typedef size_t
 * @brief Unsigned integer data type.
 */
typedef unsigned long size_t;

/**
 * @typedef ssize_t
 * @brief Integer data type.
 */
typedef long ssize_t;

/**
 * @typedef uint
 * @brief 32-bit unsigned integer data type.
 */
typedef uint32_t uint;

/**
 * @typedef intptr_t
 * @brief Interger pointer type.
 */
typedef long int intptr_t;

/**
 * @typedef uintptr_t
 * @brief Unsigned interger pointer type.
 */
typedef unsigned long int uintptr_t;

/**
 * @typedef clock_t
 * @param Clock type.
 */
typedef uint64_t clock_t;

#endif /* !CONFIG_ARCH_SIM */


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_MCUBE_TYPES_H__ */
