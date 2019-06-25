/**
 * @file include/x86_64/vga.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_VGA_H__
#define __MCUBE_X86_64_VGA_H__

/*
 * VGA Colors
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 * Reference: 'System BIOS for IBM PCs, compatibles, and EISA computers',
 * second edition, Phoenix press
 */

/**
 * @def VGA_COLOR(bg, fg)
 * @brief VGA color attribute is an 8 bit value, low 4 bits sets
 * foreground color, while the high 4 sets the background.
 *
 * @param bg Background color.
 * @param fg Foreground color.
 */
#define VGA_COLOR(bg, fg)  (((bg) << 4) | (fg))

/**
 * @def VGA_BLACK
 * @brief VGA black.
 */
#define VGA_BLACK 0x0

/**
 * @def VGA_BLUE
 * @brief VGA blue.
 */
#define VGA_BLUE 0x1

/**
 * @def VGA_GREEN
 * @brief VGA green.
 */
#define VGA_GREEN 0x2

/**
 * @def VGA_CYAN
 * @brief VGA cyan.
 */
#define VGA_CYAN 0x3

/**
 * @def VGA_RED
 * @brief VGA red.
 */
#define VGA_RED 0x4

/**
 * @def VGA_MAGENTA
 * @brief VGA magenta.
 */
#define VGA_MAGENTA 0x5

/**
 * @def VGA_BROWN
 * @brief VGA brown.
 */
#define VGA_BROWN 0x6

/**
 * @def VGA_LIGHT_GRAY
 * @brief VGA light gray.
 */
#define VGA_LIGHT_GRAY 0x7

/**
 * @def VGA_GRAY
 * @brief VGA gray.
 */
#define VGA_GRAY 0x8

/**
 * @def VGA_LIGHT_BLUE
 * @brief VGA light blue.
 */
#define VGA_LIGHT_BLUE 0x9

/**
 * @def VGA_LIGHT_GREEN
 * @brief VGA light green.
 */
#define VGA_LIGHT_GREEN 0xa

/**
 * @def VGA_LIGHT_CYAN
 * @brief VGA light cyan.
 */
#define VGA_LIGHT_CYAN 0xb

/**
 * @def VGA_LIGHT_RED
 * @brief VGA light red.
 */
#define VGA_LIGHT_RED 0xc

/**
 * @def VGA_LIGHT_MAGENTA
 * @brief VGA light magenta.
 */
#define VGA_LIGHT_MAGENTA 0xd

/**
 * @def VGA_YELLOW
 * @brief VGA yellow.
 */
#define VGA_YELLOW 0xe

/**
 * @def VGA_WHITE
 * @brief VGA white.
 */
#define VGA_WHITE 0xf

/**
 * @def VGA_COLOR_MAX
 * @brief Max color value, 4 bytes.
 */
#define VGA_COLOR_MAX 0xf

#endif /* __MCUBE_X86_64_VGA_H__ */
