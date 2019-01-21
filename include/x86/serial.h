/**
 * @file include/x86/serial.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_SERIAL_H__
#define __MCUBE_X86_SERIAL_H__

/*
 * Serial port, 8250/16550 UART
 *
 * Copyright (C) 2010 Ahmed S. Darwish <darwish.07@gmail.com>
 */

#ifndef __ASSEMBLY__

void serial_init(void);
void serial_write(const char *buf, int len);
void serial_putc(char ch);

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_X86_SERIAL_H__ */
