/**
 * @file lib/buffer_dumper.c
 *
 * @author Hiroyuki Chishiro
 */
/*
 * BufferDumper Class - Log messages (and bufs) to custom output devices
 *
 * Copyright (C) 2013 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#include <mcube/mcube.h>

/*
 * Possible functions for the printer:
 * - printk(fmt, ...)
 * - print_uart(fmt, ...)
 * - Below null printer
 */


/*
 * Possible functions for the buffer-dump formatter:
 */

/*
 * Print @given_buf, with length of @len bytes, in the format:
 *
 *  $ od --format=x1 --address-radix=none --output-duplicates
 *
 */
void buf_hex_dump(struct buffer_dumper *dumper, void *given_buf, uint len)
{
  unsigned int bytes_perline = 16, n = 0;
  uint8_t *buf = (uint8_t *) given_buf;

  for (uint i = 0; i < len; i++) {
    dumper->pr(" ");

    if (buf[i] < 0x10) {
      dumper->pr("0");
    }

    dumper->pr("%x", buf[i]);

    n++;

    if (n == bytes_perline || i == len - 1) {
      dumper->pr("\n");
      n = 0;
    }
  }
}

/*
 * Print @given_buf as ASCII text.
 */
void buf_char_dump(struct buffer_dumper *dumper, void *given_buf, uint len)
{
  char *buf = (char *) given_buf;

  for (uint i = 0; i < len; i++) {
    dumper->pr("%c", buf[i]);
  }
}

/*
 * NULL buffer printer. Useful for ignoring big debugging dumps, etc.
 */
void buf_null_dump(__unused struct buffer_dumper *dumper,
                   __unused void *given_buf, __unused uint len)
{
}

/**
 * Public interface:
 */
void printbuf(struct buffer_dumper *dumper, void *buf, uint len)
{
  assert(dumper);
  dumper->formatter(dumper, buf, len);
}
