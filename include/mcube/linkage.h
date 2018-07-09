/**
 * @file include/mcube/linkage.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_LINKAGE_H__
#define __MCUBE_MCUBE_LINKAGE_H__

#define SYMBOL_NAME(name) name

#define SYMBOL_NAME_LABEL(name) name##:

#define ENTRY(name)					\
  .globl name;                \
	SYMBOL_NAME_LABEL(name)


#define END(name)																\
	.size name, .-name


#define ASM_SIZE_DIRECTIVE(name) .size name,.-name;



#if CONFIG_ARCH_X86
#define asmlinkage __attribute__((regparm(0)))
#else
#define asmlinkage
#endif



#endif /* __MCUBE_MCUBE_LINKAGE_H__ */
