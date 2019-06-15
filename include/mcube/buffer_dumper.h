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
 * @brief Buffer Dumper.
 */
struct buffer_dumper {
  /**
   * printf()-like method, determining the output device (to VGA, serial)
   */
  int (*pr)(const char *fmt, ...);

  /**
   * formatter: how to dump buffers to output device (using hex, ascii, null)
   */
  void (*formatter)(struct buffer_dumper *dumper, void *buf, uint len);
};


extern struct buffer_dumper serial_char_dumper;
extern struct buffer_dumper vga_hex_dumper;
extern struct buffer_dumper vga_char_dumper;
extern struct buffer_dumper vga_null_dumper;
extern struct buffer_dumper serial_hex_dumper;
extern struct buffer_dumper serial_null_dumper;
extern struct buffer_dumper null_null_dumper;

/**
 * @fn int null_printer(const char *fmt, ...)
 * @brief Possible functions for the printer.
 */
int null_printer(const char *fmt, ...);

/**
 * @fn void buf_hex_dump(struct buffer_dumper *dumper, void *given_buf, uint len)
 * @brief Dump buffer by hex.
 * @param dumper Dump buffer.
 * @param given_buf Given buffer.
 * @param len Length.
 */
void buf_hex_dump(struct buffer_dumper *dumper, void *given_buf, uint len);

/**
 * @fn void buf_char_dump(struct buffer_dumper *dumper, void *given_buf, uint len)
 * @brief Dump buffer by char.
 * @param dumper Dump buffer.
 * @param given_buf Given buffer.
 * @param len Length.
 */
void buf_char_dump(struct buffer_dumper *dumper, void *given_buf, uint len);

/**
 * @fn void buf_null_dump(struct buffer_dumper *dumper, void *given_buf, uint len)
 * @brief Dump buffer by null.
 * @param dumper Dump buffer.
 * @param given_buf Given buffer.
 * @param len Length.
 */
void buf_null_dump(__unused struct buffer_dumper *dumper,
                   __unused void *given_buf, __unused uint len);


/**
 * @fn void printbuf(struct buffer_dumper *dumper, void *buf, uint len)
 * @brief Print buffer.
 * @param dumper Dump buffer.
 * @param buf Buffer.
 * @param len Length.
 */
void printbuf(struct buffer_dumper *dumper, void *buf, uint len);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_BUFFER_DUMPER_H__ */
