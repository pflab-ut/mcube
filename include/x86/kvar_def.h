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

#ifndef _ADVOS_KVAR_DEF_H
#define _ADVOS_KVAR_DEF_H

/* For kernel variables */
#define KVAR_ADDR       0xc0068000
#define KVAR_SIZE       0x00001000

/* Trampoline: 0x70 (0x70000) */
#define TRAMPOLINE_VEC          0x70
#define TRAMPOLINE_MAX_SIZE     0x1000

/* Temporary GDT for application processors */
#define AP_GDT_CODE64_SEL       0x08    /* Code64 selector */
#define AP_GDT_DATA64_SEL       0x10    /* Data64 selector */
#define AP_GDT_CODE32_SEL       0x18    /* Code32 selector */
#define AP_GDT_DATA32_SEL       0x20    /* Data32 selector */
#define AP_GDT_CODE16_SEL       0x28    /* Code16 selector */
#define AP_GDT_DATA16_SEL       0x30    /* Data16 selector */

#define KERNEL_LMAP             0x100000000

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
