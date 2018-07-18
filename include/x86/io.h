/**
 * @file include/x86/io.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_X86_IO_H__
#define	__MCUBE_X86_IO_H__

#ifdef __ASSEMBLY__

.macro putchar c
movb $0x0e, %ah
	 movb $\c, %al
	 movb $0, %bh
	 movb $0xf, %bl
	 int $0x10
.endm

.macro mmio_out32 addr data
	 movl $\data, %eax
	 movl $\addr, %edx
	 movl %eax, (%edx)
.endm


#else


  

/* keyboard and mouse */
#define PORT_KEYDATA 0x0060
#define PORT_KEYSTATUS 0x0064
#define PORT_KEYCMD 0x0064

#define KEYSTATUS_SEND_NOT_READY 0x02

#define KEYCMD_WRITE_MODE 0x60
#define KEYCMD_SENDTO_MOUSE 0xd4
#define KEYCMD_LED 0xed
#define KEYCMD_BUFSIZE 32

#define KEYFIFO_BUFSIZE 32

#define MOUSECMD_ENABLE 0xf4

#define KBC_MODE 0x47

#define KEYDATA_OFFSET 0

#define KEYTABLE_SIZE 128
#define NR_KEYS (2 * KEYTABLE_SIZE)

void enable_key(void);
void disable_key(void);
void init_key(void);
void wait_KBC(void);
void enable_mouse(void);
void disable_mouse(void);
void init_mouse(void);

void set_cursor(uint16_t cur_pos);

void init_console(void);


extern fifo32_struct keycmd;
extern uint32_t keycmd_buf[KEYCMD_BUFSIZE];

extern fifo32_struct keyfifo;
extern uint32_t keyfifo_buf[KEYFIFO_BUFSIZE];


extern char keytable0[KEYTABLE_SIZE];
extern char keytable1[KEYTABLE_SIZE];


#define SCROLL_LOCK (0x1 << 0)
#define NUM_LOCK  (0x1 << 1)
#define CAPS_LOCK (0x1 << 2)

#define LEFT_SHIFT (0x1 << 0)
#define RIGHT_SHIFT (0x1 << 1)


#define IS_BREAK(data) (((data) >> 7) & 0x1)

enum make_break {
	MAKE,
	BREAK,
};

typedef enum make_break make_break;




#endif

#endif	/* __MCUBE_X86_IO_H__ */
