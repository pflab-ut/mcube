/**
 * @file include/mcube/mcubestd.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_MCUBESTD_H__
#define	__MCUBE_MCUBE_MCUBESTD_H__


#define NR_THREADS 16

/* 256 */
//#define STACK_SIZE 0x100
/* 1K */
//#define STACK_SIZE 0x400
/* 4K */
#define STACK_SIZE 0x1000

#define NR_PRIORITIES NR_THREADS

/* each bitmap length is 32 bit */
#define NR_PRIORITY_BITMAPS CEIL(NR_PRIORITIES, 32)

#ifndef	__ASSEMBLY__


#undef NULL
#define NULL ((void *) 0)

#undef EOF
#define EOF -1


#define __stringify_1(x...)   #x
#define __stringify(x...)    __stringify_1(x)

/**
 * @brief Convert flag
 *
 * The conv_flag structure has convert flag information.
 */
struct conv_flag {
	/** Pad. */
	short pad;
	/** Digital number. */
	short digit;
};

typedef struct conv_flag conv_flag;

extern void ustart(int id) __attribute__ ((__noreturn__));
extern void uexit(int status) __attribute__ ((__noreturn__));

extern void init(void);

extern void init_bss(void);

extern int usermain(void);
extern int usertask(void *arg);

#define inf_loop() do {																									\
		printk("%s:%s():%d %s\n", __FILE__, __func__, __LINE__, "inf_loop()"); \
		disable_local_irq();																						\
		for (;;)																														\
			;																																	\
  } while (0)


#endif	/* __ASSEMBLY__ */

#endif	/* __MCUBE_MCUBE_MCUBESTD_H__ */
