/**
 * @file include/x86/pic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_PIC_H__
#define __MCUBE_X86_PIC_H__

/*
 * The i8259A PIC
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 2.
 */
#ifndef __ASSEMBLY__

void pic_init(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_PIC_H__ */
