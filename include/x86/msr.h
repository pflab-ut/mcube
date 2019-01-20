/**
 * @file include/x86/msr.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_MSR_H__
#define __MCUBE_X86_MSR_H__

/*
 * 64-bit Model-Specific Registers
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2.
 */

#ifndef __ASSEMBLY__

/*
 * Write the content of @val using the %edx:%eax register pair to
 * the @msr MSR.
 */
static inline void write_msr(uint32_t msr, uint64_t val)
{
  uint32_t high = val >> 32;
  uint32_t low = val & 0xffffffff;

  asm volatile (
    "wrmsr"
    :
    : "a"(low), "d"(high), "c"(msr));
}

/*
 * Return the content of the @msr MSR (%ecx) using the %edx:%eax
 * register pair.
 */
static inline uint64_t read_msr(uint32_t msr)
{
  uint32_t high, low;

  asm volatile (
    "rdmsr"
    : "=&a"(low), "=&d"(high)
    : "c"(msr));

  return ((uint64_t)high << 32) + low;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_MSR_H__ */
