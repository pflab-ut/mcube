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

#include <mcube/mcube.h>

/*
 * kstrcmp
 */
int
kstrcmp(const char *s1, const char *s2)
{
    size_t i;
    int diff;

    i = 0;
    while ( s1[i] != '\0' || s2[i] != '\0' ) {
        diff = s1[i] - s2[i];
        if ( diff ) {
            return diff;
        }
        i++;
    }

    return 0;
}

/*
 * kstrncmp
 */
int
kstrncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;
    int diff;

    i = 0;
    while ( (s1[i] != '\0' || s2[i] != '\0') && i < n ) {
        diff = s1[i] - s2[i];
        if ( diff ) {
            return diff;
        }
        i++;
    }

    return 0;
}

/*
 * kstrcpy
 */
char *
kstrcpy(char *dst, const char *src)
{
    size_t i;

    i = 0;
    while ( src[i] != '\0' ) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = src[i];

    return dst;
}

/*
 * kstrncpy
 */
char *
kstrncpy(char *dst, const char *src, size_t n)
{
    size_t i;

    i = 0;
    while ( src[i] != '\0' && i < n ) {
        dst[i] = src[i];
        i++;
    }
    for ( ; i < n; i++ ) {
        dst[i] = '\0';
    }

    return dst;
}

/*
 * kstrlcpy
 */
size_t
kstrlcpy(char *dst, const char *src, size_t n)
{
    size_t i;

    i = 0;
    while ( src[i] != '\0' && i < n - 1 ) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';

    while ( '\0' != src[i] ) {
        i++;
    }

    return i;
}

