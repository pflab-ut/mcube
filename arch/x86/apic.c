/**
 * @file arch/x86/apic.c
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
 * lapic_base_addr -- get the base address for local APIC access by reading
 * APIC machine specific register (MSR)
 */
uint64_t lapic_base_addr(void)
{
  uint32_t reg;
  uint64_t msr;
  uint64_t apic_base;

  /* Read IA32_APIC_BASE register */
  msr = rdmsr(MSR_APIC_BASE);
  apic_base = msr & 0xfffffffffffff000ULL;

  /* Enable APIC at spurious interrupt vector register: default vector 0xff */
  reg = mfrd32(apic_base + APIC_SIVR);
  reg |= 0x100;       /* Bit 8: APIC Software Enable/Disable */
  mfwr32(apic_base + APIC_SIVR, reg);

  return apic_base;
}

/*
 * lapic_id -- get the local APIC ID of this CPU core
 */
int lapic_id(void)
{
  uint32_t reg;
  uint64_t apic_base;

  apic_base = lapic_base_addr();
  reg = *(uint32_t *)(apic_base + APIC_LAPIC_ID);

  return reg >> 24;
}

/*
 * lapic_send_init_ipi -- send INIT IPI
 */
void lapic_send_init_ipi(void)
{
  uint32_t icrl;
  uint32_t icrh;
  uint64_t apic_base;

  apic_base = lapic_base_addr();

  icrl = mfrd32(apic_base + APIC_ICR_LOW);
  icrh = mfrd32(apic_base + APIC_ICR_HIGH);

  icrl = (icrl & ~0x000cdfff) | APIC_ICR_INIT | APIC_ICR_DEST_ALL_EX_SELF;
  icrh = (icrh & 0x000fffff);

  mfwr32(apic_base + APIC_ICR_HIGH, icrh);
  mfwr32(apic_base + APIC_ICR_LOW, icrl);
}

/*
 * lapic_send_startup_ipi -- send start up IPI
 */
void lapic_send_startup_ipi(uint8_t vector)
{
  uint32_t icrl;
  uint32_t icrh;
  uint64_t apic_base;

  apic_base = lapic_base_addr();

  do {
    icrl = mfrd32(apic_base + APIC_ICR_LOW);
    icrh = mfrd32(apic_base + APIC_ICR_HIGH);
    /* Wait until it's idle */
  } while (icrl & (APIC_ICR_SEND_PENDING));

  icrl = (icrl & ~0x000cdfff) | APIC_ICR_STARTUP | APIC_ICR_DEST_ALL_EX_SELF
    | vector;
  icrh = (icrh & 0x000fffff);

  mfwr32(apic_base + APIC_ICR_HIGH, icrh);
  mfwr32(apic_base + APIC_ICR_LOW, icrl);
}

/*
 * lapic_bcast_fixed_ipi -- broadcast fixed IPI
 */
void lapic_bcast_fixed_ipi(uint8_t vector)
{
  uint32_t icrl;
  uint32_t icrh;
  uint64_t apic_base;

  apic_base = lapic_base_addr();

  icrl = mfrd32(apic_base + APIC_ICR_LOW);
  icrh = mfrd32(apic_base + APIC_ICR_HIGH);

  icrl = (icrl & ~0x000cdfff) | APIC_ICR_FIXED | APIC_ICR_DEST_ALL_EX_SELF
    | vector;
  icrh = (icrh & 0x000fffff);

  mfwr32(apic_base + APIC_ICR_HIGH, icrh);
  mfwr32(apic_base + APIC_ICR_LOW, icrl);
}

/*
 * lapic_send_fixed_ipi -- send a fixed IPI to the specified destination
 */
void lapic_send_fixed_ipi(int dst, uint8_t vector)
{
  uint32_t icrl;
  uint32_t icrh;
  uint64_t apic_base;

  apic_base = lapic_base_addr();

  icrl = mfrd32(apic_base + APIC_ICR_LOW);
  icrh = mfrd32(apic_base + APIC_ICR_HIGH);

  icrl = (icrl & ~0x000cdfff) | APIC_ICR_FIXED | APIC_ICR_DEST_NOSHORTHAND
    | vector;
  icrh = (icrh & 0x000fffff) | ((uint32_t) dst << 24);

  mfwr32(apic_base + APIC_ICR_HIGH, icrh);
  mfwr32(apic_base + APIC_ICR_LOW, icrl);
}

/*
 * ioapic_init -- initialize I/O APIC
 */
void ioapic_init(void)
{
  /* Ensure to disable i8259 PIC */
  outb(0xa1, 0xff);
  outb(0x21, 0xff);
}

/*
 * ioapic_map_intr -- set a map entry of interrupt vector
 */
void ioapic_map_intr(uint64_t intvec, uint64_t tbldst, uint64_t ioapic_base)
{
  uint64_t val;

  /*
   * 63:56    destination field
   * 16       interrupt mask (1: masked for edge sensitive)
   * 15       trigger mode (1=level sensitive, 0=edge sensitive)
   * 14       remote IRR (R/O) (1 if local APICs accept the level interrupts)
   * 13       interrupt input pin polarity (0=high active, 1=low active)
   * 12       delivery status (R/O)
   * 11       destination mode (0=physical, 1=logical)
   * 10:8     delivery mode
   *          000 fixed, 001 lowest priority, 010 SMI, 011 reserved
   *          100 NMI, 101 INIT, 110 reserved, 111 ExtINT
   * 7:0      interrupt vector
   */
  val = intvec;

  sfence();
  *(uint32_t *)(ioapic_base + 0x00) = tbldst * 2 + 0x10;
  sfence();
  *(uint32_t *)(ioapic_base + 0x10) = (uint32_t) val;
  sfence();
  *(uint32_t *)(ioapic_base + 0x00) = tbldst * 2 + 0x10 + 1;
  sfence();
  *(uint32_t *)(ioapic_base + 0x10) = (uint32_t)(val >> 32);
}

/*
 * lapic_set_timer -- set timer
 */
void lapic_set_timer(uint32_t value, int divide)
{
  uint64_t apic_base;

  apic_base = lapic_base_addr();

  /* Disable timer */
  mfwr32(apic_base + APIC_LVT_TMR, APIC_LVT_DISABLE);

  /* Set divide configuration of the timer */
  mfwr32(apic_base + APIC_TMRDIV, divide);

  /* Vector: lvt[18:17] = 00 : oneshot */
  mfwr32(apic_base + APIC_LVT_TMR, 0x0);

  /* Set initial counter and start the timer */
  mfwr32(apic_base + APIC_INITTMR, value);
}

/*
 * lapic_stop_and_read_timer -- stop and read the timer counter
 */
uint64_t lapic_stop_and_read_timer(void)
{
  uint64_t apic_base;
  uint32_t t;

  apic_base = lapic_base_addr();

  /* Disable current timer */
  mfwr32(apic_base + APIC_LVT_TMR, APIC_LVT_DISABLE);

  /* Read current timer */
  t = mfrd32(apic_base + APIC_CURTMR);

  return t;
}

/*
 * Start local APIC timer
 */
void lapic_start_timer(uint64_t busfreq, uint64_t freq, uint8_t vec)
{
  uint64_t apic_base;

  apic_base = lapic_base_addr();

  /* Set counter */
  mfwr32(apic_base + APIC_LVT_TMR, APIC_LVT_PERIODIC | (uint32_t) vec);
  mfwr32(apic_base + APIC_TMRDIV, APIC_TMRDIV_X16);
  mfwr32(apic_base + APIC_INITTMR, (busfreq >> 4) / freq);
}

/*
 * Stop APIC timer
 */
void lapic_stop_timer(void)
{
  uint64_t apic_base;

  apic_base = lapic_base_addr();

  /* Disable timer */
  mfwr32(apic_base + APIC_LVT_TMR, APIC_LVT_DISABLE);
}
