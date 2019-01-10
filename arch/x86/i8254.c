/**
 * @file arch/x86/i8254.c
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

#include <mcube/mcube.h>


/*
 * i8254_start_timer -- start i8254 timer
 */
void i8254_start_timer(int hz)
{
  int counter;

  /* Start the timer of counter 0 with the specified rate */
  counter = I8254_HZ / hz;
  outb(I8254_REG_CTRL, I8254_CTRL_RL_16BIT | I8254_CTRL_SQUAREWAVE);
  outb(I8254_REG_CNTR0, counter & 0xff);
  outb(I8254_REG_CNTR0, counter >> 8);
}

/*
 * i8254_stop_timer -- stop i8254 timer
 */
void i8254_stop_timer(void)
{
  /* Reset to the BIOS default */
  outb(I8254_REG_CTRL, 0x30);
  outb(I8254_REG_CNTR0, 0x0);
  outb(I8254_REG_CNTR0, 0x0);
}
