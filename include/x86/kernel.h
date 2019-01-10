/**
 * @file include/x86/kernel.h
 *
 * @author Hiroyuki Chishiro
 */
/*_
 * Copyright (c) 2018 Hirochika Asai <asai@jar.jp>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __MCUBE_X86_KERNEL_H__
#define __MCUBE_X86_KERNEL_H__

#ifndef __ASSEMBLY__


/* Variable length argument support */
typedef __builtin_va_list va_list;
#define va_start(ap, last)      __builtin_va_start((ap), (last))
#define va_arg                  __builtin_va_arg
#define va_end(ap)              __builtin_va_end(ap)
#define va_copy(dest, src)      __builtin_va_copy((dest), (src))
#define alloca(size)            __builtin_alloca((size))


#define HZ      100


void panic(const char *);

#define kassert(cond)        do {                                       \
        char buf[4096];                                                 \
        if ( !(cond) ) {                                                \
            ksnprintf(buf, 4096, "Assertion failed. %s:%d",             \
                      __FILE__, __LINE__);                              \
            panic(buf);                                                 \
        }                                                               \
    } while( 0 )

/* Defined in arch/x86_64/asm.S */
void * kmemset(void *, int, size_t);
int kmemcmp(void *, void *, size_t);
int kmemcpy(void *__restrict, void *__restrict, size_t);

/* Defined in kernel.c */
int kstrcmp(const char *, const char *);
int kstrncmp(const char *, const char *, size_t);
char * kstrcpy(char *, const char *);
char * kstrncpy(char *, const char *, size_t);
size_t kstrlcpy(char *, const char *, size_t);

/* Defined in strfmt.c */
int kvsnprintf(char *, size_t, const char *, va_list);
int ksnprintf(char *, size_t, const char *, ...);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_KERNEL_H__ */

