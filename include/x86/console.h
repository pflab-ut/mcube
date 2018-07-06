/**
 * @file include/x86/console.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_CONSOLE_H__
#define __MCUBE_X86_CONSOLE_H__


#define CUI_CONSOLE_WIDTH 80
#define CUI_CONSOLE_HEIGHT 25

#define CUI_VRAM_ADDR(cursor_x, cursor_y) \
  (CUI_VRAM_START + CUI_CONSOLE_WIDTH * (cursor_y) * 2 + (cursor_x) * 2)
//  (CUI_VRAM_START + CUI_CONSOLE_WIDTH * (cursor_y) * 2 + (cursor_x) * 2)


#define GUI_CONSOLE_WIDTH 320
#define GUI_CONSOLE_HEIGHT 200



#define COL8_000000 0  /* black */
#define COL8_FF0000 1  /* blue */
#define COL8_00FF00 2  /* green */
#define COL8_FFFF00 3  /* cyan */
#define COL8_0000FF 4  /* red */
#define COL8_FF00FF 5  /* magenta */
#define COL8_00FFFF 6  /* brown */
#define COL8_FFFFFF 7  /* gray */
#define COL8_C6C6C6 8  /* dark gray */
#define COL8_840000 9  /* bright blue */
#define COL8_008400 10 /* bright green */
#define COL8_848400 11 /* bright cyan */
#define COL8_000084 12 /* pink */
#define COL8_840084 13 /* bright magenta */
#define COL8_008484 14 /* yellow */
#define COL8_848484 15 /* white */





#endif	/* __MCUBE_X86_CONSOLE_H__ */
