/**
 * @file include/mcube/string.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_STRING_H__
#define	__MCUBE_MCUBE_STRING_H__

#ifndef __ASSEMBLY__

#if CONFIG_ARCH_SIM

#include <string.h>


#elif CONFIG_ARCH_X86 || CONFIG_ARCH_ARM || CONFIG_ARCH_AXIS

extern void *memset(void *s, int c, size_t n);
extern int memcmp(const void *s1, const void *s2, size_t n);
extern void *memcpy(void *dest, const void *src, size_t n);
extern void *memmove(void *dest, const void *src, size_t n);
extern size_t strlen(const char *s);
extern int strcmp(const char *s, const char *t);
extern int strncmp(const char *s1, const char *s2, size_t n);
extern char *strcpy(char *s, const char *t);
extern char *strncpy(char *s, const char *t, size_t);
extern char *strcat(char *s, const char *t);
extern char *strncat(char *s, const char *t, size_t n);
extern char *strchr(const char *s, int c);
extern char *strtok(char *str, const char *delim);

#else
#error "Unknown Architecture"
#endif /* CONFIG_ARCH_SIM */

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_STRING_H__ */
