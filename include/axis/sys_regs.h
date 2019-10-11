/**
 * @file include/axis/sys_regs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_SYS_REGS_H__
#define __MCUBE_AXIS_SYS_REGS_H__

/**
 * @def SYS_CH_BASE(id)
 * @brief System register channel base.
 *
 * @param id ID.
 */
#define SYS_CH_BASE(id) (SYS_BASE + ((id) * 0x100))

/**
 * @def SYS_CPU_CTRL(id)
 * @brief CPU control register.
 *
 * @param id ID.
 */
#define SYS_CPU_CTRL(id) (SYS_CH_BASE(id) + 0x0)

/**
 * @def SYS_SET_PROGRAM_COUNTER(id)
 * @brief Set program counter register.
 *
 * @param id ID.
 */
#define SYS_SET_PROGRAM_COUNTER(id) (SYS_CH_BASE(id) + 0x4)

/**
 * @def SYS_CPU_STATUS(id)
 * @brief CPU status register.
 *
 * @param id ID.
 */
#define SYS_CPU_STATUS(id) (SYS_CH_BASE(id) + 0x8)

/**
 * @def SYS_NET_BASE(id)
 * @brief Network base register.
 *
 * @param id ID.
 */
#define SYS_NET_BASE(id) (SYS_CH_BASE(id) + 0x10)

/**
 * @def SYS_NET_CTRL(id)
 * @brief Network control register.
 *
 * @param id ID.
 */
#define SYS_NET_CTRL(id) (SYS_CH_BASE(id) + 0x14)

/**
 * @def SYS_COMMON_BASE
 * @brief Common base register.
 */
#define SYS_COMMON_BASE (SYS_BASE + 0x800)

/**
 * @def SYS_COMMON_INTERRUPT_STATUS
 * @brief Common interrupt status register.
 */
#define SYS_COMMON_INTERRUPT_STATUS (SYS_COMMON_BASE + 0x0)

/**
 * @def SYS_COMMON_INTERRUPT_GENERATION
 * @brief Common interrupt generation register.
 */
#define SYS_COMMON_INTERRUPT_GENERATION (SYS_COMMON_BASE + 0x4)

/**
 * @def SYS_COMMON_INTERRUPT_CLEAR
 * @brief Common interrupt clear register.
 */
#define SYS_COMMON_INTERRUPT_CLEAR (SYS_COMMON_BASE + 0x8)

#ifndef __ASSEMBLY__

/**
 * @fn static inline void start_cpu(volatile unsigned long id)
 * @brief start CPU.
 *
 * @param id ID.
 */
static inline void start_cpu(volatile unsigned long id)
{
  if (id > NR_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_CPUS);
    return;
  }

  //  printk("SYS_CPU_CTRL(%lu) = 0x%lx\n", id, SYS_CPU_CTRL(id));
  mmio_out32(SYS_CPU_CTRL(id), 0);
}

/**
 * @fn static inline void stop_cpu(volatile unsigned long id)
 * @brief stop CPU.
 *
 * @param id ID.
 */
static inline void stop_cpu(volatile unsigned long id)
{
  if (id > NR_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_CPUS);
    return;
  }

  //  printk("stop_cpu()\n");
  //  printk("SYS_CPU_CTRL(%lu) = 0x%lx\n", id, SYS_CPU_CTRL(id));
  mmio_out32(SYS_CPU_CTRL(id), 1);
}

/**
 * @fn static inline void set_program_counter(volatile unsigned long id,
 *                                            volatile unsigned long pc)
 *
 * @brief set program counter.
 *
 * @param id ID.
 * @param pc PC.
 */
static inline void set_program_counter(volatile unsigned long id,
                                       volatile unsigned long pc)
{
  if (id > NR_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_CPUS);
    return;
  }

  mmio_out32(SYS_CPU_CTRL(id), pc);
}

/**
 * @fn static inline unsigned long get_program_counter(volatile unsigned long id)
 * @brief get program counter.
 *
 * @param id ID.
 * @return Program counter.
 */
static inline unsigned long get_program_counter(volatile unsigned long id)
{
  volatile unsigned long data;

  if (id > NR_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_CPUS);
    return -1;
  }

  data = mmio_in32(SYS_CPU_STATUS(id));
  return data;
}

/**
 * @fn static inline unsigned long get_net_base_address(volatile unsigned long id)
 * @brief get network base address.
 *
 * @param id ID.
 * @return Network base address.
 */
static inline unsigned long get_net_base_address(volatile unsigned long id)
{
  volatile unsigned long data;

  if (id > NR_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_CPUS);
    return -1;
  }

  data = mmio_in32(SYS_NET_BASE(id));
  return data;
}

/**
 * @fn static inline void set_net_base_address(volatile unsigned long id,
 *                                             volatile unsigned long addr)
 * @brief set network base address.
 *
 * @param id ID.
 * @param addr Address.
 */
static inline void set_net_base_address(volatile unsigned long id,
                                        volatile unsigned long addr)
{
  if (id > NR_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_CPUS);
    return;
  }

  mmio_out32(SYS_NET_BASE(id), addr);
}

/**
 * @fn static inline unsigned long get_net_ctrl_address(volatile unsigned long id)
 * @brief get network control address.
 *
 * @param id ID.
 * @return Network control address.
 */
static inline unsigned long get_net_ctrl_address(volatile unsigned long id)
{
  volatile unsigned long data;

  if (id > NR_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_CPUS);
    return -1;
  }

  data = mmio_in32(SYS_NET_CTRL(id));
  return data;
}

/**
 * @fn static inline void set_net_ctrl_address(volatile unsigned long id,
 *                                             volatile unsigned long addr)
 * @brief set network control address.
 *
 * @param id ID.
 * @param addr Address.
 */
static inline void set_net_ctrl_address(volatile unsigned long id,
                                        volatile unsigned long addr)
{
  if (id > NR_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_CPUS);
    return;
  }

  mmio_out32(SYS_NET_CTRL(id), addr);
}

/**
 * @fn static inline unsigned long get_common_interrupt_status(void)
 * @brief get common interrupt status.
 *
 * @return Common interrupt status.
 */
static inline unsigned long get_common_interrupt_status(void)
{
  volatile unsigned long data;
  data = mmio_in32(SYS_COMMON_INTERRUPT_STATUS);
  return data;
}

/**
 * @fn static inline void set_common_interrupt_generation(volatile unsigned long id)
 * @brief set common interrupt generation.
 *
 * @param id ID.
 */
static inline void set_common_interrupt_generation(volatile unsigned long id)
{
  if (id > NR_SOFTWARE_INTERRUPTS) {
    printk("Error: cpu id %lu exceeds %lu\n", id, NR_SOFTWARE_INTERRUPTS);
    return;
  }

  mmio_out32(SYS_COMMON_INTERRUPT_GENERATION, 0x1 << id);
}

/**
 * @fn static inline void generate_software_interrupt(volatile unsigned long id)
 * @brief generate software interrupt.
 *
 * @param id ID.
 */
static inline void generate_software_interrupt(volatile unsigned long id)
{
  set_common_interrupt_generation(id);
}

/**
 * @fn static inline void set_common_interrupt_clear(volatile unsigned long id)
 * @brief set common interrupt clear.
 *
 * @param id ID.
 */
static inline void set_common_interrupt_clear(volatile unsigned long id)
{
  if (id > NR_SOFTWARE_INTERRUPTS) {
    printk("Error: cpu id %lu exceeds %lu\n", id, NR_SOFTWARE_INTERRUPTS);
    return;
  }

  mmio_out32(SYS_COMMON_INTERRUPT_CLEAR, 0x1 << id);
}


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AXIS_SYS_REGS__ */
