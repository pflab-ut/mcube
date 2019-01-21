/**
 * @file include/mcube/bitops.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_BITOPS_H__
#define __MCUBE_MCUBE_BITOPS_H__

#ifndef __ASSEMBLY__


/*
 * Operations on a Bitmap
 *
 * Copyright (C) 2012 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2.
 *
 * We assume little-endian ordering of bytes in the bitmap, that is:
 * item #0  is represented by first  byte bit #0
 * item #7  is represented by first  byte bit #7
 * item #8  is represented by second byte bit #0
 * item #15 is represented by second byte bit #7
 * .. and so on.
 *
 * FIXME: Optimize this using x86-64 ops.
 */

#include <mcube/mcube.h>

static inline bool bit_is_set8(uint8_t val, int bit)
{
  if ((val & (1U << bit)) == 0) {
    return false;
  }

  return true;
}

/*
 * Find the first set (or clear) bit in the given @buf of
 * @len bytes. @TEST_FUNC is used for testing bit's state.
 */
static inline int64_t find_first_bit8(char *buf, uint len)
{
  uint bits_per_byte = 8;

  for (uint i = 0; i < len; i++) {
    for (uint j = 0; j < bits_per_byte; j++) {
      if (bit_is_set8(buf[i], j)) {
        return 8 * i + j;
      }
    }
  }
  return -1;
}

static inline int64_t find_first_zero_bit8(char *buf, uint len)
{
  uint bits_per_byte = 8;

  for (uint i = 0; i < len; i++) {
    for (uint j = 0; j < bits_per_byte; j++) {
      if (!bit_is_set8(buf[i], j)) {
        return 8 * i + j;
      }
    }
  }
  return -1;
}


/*
 * Given a buffer @bit offset, calculate the bit position
 * in terms of a @byte_offset within the buffer, and a
 * @bit_offset within that byte.  @len is the buf length.
 */
static inline void set_offsets8(uint bit, uint *byte_offset, uint *bit_offset, uint len)
{
  /* 8 bits per byte */
  assert(bit < len  * 8);

  *byte_offset = bit / 8;
  *bit_offset  = bit % 8;
}

/*
 * Set given @bit number in buffer.
 */
static inline void set_bit8(char *buf, uint bit, uint len)
{
  uint byte_offset, bit_offset;

  set_offsets8(bit, &byte_offset, &bit_offset, len);
  buf += byte_offset;
  *buf |= (1 << bit_offset);
}

/*
 * Clear given @bit number in buffer.
 */
static inline void clear_bit8(char *buf, uint bit, uint len)
{
  uint byte_offset, bit_offset;

  set_offsets8(bit, &byte_offset, &bit_offset, len);
  buf += byte_offset;
  *buf &= ~(1 << bit_offset);
}

/*
 * Check if given @bit number in buffer is set.
 */
static inline bool bitmap_bit_is_set(char *buf, uint bit, uint len)
{
  uint byte_offset, bit_offset;

  set_offsets8(bit, &byte_offset, &bit_offset, len);
  buf += byte_offset;
  return bit_is_set8(*buf, bit_offset);
}

/*
 * Check if given @bit number in buffer is clear.
 */
static inline bool bitmap_bit_is_clear(char *buf, uint bit, uint len)
{
  return !bitmap_bit_is_set(buf, bit, len);
}



/* find first bit */
static inline uint32_t ffb32(uint32_t bitmap)
{
  //  PDEBUG("ffs(): bitmap = %x\n", bitmap);
  //  asm volatile("bsf %1, %0" :"=r" (bitmap) :"rm" (bitmap));
  int num = 0;

  if ((bitmap & 0xffff) == 0) {
    num += 16;
    bitmap >>= 16;
  }
  if ((bitmap & 0xff) == 0) {
    num += 8;
    bitmap >>= 8;
  }
  if ((bitmap & 0xf) == 0) {
    num += 4;
    bitmap >>= 4;
  }
  if ((bitmap & 0x3) == 0) {
    num += 2;
    bitmap >>= 2;
  }
  if ((bitmap & 0x1) == 0) {
    num += 1;
  }
  return num;
}


static inline uint32_t find_first_bit32(uint32_t *b, int nr)
{
  int i;
  for (i = 0; i < nr; i++) {
    if (b[i]) {
      return 32 * i + ffb32(b[i]);
    }
  }
  return 32 * nr;
}

static inline uint32_t find_first_zero_bit32(uint32_t *b, int nr)
{
  int i;
  for (i = 0; i < nr; i++) {
    if (~b[i]) {
      return 32 * i + ffb32(~b[i]);
    }
  }
  return 32 * nr;
}


/* find last bit */
static inline uint32_t flb32(uint32_t bitmap)
{
  int num = 31;

  if (!(bitmap & (~0ul << 16))) {
    num -= 16;
    bitmap <<= 16;
  }
  if (!(bitmap & (~0ul << 24))) {
    num -= 8;
    bitmap <<= 8;
  }
  if (!(bitmap & (~0ul << 28))) {
    num -= 4;
    bitmap <<= 4;
  }
  if (!(bitmap & (~0ul << 30))) {
    num -= 2;
    bitmap <<= 2;
  }
  if (!(bitmap & (~0ul << 31)))
    num -= 1;
  return num;
}

static inline uint32_t find_last_bit32(uint32_t *b, int nr)
{
  int i;
  for (i = nr - 1; i >= 0; i--) {
    if (b[i]) {
      return 32 * i + flb32(b[i]);
    }
  }
  return 32 * nr;
}


static inline void set_bit32(volatile uint32_t *addr, int nr)
{
  int mask;
  unsigned long flags;

  addr += nr >> 5;
  mask = 1 << (nr & 0x1f);
  save_local_irq(&flags);
  //  PDEBUG("addr = %x mask = %x\n", addr, mask);
  *addr |= mask;
  restore_local_irq(&flags);
}


static inline void clear_bit32(volatile uint32_t *addr, int nr)
{
  int mask;
  unsigned long flags;

  addr += nr >> 5;
  mask = 1 << (nr & 0x1f);
  save_local_irq(&flags);
  //  PDEBUG("addr = %x mask = %x\n", addr, mask);
  *addr &= ~mask;
  restore_local_irq(&flags);
}

static inline void change_bit32(volatile uint32_t *addr, int nr)
{
  int mask;
  unsigned long flags;

  addr += nr >> 5;
  mask = 1 << (nr & 0x1f);
  save_local_irq(&flags);
  *addr ^= mask;
  restore_local_irq(&flags);
}

static inline int test_and_set_bit32(volatile uint32_t *addr, int nr)
{
  int mask, retval;
  unsigned long flags;

  addr += nr >> 5;
  mask = 1 << (nr & 0x1f);
  save_local_irq(&flags);
  retval = (mask & *addr) != 0;
  *addr |= mask;
  restore_local_irq(&flags);

  return retval;
}

static inline int test_and_clear_bit32(volatile uint32_t *addr, int nr)
{
  int mask, retval;
  unsigned long flags;

  addr += nr >> 5;
  mask = 1 << (nr & 0x1f);
  save_local_irq(&flags);
  retval = (mask & *addr) != 0;
  *addr &= ~mask;
  restore_local_irq(&flags);

  return retval;
}

static inline int test_and_change_bit32(volatile uint32_t *addr, int nr)
{
  int mask, retval;
  unsigned long flags;

  addr += nr >> 5;
  mask = 1 << (nr & 0x1f);
  save_local_irq(&flags);
  retval = (mask & *addr) != 0;
  *addr ^= mask;
  restore_local_irq(&flags);

  return retval;
}

/* find first bit */
static inline uint64_t ffb64(uint64_t bitmap)
{
  //  PDEBUG("ffs(): bitmap = %x\n", bitmap);
  //  asm volatile("bsf %1, %0" :"=r" (bitmap) :"rm" (bitmap));
  int num = 0;

  if ((bitmap & 0xffffffffffffffff) == 0) {
    num += 64;
    bitmap = 0;
  }
  if ((bitmap & 0xffffffff) == 0) {
    num += 32;
    bitmap >>= 32;
  }
  if ((bitmap & 0xffff) == 0) {
    num += 16;
    bitmap >>= 16;
  }
  if ((bitmap & 0xff) == 0) {
    num += 8;
    bitmap >>= 8;
  }
  if ((bitmap & 0xf) == 0) {
    num += 4;
    bitmap >>= 4;
  }
  if ((bitmap & 0x3) == 0) {
    num += 2;
    bitmap >>= 2;
  }
  if ((bitmap & 0x1) == 0) {
    num += 1;
  }
  return num;
}

static inline uint64_t find_first_bit64(uint64_t *b, int nr)
{
  int i;
  for (i = 0; i < nr; i++) {
    if (b[i]) {
      return 64 * i + ffb64(b[i]);
    }
  }
  return NR_PRIORITIES;
}

static inline uint64_t find_first_zero_bit64(uint64_t *b, int nr)
{
  int i;
  for (i = 0; i < nr; i++) {
    if (~b[i]) {
      return 64 * i + ffb64(~b[i]);
    }
  }
  return NR_PRIORITIES;
}



/* find last bit */
static inline uint64_t flb64(uint64_t bitmap)
{
  int num = 63;

  if (!(bitmap & 0xffffffff00000000)) {
    num -= 32;
    bitmap <<= 32;
  }
  if (!(bitmap & 0xffffffffffff0000)) {
    num -= 16;
    bitmap <<= 16;
  }
  if (!(bitmap & 0xffffffffffffff00)) {
    num -= 8;
    bitmap <<= 8;
  }
  if (!(bitmap & 0xfffffffffffffff0)) {
    num -= 4;
    bitmap <<= 4;
  }
  if (!(bitmap & 0xfffffffffffffffc)) {
    num -= 2;
    bitmap <<= 2;
  }
  if (!(bitmap & 0xfffffffffffffffe)) {
    num -= 1;
  }
  return num;
}

static inline uint64_t find_last_bit64(uint64_t *b, int nr)
{
  int i;
  for (i = nr - 1; i >= 0; i--) {
    if (b[i]) {
      return 64 * i + flb64(b[i]);
    }
  }
  return NR_PRIORITIES;
}

static inline uint64_t find_last_zero_bit64(uint64_t *b, int nr)
{
  int i;
  for (i = nr - 1; i >= 0; i--) {
    if (~b[i]) {
      return 64 * i + flb64(b[i]);
    }
  }
  return NR_PRIORITIES;
}

static inline void set_bit64(volatile uint64_t *addr, int nr)
{
  int mask;
  unsigned long flags;

  addr += nr >> 6;
  mask = 1 << (nr & 0x3f);
  save_local_irq(&flags);
  //  PDEBUG("addr = %x mask = %x\n", addr, mask);
  *addr |= mask;
  restore_local_irq(&flags);
}


static inline void clear_bit64(volatile uint64_t *addr, int nr)
{
  int mask;
  unsigned long flags;

  addr += nr >> 6;
  mask = 1 << (nr & 0x3f);
  save_local_irq(&flags);
  //  PDEBUG("addr = %x mask = %x\n", addr, mask);
  *addr &= ~mask;
  restore_local_irq(&flags);
}

static inline void change_bit64(volatile uint64_t *addr, int nr)
{
  int mask;
  unsigned long flags;

  addr += nr >> 6;
  mask = 1 << (nr & 0x3f);
  save_local_irq(&flags);
  *addr ^= mask;
  restore_local_irq(&flags);
}

static inline int test_and_set_bit64(volatile uint64_t *addr, int nr)
{
  int mask, retval;
  unsigned long flags;

  addr += nr >> 6;
  mask = 1 << (nr & 0x3f);
  save_local_irq(&flags);
  retval = (mask & *addr) != 0;
  *addr |= mask;
  restore_local_irq(&flags);

  return retval;
}

static inline int test_and_clear_bit64(volatile uint64_t *addr, int nr)
{
  int mask, retval;
  unsigned long flags;

  addr += nr >> 6;
  mask = 1 << (nr & 0x3f);
  save_local_irq(&flags);
  retval = (mask & *addr) != 0;
  *addr &= ~mask;
  restore_local_irq(&flags);

  return retval;
}

static inline int test_and_change_bit64(volatile uint64_t *addr, int nr)
{
  int mask, retval;
  unsigned long flags;

  addr += nr >> 6;
  mask = 1 << (nr & 0x3f);
  save_local_irq(&flags);
  retval = (mask & *addr) != 0;
  *addr ^= mask;
  restore_local_irq(&flags);

  return retval;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_BITOPS_H__ */
