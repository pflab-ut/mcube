/**
 * @file include/debug/regs_debug.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_DEBUG_REGS_DEBUG_H__
#define	__MCUBE_DEBUG_REGS_DEBUG_H__

/* Never include this file directly. Include <debug/debug.h> instead. */

#ifndef __ASSEMBLY__
#if CONFIG_DEBUG


#if CONFIG_ARCH_SIM

#define pdebug_registers(ptr) do {							\
  } while (0)

#elif CONFIG_ARCH_X86

#define pdebug_registers(ptr) do {							\
  print("RCX: %lx     R8: %lx    R13: %lx\n"  \
        "RDX: %lx     R9: %lx    R14: %lx\n"   \
        "RBP: %lx    R10: %lx    R15: %lx\n",           \
        regs->rax, regs->rsi, regs->r11,                \
        regs->rbx, regs->rdi, regs->r12,                \
        regs->rcx, regs->r8, regs->r13,                 \
        regs->rdx, regs->r9, regs->r14,                 \
        regs->rbp, regs->r10, regs->r15);               \
  } while (0)

      
#elif CONFIG_ARCH_ARM

#define pdebug_registers(ptr) do {							\
  } while (0)

#elif CONFIG_ARCH_AXIS

#define pdebug_registers(ptr) do {							\
  } while (0)

#else
#error "Unknown Architecture"
#endif

#else
#define pdebug_registers(ptr)


#endif
#endif /* !__ASSEMBLY__ */

#endif	/* __MCUBE_DEBUG_REGS_DEBUG_H__ */
