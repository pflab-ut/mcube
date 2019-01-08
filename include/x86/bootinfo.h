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

#ifndef _ADVOS_BOOTINFO_H
#define _ADVOS_BOOTINFO_H

/* Definition of the boot information region */
#define BI_START_ADDR           0x8000
#define BI_SIZE                 0x1000

/* Address to store drive information (1 byte) */
#define BI_DRIVE_ADDR           0x8000

/* Address to store # of entries of the memory map (2 bytes) */
#define BI_MM_NENT_ADDR         0x87f0
/* Address to store entries of the memory map (24-byte entries) */
#define BI_MM_TABLE_ADDR        0x8800
/* Maximum allowed size of the memory map region */
#define BI_MM_SIZE              0x0800

#endif

