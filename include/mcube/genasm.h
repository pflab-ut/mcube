/**
 * @file include/mcube/genasm.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_MCUBE_GENASM_H__
#define	__MCUBE_MCUBE_GENASM_H__

#define	show_me_in_asm(mark)	\
	do{ asm volatile(".globl "mark"\n"mark":"); }while(0);

#define	stringify(x...)	#x
#define	ASMMACRO(x, args...)	asm volatile (stringify(x) args)

/* this suppresses gcc warning (shan't abuse however) */
#define	discard_arg(arg)	asm("" : : "r"(arg))

#define	gcc_flush_registers()	asm volatile ("" : : :"memory")

#endif /* __MCUBE_MCUBE_GENASM_H__ */

