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

void *memset(void *s, int c, size_t n);
void *memsetw(void *s, int c, size_t n);
void *memsetd(void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
size_t strlen(const char *s);
int strcmp(const char *s, const char *t);
int strncmp(const char *s1, const char *s2, size_t n);
char *strcpy(char *s, const char *t);
char *strncpy(char *s, const char *t, size_t);
char *strcat(char *s, const char *t);
char *strncat(char *s, const char *t, size_t n);
char *strchr(const char *s, int c);
char *strtok(char *str, const char *delim);

#else
#error "Unknown Architecture"
#endif /* CONFIG_ARCH_SIM */

#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_STRING_H__ */
