/**
 * @file include/mcube/byteops.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_BYTEOPS_H__
#define __MCUBE_MCUBE_BYTEOPS_H__

#ifndef __ASSEMBLY__

/**
 * swap byte order of 32-bits data.
 *
 * @param x 32-bits data.
 */
#define swap32(x) ((uint32_t)((((uint32_t)(x) & 0x000000ff) << 24) |  \
                              (((uint32_t)(x) & 0x0000ff00) <<  8) |  \
                              (((uint32_t)(x) & 0x00ff0000) >>  8) |  \
                              (((uint32_t)(x) & 0xff000000) >> 24) ))

/**
 * swap byte order of 16-bits data.
 *
 * @param x 16-bits data.
 */
#define swap16(x) ((uint16_t)((((uint16_t)(x) & 0x00ff) <<  8) |  \
                              (((uint16_t)(x) & 0xff00) >>  8))

/**
 * pack four 8-bits data into 32-bits data.
 *
 * @param ui32 32-bits destination data.
 * @param ui8hh 8-bits bits[31-24] data.
 * @param ui8hl 8-bits bits[23-16] data.
 * @param ui8lh 8-bits bits[15-8] data.
 * @param ui8ll 8-bits bits[7-0] data.
 */
#define pack32(ui32, ui8hh, ui8hl, ui8lh, ui8ll) {  \
    uint16_t ui16h, ui16l;                          \
    ui16h = ((uint16_t)(ui8hh) << 8) | (ui8hl);     \
    ui16l = ((uint16_t)(ui8lh) << 8) | (ui8ll);     \
    (ui32) = ((uint32_t) ui16h << 16) | ui16l;      \
  }

/**
 * unpack four 8-bits data from 32-bits data.
 *
 * @param ui32 32-bits source data.
 * @param ui8hh 8-bits bits[31-24] data.
 * @param ui8hl 8-bits bits[23-16] data.
 * @param ui8lh 8-bits bits[15-8] data.
 * @param ui8ll 8-bits bits[7-0] data.
 */
#define unpack32(ui32, ui8hh, ui8hl, ui8lh, ui8ll) {  \
    uint16_t ui16h, ui16l;                            \
    ui16h = (uint16_t)((ui32) >> 16);                 \
    ui16l = (uint16_t)(ui32);                         \
    (ui8hh) = (uint8_t)(ui16h >> 8);                  \
    (ui8hl) = (uint8_t)ui16h;                         \
    (ui8lh) = (uint8_t)(ui16l >> 8);                  \
    (ui8ll) = (uint8_t)ui16l;                         \
  }

/**
 * pack two 8-bits data into 16-bits data.
 *
 * @param ui16 16-bits destination data.
 * @param ui8h 8-bits bits[15-8] data.
 * @param ui8l 8-bits bits[7-0] data.
 */
#define pack16(ui16, ui8h, ui8l) {              \
    (ui16) = ((uint16_t) (ui8h) << 8) | (ui8l); \
  }

/**
 * unpack two 8-bits data from 16-bits data.
 *
 * @param ui32 16-bits source data.
 * @param ui8h 8-bits bits[15-8] data.
 * @param ui8l 8-bits bits[7-0] data.
 */
#define unpack16(ui16, ui8h, ui8l) {            \
    (ui8h) = (uint8_t)((ui16) >> 8);            \
    (ui8l) = (uint8_t)(ui16);                   \
  }

/**
 * Long to char shift length
 */
#define LONG_TO_CHAR_SHIFT_WIDTH 24

/**
 * Char to long shift length
 */
#define CHAR_TO_LONG_SHIFT_WIDTH LONG_TO_CHAR_SHIFT_WIDTH

static inline unsigned char long_to_char(unsigned long l)
{
  return (l >> LONG_TO_CHAR_SHIFT_WIDTH);
}

static inline unsigned long char_to_long(unsigned char c)
{
  return (c << CHAR_TO_LONG_SHIFT_WIDTH);
}

static inline unsigned char get_byte(unsigned long l, unsigned char byte)
{
  return (l >> (8 * byte)) & 0xff;
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_BYTEOPS_H__ */
