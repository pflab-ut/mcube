/**
 * @file include/x86_64/cpu.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_X86_64_CPU_H__
#define	__MCUBE_X86_64_CPU_H__

/**
 * @def USER_LEVEL
 * @brief User level.
 */
#define USER_LEVEL 0

/**
 * @def KERNEL_LEVEL
 * @brief Kernel level.
 */
#define KERNEL_LEVEL 3

/**
 * @def REGISTER_LENGTH
 * @brief Register length.
 */
#define REGISTER_LENGTH 64

/**
 * @def VENDOR_ID_LENGTH
 * @brief Vendor ID length.
 */
#define VENDOR_ID_LENGTH 12

/**
 * @def CPU_BRAND_LENGTH
 * @brief CPU brand length.
 */
#define CPU_BRAND_LENGTH 48

/**
 * @def REG_LENGTH
 * @brief Register length.
 */
#define REG_LENGTH 64


#ifndef __ASSEMBLY__


// CPU EFLAGS register values
/* 31-22: reserved */

/**
 * @def CPU_EFLAGS_CPUID
 * @brief Able to use CPUID instruction (Pentium+).
 */
#define CPU_EFLAGS_CPUID (1 << 21)

/**
 * @def CPU_EFLAGS_VPENDING
 * @brief Virtual interrupt pending (Pentium+).
 */
#define CPU_EFLAGS_VPENDING (1 << 20)

/**
 * @def CPU_EFLAGS_VINTERRUPT
 * @brief Virtual interrupt flag (Pentium+).
 */
#define CPU_EFLAGS_VINTERRUPT (1 << 19)

/**
 * @def CPU_EFLAGS_ALIGNCHECK
 * @brief Alignment check  (486SX+ only).
 */
#define CPU_EFLAGS_ALIGNCHECK (1 << 18)

/**
 * @def CPU_EFLAGS_V8086
 * @brief Virtual 8086 mode flag (386+ only).
 */
#define CPU_EFLAGS_V8086 (1 << 17)

/**
 * @def CPU_EFLAGS_RESUME
 * @brief Resume flag (386+ only).
 */
#define CPU_EFLAGS_RESUME (1 << 16)

/**
 * @def EFLAGS_BIT15
 * @brief reserved, always 1 on 8086 and 186, always 0 on later models.
 */
#define EFLAGS_BIT15           (0 << 15)

/**
 * @def CPU_EFLAGS_NESTED
 * @brief Nested task flag (286+ only), always 1 on 8086 and 186.
 */
#define CPU_EFLAGS_NESTED (1 << 14)

/**
 * @def CPU_EFLAGS_IOPL0
 * @brief I/O privilege level 0 (286+ only), always 1 on 8086 and 186
 */
#define CPU_EFLAGS_IOPL0 (1 << 13)

/**
 * @def CPU_EFLAGS_IOPL1
 * @brief I/O privilege level 1 (286+ only), always 1 on 8086 and 186
 */
#define CPU_EFLAGS_IOPL1 (1 << 12)

/**
 * @def CPU_EFLAGS_OVERFLOW
 * @brief Overflow flag.
 */
#define CPU_EFLAGS_OVERFLOW (1 << 11)

/**
 * @def CPU_EFLAGS_DIRECTION
 * @brief Direction flag.
 */
#define CPU_EFLAGS_DIRECTION (1 << 10)

/**
 * @def CPU_EFLAGS_INTERRUPT
 * @brief Interrupt enable flag.
 */
#define CPU_EFLAGS_INTERRUPT (1 << 9)

/**
 * @def CPU_EFLAGS_TRAP
 * @brief Trap flag (single step).
 */
#define CPU_EFLAGS_TRAP (1 << 8)

/**
 * @def CPU_EFLAGS_SIGN
 * @brief Sign flag.
 */
#define CPU_EFLAGS_SIGN (1 << 7)

/**
 * @def CPU_EFLAGS_ZERO
 * @brief Zero flag.
 */
#define CPU_EFLAGS_ZERO (1 << 6)

/**
 * @def CPU_EFLAGS_BIT5
 * @brief Always 0.
 */
#define CPU_EFLAGS_BIT5 (0 << 5)

/**
 * @def CPU_EFLAGS_ADJUST
 * @brief Adjust flag.
 */
#define CPU_EFLAGS_ADJUST (1 << 4)

/**
 * @def CPU_EFLAGS_BIT3
 * @brief Always 0.
 */
#define CPU_EFLAGS_BIT3 (0 << 3)

/**
 * @def CPU_EFLAGS_PARITY
 * @brief Parity flag.
 */
#define CPU_EFLAGS_PARITY (1 << 2)

/**
 * @def CPU_EFLAGS_BIT1
 * @brief Always 1.
 */
#define CPU_EFLAGS_BIT1 (1 << 1)

/**
 * @def CPU_EFLAGS_CARRY
 * @brief Carry flag.
 */
#define CPU_EFLAGS_CARRY (1 << 0)

/**
 * @def CPU_EFLAGS_BIT
 * @brief Always bits.
 */
#define CPU_EFLAGS_BIT (CPU_EFLAGS_BIT1 | CPU_EFLAGS_BIT3 | CPU_EFLAGS_BIT5 | CPU_EFLAGS_BIT15)

/**
 * @def INIT_CPU_EFLAGS
 * @brief initialize CPU EFLAGS.
 */
#define INIT_CPU_EFLAGS (CPU_EFLAGS_IF | CPU_EFLAGS_BIT)


/**
 * @fn static inline unsigned long get_cpu_id(void)
 * @brief get CPU ID.
 *
 * @return CPU ID.
 */
static inline unsigned long get_cpu_id(void)
{
  cpuid_info_t cinfo;
  cpuid(0x1, &cinfo);
  return (cinfo.rbx & 0x0f000000) >> 24;
}

/**
 * @fn static inline bool is_syscall_available(cpuid_info_t *cinfo)
 * @brief Is syscall available?
 *
 * @param cinfo CPU ID information.
 * @return True if syscal is available.
 */
static inline bool is_syscall_available(cpuid_info_t *cinfo)
{
  return (cinfo->rdx >> 11) & 1;
}

/**
 * @fn static inline bool is_long_mode_available(cpuid_info_t *cinfo)
 * @brief Is long mode available?
 *
 * @param cinfo CPU ID information.
 * @return True if long mode is available.
 */
static inline bool is_long_mode_available(cpuid_info_t *cinfo)
{
  return (cinfo->rdx >> 29) & 1;
}

/**
 * @fn void init_syscall(void)
 * @brief initialize syscall.
 */
void init_syscall(void);

/**
 * @fn void change_ring0_to_ring3(uint64_t selector)
 * @brief change ring 0 to ring 3.
 *
 * @param selector Selector.
 */
void change_ring0_to_ring3(uint64_t selector);


#endif /* __ASSEMBLY__ */


#endif	/* __MCUBE_X86_64_CPU_H__ */
