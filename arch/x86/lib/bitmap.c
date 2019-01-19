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

static bool bit_is_set(uint8_t val, int bit)
{
  if ((val & (1U << bit)) == 0)
    return false;

  return true;
}

/*
 * Find the first set (or clear) bit in the given @buf of
 * @len bytes. @TEST_FUNC is used for testing bit's state.
 */
#define add_search_function(NAME, TEST_FUNC)    \
  int64_t NAME(char *buf, uint len)             \
  {                                             \
    uint bits_per_byte = 8;                     \
                                                \
    for (uint i = 0; i < len; i++)              \
      for (uint j = 0; j < bits_per_byte; j++)  \
        if (TEST_FUNC(buf[i], j))               \
          return (i * 8) + j;                   \
    return -1;                                  \
  }

add_search_function(bitmap_first_set_bit, bit_is_set)
add_search_function(bitmap_first_zero_bit, !bit_is_set)

/*
 * Given a buffer @bit offset, calculate the bit position
 * in terms of a @byte_offset within the buffer, and a
 * @bit_offset within that byte.  @len is the buf length.
 */
static void set_offsets(uint bit, uint *byte_offset, uint *bit_offset, uint len)
{
  /* 8 bits per byte */
  assert(bit < len  * 8);

  *byte_offset = bit / 8;
  *bit_offset  = bit % 8;
}

/*
 * Set given @bit number in buffer.
 */
void bitmap_set_bit(char *buf, uint bit, uint len)
{
  uint byte_offset, bit_offset;

  set_offsets(bit, &byte_offset, &bit_offset, len);
  buf += byte_offset;
  *buf |= (1 << bit_offset);
}

/*
 * Clear given @bit number in buffer.
 */
void bitmap_clear_bit(char *buf, uint bit, uint len)
{
  uint byte_offset, bit_offset;

  set_offsets(bit, &byte_offset, &bit_offset, len);
  buf += byte_offset;
  *buf &= ~(1 << bit_offset);
}

/*
 * Check if given @bit number in buffer is set.
 */
bool bitmap_bit_is_set(char *buf, uint bit, uint len)
{
  uint byte_offset, bit_offset;

  set_offsets(bit, &byte_offset, &bit_offset, len);
  buf += byte_offset;
  return bit_is_set(*buf, bit_offset);
}

/*
 * Check if given @bit number in buffer is clear.
 */
bool bitmap_bit_is_clear(char *buf, uint bit, uint len)
{
  return !bitmap_bit_is_set(buf, bit, len);
}

