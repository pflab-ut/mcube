/**
 * @file include/mcube/stdlib.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_STDLIB_H__
#define __MCUBE_MCUBE_STDLIB_H__

#ifndef __ASSEMBLY__

/**
 * @typedef int (*sortcmp)(const void *a, const void *b)
 * @brief Typedef of sort compare function.
 */
typedef int (*sortcmp)(const void *a, const void *b);

/**
 * @fn void *kmalloc(size_t size)
 * @brief allocate kernel memory.
 *
 * @param size Size.
 * @return Pointer to allocated memory.
 */
void *kmalloc(size_t size);

/**
 * @fn void kfree(void *addr)
 * @brief free kernel memory.
 *
 * @param ptr Pointer to allocated memory.
 */
void kfree(void *ptr);


char *itoa(int val, char *buf, int radix);
char *ltoa(long val, char *buf, int radix);
char *ultoa(unsigned long val, char *buf, int radix);


#if !CONFIG_ARCH_SIM

long strtol(const char *cp, char **endp, int base);
unsigned long strtoul(const char *cp, char **endp, int base);
void exit(int status);

void *sbrk(intptr_t increment);

void *malloc(size_t size);
void *calloc(size_t num, size_t nsize);
void *realloc(void *block, size_t size);
void free(void *ptr);


void qsort(void *base, size_t num, size_t size, sortcmp cmp);


#endif /* CONFIG_ARCH_SIM */


#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_STDLIB_H__ */
