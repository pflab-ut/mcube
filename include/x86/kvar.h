/**
 * @file include/x86/kvar.h
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

#ifndef __MCUBE_X86_KVAR_H__
#define __MCUBE_X86_KVAR_H__

#ifndef __ASSEMBLY__

/*
 * Kernel variable
 */
typedef struct {
    phys_memory_t phys;
    memory_t mm;
    memory_slab_allocator_t slab;
    /* Architecture specific data */
    pgt_t pgt;
} kvar_t;

/* For kernel variables */
#define KVAR                    ((kvar_t *) KVAR_ADDR)

/* Pointer to per-core data (flags, tss, stack); 256 */
#define KVAR_KDATA_PER_CORE     ((void *) KDATA_PER_CORE)

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_KVAR_H__ */

