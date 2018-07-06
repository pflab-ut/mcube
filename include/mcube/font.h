/**
 * @file include/mcube/font.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_FONT_H__
#define __MCUBE_MCUBE_FONT_H__

#ifndef __ASSEMBLY__

#define NR_HANKAKUS 256

#define HANKAKU_FONT_WIDTH 8
#define HANKAKU_FONT_HEIGHT 16

#define FONTNAME 32

/**
 * @brief Japanese hankaku font
 *
 * The hankaku_font structure has Japanese hankaku font information.
 */
struct hankaku_font {
	/** Font name. */
	char fontname[FONTNAME];
	/** Font information. */
	char font[HANKAKU_FONT_HEIGHT][HANKAKU_FONT_WIDTH];
};

typedef struct hankaku_font hankaku_font;

extern hankaku_font hankaku[NR_HANKAKUS];

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_FONT_H__ */
