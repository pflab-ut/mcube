/**
 * @file include/axis/sys_regs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_SYS_REGS_H__
#define __MCUBE_AXIS_SYS_REGS_H__


#define SYS_CH_BASE(id) (SYS_BASE + ((id) * 0x100))

#define SYS_CPU_CTRL(id) (SYS_CH_BASE(id) + 0x0)
#define SYS_SET_PROGRAM_COUNTER(id) (SYS_CH_BASE(id) + 0x4)
#define SYS_CPU_STATUS(id) (SYS_CH_BASE(id) + 0x8)
#define SYS_NET_BASE(id) (SYS_CH_BASE(id) + 0x10)
#define SYS_NET_CTRL(id) (SYS_CH_BASE(id) + 0x14)

#define SYS_COMMON_BASE (SYS_BASE + 0x800)

#define SYS_COMMON_INTERRUPT_STATUS (SYS_COMMON_BASE + 0x0)
#define SYS_COMMON_INTERRUPT_GENERATION (SYS_COMMON_BASE + 0x4)
#define SYS_COMMON_INTERRUPT_CLEAR (SYS_COMMON_BASE + 0x8)

#ifndef __ASSEMBLY__

static inline void start_cpu(volatile unsigned long id)
{
  if (id > NR_INTRA_KERNEL_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_INTRA_KERNEL_CPUS);
    return;
  }
  printk("SYS_CPU_CTRL(%lu) = 0x%lx\n", id, SYS_CPU_CTRL(id));
  mmio_out32(SYS_CPU_CTRL(id), 0);
}

static inline void stop_cpu(volatile unsigned long id)
{
  if (id > NR_INTRA_KERNEL_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_INTRA_KERNEL_CPUS);
    return;
  }
  //  printk("stop_cpu()\n");
  printk("SYS_CPU_CTRL(%lu) = 0x%lx\n", id, SYS_CPU_CTRL(id));
  mmio_out32(SYS_CPU_CTRL(id), 1);
}

static inline void set_program_counter(volatile unsigned long id, volatile unsigned long pc)
{
  if (id > NR_INTRA_KERNEL_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_INTRA_KERNEL_CPUS);
    return;
  }
  mmio_out32(SYS_CPU_CTRL(id), pc);
}

static inline unsigned long get_program_counter(volatile unsigned long id)
{
  volatile unsigned long data;
  if (id > NR_INTRA_KERNEL_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_INTRA_KERNEL_CPUS);
    return -1;
  }
  data = mmio_in32(SYS_CPU_STATUS(id));
  return data;
}

static inline unsigned long get_net_base_address(volatile unsigned long id)
{
  volatile unsigned long data;
  if (id > NR_INTRA_KERNEL_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_INTRA_KERNEL_CPUS);
    return -1;
  }
  data = mmio_in32(SYS_NET_BASE(id));
  return data;
}

static inline void set_net_base_address(volatile unsigned long id, volatile unsigned long addr)
{
  if (id > NR_INTRA_KERNEL_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_INTRA_KERNEL_CPUS);
    return;
  }
  mmio_out32(SYS_NET_BASE(id), addr);
}

static inline unsigned long get_net_ctrl_address(volatile unsigned long id)
{
  volatile unsigned long data;
  if (id > NR_INTRA_KERNEL_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_INTRA_KERNEL_CPUS);
    return -1;
  }
  data = mmio_in32(SYS_NET_CTRL(id));
  return data;
}

static inline void set_net_ctrl_address(volatile unsigned long id, volatile unsigned long addr)
{
  if (id > NR_INTRA_KERNEL_CPUS) {
    printk("Error: cpu id %lu exceeds %d\n", id, NR_INTRA_KERNEL_CPUS);
    return;
  }
  mmio_out32(SYS_NET_CTRL(id), addr);
}

static inline unsigned long get_common_interrupt_status(void)
{
  volatile unsigned long data;
  data = mmio_in32(SYS_COMMON_INTERRUPT_STATUS);
  return data;
}

static inline void set_common_interrupt_generation(volatile unsigned long id)
{
  if (id > NR_SOFTWARE_INTERRUPTS) {
    printk("Error: cpu id %lu exceeds %lu\n", id, NR_SOFTWARE_INTERRUPTS);
    return;
  }
  mmio_out32(SYS_COMMON_INTERRUPT_GENERATION, 0x1 << id);
}

static inline void generate_software_interrupt(volatile unsigned long id)
{
  set_common_interrupt_generation(id);
}

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
