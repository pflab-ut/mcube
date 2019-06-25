/**
 * @file include/x86_64/sections.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_SECTIONS_H__
#define __MCUBE_X86_64_SECTIONS_H__

/*
 * Section boundaries symbols
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#ifndef __ASSEMBLY__

/*
 * ELF section boundaries are provided by the kernel
 * linker script kernel.ld. We could've declared them as
 * char -- not char[] -- and use the symbols as &symbol,
 * but the current method is more convenient.
 */

/**
 * @var __text_start[]
 * @brief Text start.
 */
extern char __text_start[];

/**
 * @var __text_end[]
 * @brief Text end.
 */
extern char __text_end[];

/**
 * @var __data_start[]
 * @brief Data start.
 */
extern char __data_start[];

/**
 * @var __data_end[]
 * @brief Data end.
 */
extern char __data_end[];

/**
 * @var __bss_start[]
 * @brief BSS start.
 */
extern char __bss_start[];

/**
 * @var __bss_end[]
 * @brief BSS end.
 */
extern char __bss_end[];

/**
 * @var __kernel_end[]
 * @brief Kernel end.
 */
extern char __kernel_end[];

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_SECTIONS_H__ */
