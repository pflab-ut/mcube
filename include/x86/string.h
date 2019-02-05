/**
 * @file include/x86/string.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_STRING_H__
#define __MCUBE_X86_STRING_H__

/*
 * Standard C string definitions
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#ifndef __ASSEMBLY__

//size_t strlen(const char *str);
size_t strnlen(const char *str, int n);
//char *strncpy(char *dst, const char *src, size_t n);
//int strncmp(const char *s1, const char *s2, int n);


//int memcmp(const void *s1, const void *s2, size_t len);

#if    STRING_TESTS
void string_run_tests(void);
#else
static void __unused string_run_tests(void) { }
#endif

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_STRING_H__ */

