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
 */

#include <mcube/mcube.h>

/**
 * @fn static inline bool bit_is_set8(uint8_t val, int bit)
 * @brief Is bit set by 8-bits?
 * @param val Value.
 * @param bit Bit index.
 * @return True if bit is set.
 */
static inline bool bit_is_set8(uint8_t val, int bit)
{
  if ((val & (1U << bit)) == 0) {
    return false;
  }

  return true;
}

/**
 * @fn static inline int64_t find_first_bit8(char *buf, uint len)
 * @brief find first bit by 8-bits.
 * @param buf Buffer.
 * @param len Length.
 * @return Bit index.
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

/**
 * @fn static inline int64_t find_first_zero_bit8(char *buf, uint len)
 * @brief find first zero bit by 8-bits.
 * @param buf Buffer.
 * @param len Length.
 * @return Bit index.
 */
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


/**
 * @fn static inline void set_offsets8(uint bit, uint *byte_offset, uint *bit_offset,
 *                                     uint len)
 * @brief set offset by 8-bits.
 * @param bit Given bit offset of buffer.
 * @param byte_offset Byte offset.
 * @param bit_offset Bit offset.
 * @param len Buffer length.
 */
static inline void set_offsets8(uint bit, uint *byte_offset, uint *bit_offset,
                                uint len)
{
  /* 8 bits per byte */
  assert(bit < len  * 8);

  *byte_offset = bit / 8;
  *bit_offset  = bit % 8;
}

/**
 * @fn static inline void set_bit8(char *buf, uint bit, uint len)
 * @brief set bit by 8-bits.
 * @param buf Buffer.
 * @param bit Bit.
 * @param len Length.
 */
static inline void set_bit8(char *buf, uint bit, uint len)
{
  uint byte_offset, bit_offset;

  set_offsets8(bit, &byte_offset, &bit_offset, len);
  buf += byte_offset;
  *buf |= (1 << bit_offset);
}

/**
 * @fn static inline void clear_bit8(char *buf, uint bit, uint len)
 * @brief clear bit by 8-bits.
 * @param buf Buffer.
 * @param bit Bit.
 * @param len Length.
 */
static inline void clear_bit8(char *buf, uint bit, uint len)
{
  uint byte_offset, bit_offset;

  set_offsets8(bit, &byte_offset, &bit_offset, len);
  buf += byte_offset;
  *buf &= ~(1 << bit_offset);
}

/**
 * @fn static inline bool bitmap_bit_is_set(char *buf, uint bit, uint len)
 * @brief check if given @a bit number in buffer is set.
 * @param buf Buffer.
 * @param bit Bit.
 * @param len Length.
 * @return True if bit is set.
 */
static inline bool bitmap_bit_is_set(char *buf, uint bit, uint len)
{
  uint byte_offset, bit_offset;

  set_offsets8(bit, &byte_offset, &bit_offset, len);
  buf += byte_offset;
  return bit_is_set8(*buf, bit_offset);
}

/**
 * @fn static inline bool bitmap_bit_is_clear(char *buf, uint bit, uint len)
 * @brief check if given @a bit number in buffer is clear.
 * @param buf Buffer.
 * @param bit Bit.
 * @param len Length.
 * @return True if bit is clear.
 */
static inline bool bitmap_bit_is_clear(char *buf, uint bit, uint len)
{
  return !bitmap_bit_is_set(buf, bit, len);
}


/**
 * @fn static inline uint32_t ffb32(uint32_t bitmap)
 * @brief find first bit in 32-bits.
 * @param bitmap Bitmap.
 * @return Bit index.
 */
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


/**
 * @fn static inline uint32_t find_first_bit32(uint32_t *buf, int nr)
 * @brief find first bit by 32-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 * @return Bit index.
 */
static inline uint32_t find_first_bit32(uint32_t *buf, int nr)
{
  int i;

  for (i = 0; i < nr; i++) {
    if (buf[i]) {
      return 32 * i + ffb32(buf[i]);
    }
  }

  return 32 * nr;
}

/**
 * @fn static inline uint32_t find_first_zero_bit32(uint32_t *buf, int nr)
 * @brief find first zero bit by 32-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 * @return Bit index.
 */
static inline uint32_t find_first_zero_bit32(uint32_t *buf, int nr)
{
  int i;

  for (i = 0; i < nr; i++) {
    if (~buf[i]) {
      return 32 * i + ffb32(~buf[i]);
    }
  }

  return 32 * nr;
}


/**
 * @fn static inline uint32_t flb32(uint32_t bitmap)
 * @brief find last bit in 32-bits.
 * @param bitmap Bitmap.
 * @return Bit index.
 */
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

  if (!(bitmap & (~0ul << 31))) {
    num -= 1;
  }

  return num;
}


/**
 * @fn static inline uint32_t find_last_bit32(uint32_t *buf, int nr)
 * @brief find last bit by 32-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 * @return Bit index.
 */
static inline uint32_t find_last_bit32(uint32_t *buf, int nr)
{
  int i;

  for (i = nr - 1; i >= 0; i--) {
    if (buf[i]) {
      return 32 * i + flb32(buf[i]);
    }
  }

  return 32 * nr;
}

/**
 * @fn static inline uint32_t find_last_zero_bit32(uint32_t *buf, int nr)
 * @brief find last zero bit by 32-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 * @return Bit index.
 */
static inline uint32_t find_last_zero_bit32(uint32_t *buf, int nr)
{
  int i;

  for (i = nr - 1; i >= 0; i--) {
    if (~buf[i]) {
      return 32 * i + flb32(buf[i]);
    }
  }

  return 32 * nr;
}

/**
 * @fn static inline void set_bit32(volatile uint32_t *buf, int nr)
 * @brief set bit by 32-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 */
static inline void set_bit32(volatile uint32_t *buf, int nr)
{
  int mask;
  union rflags flags;

  buf += nr >> 5;
  mask = 1 << (nr & 0x1f);
  save_local_irq(&flags);
  //  PDEBUG("buf = %x mask = %x\n", buf, mask);
  *buf |= mask;
  restore_local_irq(&flags);
}


/**
 * @fn static inline void clear_bit32(volatile uint32_t *buf, int nr)
 * @brief clear bit by 32-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 */
static inline void clear_bit32(volatile uint32_t *buf, int nr)
{
  int mask;
  union rflags flags;

  buf += nr >> 5;
  mask = 1 << (nr & 0x1f);
  save_local_irq(&flags);
  //  PDEBUG("buf = %x mask = %x\n", buf, mask);
  *buf &= ~mask;
  restore_local_irq(&flags);
}

/**
 * @fn static inline void change_bit32(volatile uint32_t *buf, int nr)
 * @brief change bit by 32-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 */
static inline void change_bit32(volatile uint32_t *buf, int nr)
{
  int mask;
  union rflags flags;

  buf += nr >> 5;
  mask = 1 << (nr & 0x1f);
  save_local_irq(&flags);
  *buf ^= mask;
  restore_local_irq(&flags);
}

/**
 * @fn static inline int test_and_set_bit32(volatile uint32_t *buf, int nr)
 * @brief test and set bit by 32-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 * @return True if test and set is successful.
 */
static inline int test_and_set_bit32(volatile uint32_t *buf, int nr)
{
  int mask, retval;
  union rflags flags;

  buf += nr >> 5;
  mask = 1 << (nr & 0x1f);
  save_local_irq(&flags);
  retval = (mask & *buf) != 0;
  *buf |= mask;
  restore_local_irq(&flags);

  return retval;
}

/**
 * @fn static inline int test_and_clear_bit32(volatile uint32_t *buf, int nr)
 * @brief test and clear bit by 32-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 * @return True if test and clear is successful.
 */
static inline int test_and_clear_bit32(volatile uint32_t *buf, int nr)
{
  int mask, retval;
  union rflags flags;

  buf += nr >> 5;
  mask = 1 << (nr & 0x1f);
  save_local_irq(&flags);
  retval = (mask & *buf) != 0;
  *buf &= ~mask;
  restore_local_irq(&flags);

  return retval;
}

/**
 * @fn static inline int test_and_change_bit32(volatile uint32_t *buf, int nr)
 * @brief test and change bit by 32-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 * @return True if test and change is successful.
 */
static inline int test_and_change_bit32(volatile uint32_t *buf, int nr)
{
  int mask, retval;
  union rflags flags;

  buf += nr >> 5;
  mask = 1 << (nr & 0x1f);
  save_local_irq(&flags);
  retval = (mask & *buf) != 0;
  *buf ^= mask;
  restore_local_irq(&flags);

  return retval;
}

/**
 * @fn static inline uint64_t ffb64(uint64_t bitmap)
 * @brief find first bit in 64-bits.
 * @param bitmap Bitmap.
 * @return Bit index.
 */
static inline uint64_t ffb64(uint64_t bitmap)
{
  //  PDEBUG("ffs(): bitmap = %x\n", bitmap);
  //  asm volatile("bsf %1, %0" :"=r" (bitmap) :"rm" (bitmap));
  int num = 0;

  if ((bitmap & 0xffffffffffffffffUL) == 0) {
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


/**
 * @fn static inline uint64_t find_first_bit64(uint64_t *b, int nr)
 * @brief find first bit by 64-bits.
 * @param b Buffer.
 * @param nr Number of bytes.
 * @return Bit index.
 */
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

/**
 * @fn static inline uint64_t find_first_zero_bit64(uint64_t *b, int nr)
 * @brief find first zero bit by 64-bits.
 * @param b Buffer.
 * @param nr Number of bytes.
 * @return Bit index.
 */
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



/**
 * @fn static inline uint64_t flb64(uint64_t bitmap)
 * @brief find last bit in 64-bits.
 * @param bitmap Bitmap.
 * @return Bit index.
 */
static inline uint64_t flb64(uint64_t bitmap)
{
  int num = 63;

  if (!(bitmap & 0xffffffff00000000UL)) {
    num -= 32;
    bitmap <<= 32;
  }

  if (!(bitmap & 0xffffffffffff0000UL)) {
    num -= 16;
    bitmap <<= 16;
  }

  if (!(bitmap & 0xffffffffffffff00UL)) {
    num -= 8;
    bitmap <<= 8;
  }

  if (!(bitmap & 0xfffffffffffffff0UL)) {
    num -= 4;
    bitmap <<= 4;
  }

  if (!(bitmap & 0xfffffffffffffffcUL)) {
    num -= 2;
    bitmap <<= 2;
  }

  if (!(bitmap & 0xfffffffffffffffeUL)) {
    num -= 1;
  }

  return num;
}

/**
 * @fn static inline uint64_t find_last_bit64(uint64_t *b, int nr)
 * @brief find last bit by 64-bits.
 * @param b Buffer.
 * @param nr Number of bytes.
 * @return Bit index.
 */
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

/**
 * @fn static inline uint64_t find_last_zero_bit64(uint64_t *b, int nr)
 * @brief find last zero bit by 64-bits.
 * @param b Buffer.
 * @param nr Number of bytes.
 * @return Bit index.
 */
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


/**
 * @fn static inline void set_bit64(volatile uint64_t *buf, int nr)
 * @brief set bit by 64-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 * @return Bit index.
 */
static inline void set_bit64(volatile uint64_t *buf, int nr)
{
  int mask;
  union rflags flags;

  buf += nr >> 6;
  mask = 1 << (nr & 0x3f);
  save_local_irq(&flags);
  //  PDEBUG("buf = %x mask = %x\n", buf, mask);
  *buf |= mask;
  restore_local_irq(&flags);
}


/**
 * @fn static inline void clear_bit64(volatile uint64_t *buf, int nr)
 * @brief clear bit by 64-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 * @return Bit index.
 */
static inline void clear_bit64(volatile uint64_t *buf, int nr)
{
  int mask;
  union rflags flags;

  buf += nr >> 6;
  mask = 1 << (nr & 0x3f);
  save_local_irq(&flags);
  //  PDEBUG("buf = %x mask = %x\n", buf, mask);
  *buf &= ~mask;
  restore_local_irq(&flags);
}


/**
 * @fn static inline void change_bit64(volatile uint64_t *buf, int nr)
 * @brief change bit by 64-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 * @return Bit index.
 */
static inline void change_bit64(volatile uint64_t *buf, int nr)
{
  int mask;
  union rflags flags;

  buf += nr >> 6;
  mask = 1 << (nr & 0x3f);
  save_local_irq(&flags);
  *buf ^= mask;
  restore_local_irq(&flags);
}

/**
 * @fn static inline void test_and_set_bit64(volatile uint64_t *buf, int nr)
 * @brief test and set bit by 64-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 * @return Bit index.
 */
static inline int test_and_set_bit64(volatile uint64_t *buf, int nr)
{
  int mask, retval;
  union rflags flags;

  buf += nr >> 6;
  mask = 1 << (nr & 0x3f);
  save_local_irq(&flags);
  retval = (mask & *buf) != 0;
  *buf |= mask;
  restore_local_irq(&flags);

  return retval;
}

/**
 * @fn static inline void test_and_clear_bit64(volatile uint64_t *buf, int nr)
 * @brief test and clear bit by 64-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 * @return Bit index.
 */
static inline int test_and_clear_bit64(volatile uint64_t *buf, int nr)
{
  int mask, retval;
  union rflags flags;

  buf += nr >> 6;
  mask = 1 << (nr & 0x3f);
  save_local_irq(&flags);
  retval = (mask & *buf) != 0;
  *buf &= ~mask;
  restore_local_irq(&flags);

  return retval;
}

/**
 * @fn static inline void test_and_change_bit64(volatile uint64_t *buf, int nr)
 * @brief test and change bit by 64-bits.
 * @param buf Buffer.
 * @param nr Number of bytes.
 * @return Bit index.
 */
static inline int test_and_change_bit64(volatile uint64_t *buf, int nr)
{
  int mask, retval;
  union rflags flags;

  buf += nr >> 6;
  mask = 1 << (nr & 0x3f);
  save_local_irq(&flags);
  retval = (mask & *buf) != 0;
  *buf ^= mask;
  restore_local_irq(&flags);

  return retval;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_BITOPS_H__ */
