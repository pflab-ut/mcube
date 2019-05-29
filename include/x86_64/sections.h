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

extern char __text_start[];
extern char __text_end[];

extern char __data_start[];
extern char __data_end[];

extern char __bss_start[];
extern char __bss_end[];

extern char __kernel_end[];

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_SECTIONS_H__ */
