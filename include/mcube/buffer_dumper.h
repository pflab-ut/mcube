/**
 * @file include/mcube/buffer_dumper.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_BUFFER_DUMPER_H__
#define __MCUBE_MCUBE_BUFFER_DUMPER_H__

#ifndef __ASSEMBLY__

/*
 * BufferDumper Class - Log messages (and bufs) to custom output devices
 *
 * Copyright (C) 2013 Ahmed S. Darwish <darwish.07@gmail.com>
 */

/**
 * @struct buffer_dumper
 * @brief Buffer Dumper
 * @pr: printf()-like method, determining the output device (to VGA, serial)
 * @formatter: how to dump buffers to output device (using hex, ascii, null)
 */
struct buffer_dumper {
  int (*pr)(const char *fmt, ...);
  void (*formatter)(struct buffer_dumper *dumper, void *buf, uint len);
};

extern struct buffer_dumper serial_char_dumper;

/*
 * Possible functions for the printer:
 * - printk(fmt, ...)
 * - prints(fmt, ...)
 * - and below null printer
 */
int null_printer(const char *fmt, ...);

/*
 * Possible functions for the formatter:
 */
void buf_hex_dump(struct buffer_dumper *dumper, void *given_buf, uint len);
void buf_char_dump(struct buffer_dumper *dumper, void *given_buf, uint len);
void buf_null_dump(__unused struct buffer_dumper *dumper,
                   __unused void *given_buf, __unused uint len);


void printbuf(struct buffer_dumper *, void *buf, uint len);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_BUFFER_DUMPER_H__ */
