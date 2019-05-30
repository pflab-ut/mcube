/**
 * @file include/debug/regs_debug.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_DEBUG_REGS_DEBUG_H__
#define __MCUBE_DEBUG_REGS_DEBUG_H__

#ifndef __ASSEMBLY__

#if CONFIG_OPTION_DEBUG

#if CONFIG_ARCH_SIM

#define pdebug_registers(regs) do {             \
  } while (0)

#elif CONFIG_ARCH_X86_64

#define pdebug_registers(regs) do {             \
  } while (0)


#elif CONFIG_ARCH_AARCH64_RASPI3 || CONFIG_ARCH_AARCH64_SYNQUACER

#define pdebug_registers(regs) do {                               \
    for (int i = 0; i < 32; i += 2) {                             \
      printk("x%02d: 0x%016lx  x%02d: 0x%016lx\n",                \
             i, (regs).gpr[i], i + 1, (regs).gpr[i + 1]);         \
    }                                                             \
  } while (0)

#elif CONFIG_ARCH_AXIS

#define pdebug_registers(regs) do {                                \
    for (int i = 0; i < 32; i += 2) {                              \
      printk("r%02d: 0x%016lx  r%02d: 0x%016lx\n",                 \
             i, (regs).gpr_regs.regs[i],                           \
             i + 1, (regs).gpr_regs.regs[i + 1]);                  \
    }                                                              \
  } while (0)

#else
#error "Unknown Architecture"
#endif

#else
#define pdebug_registers(regs)

#endif /* CONFIG_OPTION_DEBUG */

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_DEBUG_REGS_DEBUG_H__ */
