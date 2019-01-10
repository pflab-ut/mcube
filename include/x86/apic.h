/**
 * @file include/x86/apic.h
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

#ifndef	__MCUBE_X86_APIC_H__
#define	__MCUBE_X86_APIC_H__

#define MSR_APIC_BASE           0x1b


#define APIC_LAPIC_ID                   0x020
#define APIC_SIVR                       0x0f0
#define APIC_ICR_LOW                    0x300
#define APIC_ICR_HIGH                   0x310
#define APIC_LVT_TMR                    0x320
#define APIC_INITTMR                    0x380
#define APIC_CURTMR                     0x390
#define APIC_TMRDIV                     0x3e0


/* ICR delivery mode */
#define APIC_ICR_FIXED                  0x00000000
#define APIC_ICR_INIT                   0x00000500
#define APIC_ICR_STARTUP                0x00000600
/* ICR status */
#define APIC_ICR_SEND_PENDING           0x00001000
/* ICR level */
#define APIC_ICR_LEVEL_ASSERT           0x00004000
/* ICR destination */
#define APIC_ICR_DEST_NOSHORTHAND       0x00000000
#define APIC_ICR_DEST_SELF              0x00040000
#define APIC_ICR_DEST_ALL_INC_SELF      0x00080000
#define APIC_ICR_DEST_ALL_EX_SELF       0x000c0000
/* LVT */
#define APIC_LVT_DISABLE                0x10000
#define APIC_LVT_ONESHOT                0x00000000
#define APIC_LVT_PERIODIC               0x00020000
#define APIC_LVT_TSC_DEADLINE           0x00040000
/* Timer */
#define APIC_TMRDIV_X1                  0xb
#define APIC_TMRDIV_X2                  0x0
#define APIC_TMRDIV_X4                  0x1
#define APIC_TMRDIV_X8                  0x2
#define APIC_TMRDIV_X16                 0x3
#define APIC_TMRDIV_X32                 0x8
#define APIC_TMRDIV_X64                 0x9
#define APIC_TMRDIV_X128                0xa

#ifndef __ASSEMBLY__

void lapic_send_init_ipi(void);
void lapic_send_startup_ipi(uint8_t);
void lapic_bcast_fixed_ipi(uint8_t);
void lapic_send_fixed_ipi(int, uint8_t);
void lapic_set_timer(uint32_t, int);
uint64_t lapic_stop_and_read_timer(void);
void lapic_start_timer(uint64_t, uint64_t, uint8_t);
void lapic_stop_timer(void);
void ioapic_init(void);
void ioapic_map_intr(uint64_t, uint64_t, uint64_t);

#endif /* __ASSEMBLY__ */


#endif /* __MCUBE_X86_APIC_H__ */

