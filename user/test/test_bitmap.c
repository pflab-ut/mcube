/**
 * @file user/user/test_bitmap.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>


/*
 * Below testcases will be very useful only when the naive code
 * above get substituted with optimized x86-64 ops!
 */

int test_bitmap(void)
{
  char *buf;
  uint buflen_bytes, buflen_bits;
  int64_t bit;

  buflen_bytes = 4096;
  buflen_bits = buflen_bytes * 8;
  buf = kmalloc(buflen_bytes);

  /* An all-zeroes buffer */
  memset(buf, 0, buflen_bytes);
  bit = find_first_bit8(buf, buflen_bytes);
  if (bit != -1) {
    panic("Zeroed buf at 0x%lx, but first_set_bit returned "
          "bit #%u as set!", buf, bit);
  }
  bit = find_first_zero_bit8(buf, buflen_bytes);
  if (bit != 0) {
    panic("Zeroed buf at 0x%lx, but first_zero_bit returned "
          "bit #%u instead of bit #0!", buf, bit);
  }
  for (uint i = 0; i < buflen_bits; i++) {
    if (bitmap_bit_is_set(buf, i, buflen_bytes)) {
      panic("Zeroed buf at 0x%lx, but bit_is_set?() returned "
            "bit #%u as set!", buf, i);
    }
    assert(bitmap_bit_is_clear(buf, i, buflen_bytes));
  }

  /* Mixed buffer */
  for (uint i = 0; i < buflen_bits; i++) {
    memset(buf, 0, buflen_bytes);
    set_bit8(buf, i, buflen_bytes);
    bit = find_first_bit8(buf, buflen_bytes);
    if (bit != i) {
      panic("Zeroed buf at 0x%lx, then set its #%u bit, "
            "but first_set_bit returned #%u as set instead!",
            buf, i, bit);
    }
    if (bitmap_bit_is_clear(buf, i, buflen_bytes)) {
      panic("Zeroed buf at 0x%lx, then set its #%u bit, "
            "but bit_is_clear?() returned the bit as clear!",
            buf, i);
    }
    assert(bitmap_bit_is_set(buf, i, buflen_bytes));
  }

  /* An all-ones buffer */
  memset(buf, 0xff, buflen_bytes);
  for (uint i = 0; i < buflen_bytes; i++) {
    bit = find_first_bit8(buf + i, buflen_bytes - i);
    if (bit != 0) {
      panic("buf at 0x%lx+%d have all bits set, first_set_bit "
            "should have returned bit #0, but #%u was "
            "returned instead!", buf, i, bit);
    }
    bit = find_first_zero_bit8(buf + i, buflen_bytes - i);
    if (bit != -1) {
      panic("buf at 0x%lx+%d have all bits set, "
            "first_zero_bit should've returned no bit as "
            "clear, but bit #%u was returned!", buf, i, bit);
    }
    if (bitmap_bit_is_clear(buf + i, 0, buflen_bytes - i)) {
      panic("buf at 0x%lx+%d have all bits set, bit_is_clear? "
            "should have returned bit #0 as not clear", buf, i);
    }
    assert(bitmap_bit_is_set(buf + i, 0, buflen_bytes - i));
  }

  printk("%s: Sucess!\n", __func__);
  kfree(buf);
  return 0;
}

