/**
 * @file include/mcube/stdlib.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_STDLIB_H__
#define __MCUBE_MCUBE_STDLIB_H__

#ifndef __ASSEMBLY__

/**
 * @fn void *kmalloc(size_t size)
 * @brief allocate kernel memory.
 *
 * @param size Size.
 * @return Pointer to allocated memory.
 */
void *kmalloc(size_t size);

/**
 * @fn void kfree(void *ptr)
 * @brief free kernel memory.
 *
 * @param ptr Pointer to allocated memory.
 */
void kfree(void *ptr);

/**
 * @fn char *itoa(int val, char *str, int base)
 * @brief convert an interger to string to the given base.
 *
 * @param val Integer.
 * @param str String.
 * @param base Base.
 * @return Pointer to string.
 */
char *itoa(int val, char *str, int base);

/**
 * @fn char *ltoa(long val, char *str, int base)
 * @brief convert a long interger to string to the given base.
 *
 * @param val Long integer.
 * @param str String.
 * @param base Base.
 * @return Pointer to string.
 */
char *ltoa(long val, char *str, int base);

/**
 * @fn char *ultoa(unsigned long val, char *str, int base)
 * @brief convert an unsigned long interger to string to the given base.
 *
 * @param val Unsigned long integer.
 * @param str String.
 * @param base Base.
 * @return Pointer to string.
 */
char *ultoa(unsigned long val, char *str, int base);


#if !CONFIG_ARCH_SIM

/**
 * @fn long strtol(const char *nptr, char **endp, int base)
 * @brief convert the initial part of the string in @a nptr to a long integer
 * value according to the given @a base.
 *
 * @param nptr String.
 * @param endp Address of the first invalid character.
 * @param base Base.
 * @return Converted long integer.
 */
long strtol(const char *nptr, char **endp, int base);

/**
 * @fn unsigned long strtoul(const char *nptr, char **endp, int base)
 * @brief converts the initial part of the string in @a nptr to an unsigned long
 * int value according to the given @a base.
 *
 * @param nptr String.
 * @param endp Address of the first invalid character.
 * @param base Base.
 * @return Converted unsigned long integer.
 */
unsigned long strtoul(const char *nptr, char **endp, int base);

/**
 * @typedef int (*sortcmp)(const void *a, const void *b)
 * @brief Typedef of sort compare function.
 */
typedef int (*sortcmp)(const void *a, const void *b);


/**
 * @fn void qsort(void *base, size_t nmemb, size_t size, sortcmp cmp)
 * @brief sorts an array with @a nmemb elements of size @a size.
 * The @a base argument points to the start of the array.
 *
 * @param base Base.
 * @param nmemb Number of elements.
 * @param size Size.
 * @param cmp Sort compare function.
 */
void qsort(void *base, size_t nmemb, size_t size, sortcmp cmp);

/**
 * @fn void exit(int status)
 * @brief causes normal process termination.
 *
 * @param status Status.
 */
void exit(int status);


#endif /* !CONFIG_ARCH_SIM */


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_STDLIB_H__ */
