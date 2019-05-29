/**
 * @file include/x86_64/pic.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_PIC_H__
#define __MCUBE_X86_64_PIC_H__

/*
 * The i8259A PIC
 *
 * Copyright (C) 2009 Ahmed S. Darwish <darwish.07@gmail.com>
 */
#ifndef __ASSEMBLY__

/*
 * AT+ standard PIC ports
 */
#define PIC_MASTER_CMD  0x20
#define PIC_SLAVE_CMD   0xa0
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_DATA  0xa1

/*
 * Where the slave PIC is connected
 */
#define PIC_CASCADE_IRQ 2

void PIC_handler(void);

void pic_init(void);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_64_PIC_H__ */
