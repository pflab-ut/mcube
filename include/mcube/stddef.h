/**
 * @file include/mcube/stddef.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_MCUBE_STDDEF_H__
#define __MCUBE_MCUBE_STDDEF_H__

#ifndef __ASSEMBLY__

#if CONFIG_ARCH_SIM
#include <stddef.h>

#elif CONFIG_ARCH_X86 || CONFIG_ARCH_ARM || CONFIG_ARCH_AXIS
//#define offsetof(T, F) ((unsigned int)((char *)&((T *)0L)->F - (char *)0L))
#define offsetof(type, elem)  ((unsigned long) &((type *) 0)->elem)
#else
#error "Uknown Architecture"
#endif /* CONFIG_ARCH_SIM */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_MCUBE_STDDEF_H__ */
