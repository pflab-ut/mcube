/**
 * @file include/aarch64/irq.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AARCH64_IRQ_H__
#define __MCUBE_AARCH64_IRQ_H__

/**
 * @def AARCH64_DAIF_FIQ
 * @brief Aarch64 PSTATE {DAIF} Fast Interrupt reQeust (FIQ).
 */
#define AARCH64_DAIF_FIQ (0x1 << 0)

/**
 * @def AARCH64_DAIF_IRQ
 * @brief Aarch64 PSTATE {DAIF} IRQ.
 */
#define AARCH64_DAIF_IRQ (0x1 << 1)

/**
 * @def AARCH64_DAIF_SERROR
 * @brief Aarch64 PSTATE {DAIF} SError.
 */
#define AARCH64_DAIF_SERROR (0x1 << 2)

/**
 * @def AARCH64_DAIF_DEBUG_EXCEPTION
 * @brief Aarch64 PSTATE {DAIF} Exception.
 */
#define AARCH64_DAIF_DEBUG_EXCEPTION (0x1 << 3)

/**
 * @def S_FRAME_SIZE
 * @brief Size of all saved registers.
 */
#define S_FRAME_SIZE 272

/**
 * @def S_X0
 * @brief Offset of x0 register in saved stack frame.
 */
#define S_X0 0

/**
 * @def SYNC_INVALID_EL1t
 * @brief Sync invalid EL1t.
 */
#define SYNC_INVALID_EL1t 0

/**
 * @def IRQ_INVALID_EL1t
 * @brief IRQ invalid EL1t.
 */
#define IRQ_INVALID_EL1t 1

/**
 * @def FIQ_INVALID_EL1t
 * @brief FIQ invalid EL1t.
 */
#define FIQ_INVALID_EL1t 2

/**
 * @def ERROR_INVALID_EL1t
 * @brief Error invalid EL1t.
 */
#define ERROR_INVALID_EL1t 3

/**
 * @def SYNC_INVALID_EL1h
 * @brief Sync invalid EL1h.
 */
#define SYNC_INVALID_EL1h 4

/**
 * @def IRQ_INVALID_EL1h
 * @brief IRQ invalid EL1h.
 */
#define IRQ_INVALID_EL1h 5

/**
 * @def FIQ_INVALID_EL1h
 * @brief FIQ invalid EL1h.
 */
#define FIQ_INVALID_EL1h 6

/**
 * @def ERROR_INVALID_EL1h
 * @brief Error invalid EL1h.
 */
#define ERROR_INVALID_EL1h 7

/**
 * @def SYNC_INVALID_EL0_64
 * @brief Sync invalid EL0 in AArch64.
 */
#define SYNC_INVALID_EL0_64 8

/**
 * @def IRQ_INVALID_EL0_64
 * @brief IRQ invalid EL0 in AArch64.
 */
#define IRQ_INVALID_EL0_64 9

/**
 * @def FIQ_INVALID_EL0_64
 * @brief FIQ invalid EL0 in AArch64.
 */
#define FIQ_INVALID_EL0_64 10

/**
 * @def ERROR_INVALID_EL0_64
 * @brief Error invalid EL0 in AArch64.
 */
#define ERROR_INVALID_EL0_64 11

/**
 * @def SYNC_INVALID_EL0_32
 * @brief Sync invalid EL0 in AArch32.
 */
#define SYNC_INVALID_EL0_32 12

/**
 * @def IRQ_INVALID_EL0_32
 * @brief IRQ invalid EL0 in AArch32.
 */
#define IRQ_INVALID_EL0_32 13

/**
 * @def FIQ_INVALID_EL0_32
 * @brief FIQ invalid EL0 in AArch32.
 */
#define FIQ_INVALID_EL0_32 14

/**
 * @def ERROR_INVALID_EL0_32
 * @brief Error invalid EL0 in AArch32.
 */
#define ERROR_INVALID_EL0_32 15

/**
 * @def SYNC_ERROR
 * @brief Sync error.
 */
#define SYNC_ERROR 16

/**
 * @def SYSCALL_ERROR
 * @brief Syscall error.
 */
#define SYSCALL_ERROR 17



#ifdef __ASSEMBLY__

.macro enable_local_irq
msr daifclr, #AARCH64_DAIF_IRQ
.endm

.macro disable_local_irq
msr   daifset, #AARCH64_DAIF_IRQ
.endm

#else

/**
 * @fn static inline uint64_t get_irq_disable_flag(void)
 * @brief get IRQ disable flag.
 *
 * @return IRQ disable flag.
 */
static inline uint64_t get_irq_disable_flag(void)
{
  uint64_t data;
  asm volatile("mrs %0, daif\n\t":"=r"(data));
  return data;
}

/**
 * @fn static inline void set_irq_disable_flag(uint64_t *flags)
 * @brief set IRQ disable flag.
 *
 * @param flags Flags.
 */
static inline void set_irq_disable_flag(uint64_t *flags)
{
  asm volatile("msr daif, %0\n\t":"=r"(flags));
}

/**
 * @def PSR_I_BIT
 * @brief PSR I bit.
 */
#define PSR_I_BIT 0x00000080

/**
 * @fn static inline void enable_local_irq(void)
 * @brief enable local IRQ.
 */
static inline void enable_local_irq(void)
{
  asm volatile("msr daifclr, %0\n\t" :: "i"(AARCH64_DAIF_IRQ));
}

/**
 * @fn static inline void disable_local_irq(void)
 * @brief disable local IRQ.
 */
static inline void disable_local_irq(void)
{
  asm volatile("msr daifset, %0\n\t" :: "i"(AARCH64_DAIF_IRQ));
}

/**
 * @fn static inline bool is_irq_enabled(unsigned long flags)
 * @brief Is IRQ enabled?
 *
 * @param flags Flags.
 * @return True if IRQ is enabled.
 */
static inline bool is_irq_enabled(unsigned long flags)
{
  return !(flags & PSR_I_BIT);
}

/**
 * @fn static inline void save_local_irq(union rflags *flags)
 * @brief save local IRQ.
 *
 * @param flags Flags.
 */
static inline void save_local_irq(union rflags *flags)
{
  asm volatile("mrs    %0, daif\n"
               : "=r"(flags->raw)
               :
               : "memory");

  if (is_irq_enabled(flags->raw)) {
    disable_local_irq();
  }
}

/**
 * @fn static inline void restore_local_irq(union rflags *flags)
 * @brief restore local IRQ.
 */
static inline void restore_local_irq(union rflags *flags)
{
  asm volatile("msr    daif, %0"
               :
               : "r"(flags->raw)
               : "memory");

  if (is_irq_enabled(flags->raw)) {
    enable_local_irq();
  }
}

/**
 * @fn static inline void generate_software_interrupt(volatile unsigned long id)
 * @brief generate software interrupt.
 */
static inline void generate_software_interrupt(volatile unsigned long id)
{
  svc(id);
}

/**
 * @fn void init_vector(void)
 * @brief initialize vector.
 */
void init_vector(void);

/**
 * @fn void show_invalid_entry_message(int type, unsigned long esr,
 *                                     unsigned long address, struct full_regs *regs);
 * @brief show invalid entry message.
 *
 * @param type Type.
 * @param esr Exception syndrome register.
 * @param address Address.
 * @param regs Registers.
 */
void show_invalid_entry_message(int type, unsigned long esr,
                                unsigned long address, struct full_regs *regs);



#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AARCH64_IRQ_H__ */
