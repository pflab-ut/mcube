/**
 * @file include/axis/special_regs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_SPECIAL_REGS_H__
#define __MCUBE_AXIS_SPECIAL_REGS_H__

#ifndef __ASSEMBLY__

/**
 * @def NR_SOFTWARE_INTERRUPTS
 * @brief Number of software interrupts.
 */
#define NR_SOFTWARE_INTERRUPTS 8

/**
 * @def SOFTWARE_INTERRUPT_OFFSET
 * @brief Offset of software interrupt.
 */
#define SOFTWARE_INTERRUPT_OFFSET 8

/**
 * @fn static inline unsigned long get_cpu_id(void)
 * @brief get CPU ID.
 *
 * @return CPU ID.
 */
static inline unsigned long get_cpu_id(void)
{
  unsigned long data;
  unsigned long cluster;
  asm volatile("mfs %0, $1" : "=r"(data));
  cluster = data >> 16;
  return (cluster << 3) + (data & 0x7);
}

/**
 * @fn static inline unsigned long get_cluster_id(void)
 * @brief get cluster ID.
 *
 * @return Cluster ID.
 */
static inline unsigned long get_cluster_id(void)
{
  unsigned long data;
  asm volatile("mfs %0, $1" : "=r"(data));
  return data >> 16;
}

/**
 * @fn static inline unsigned long get_cluster_and_cpu_ids(void)
 * @brief get cluster and CPU IDs.
 *
 * @return Cluster and CPU IDs.
 */
static inline unsigned long get_cluster_and_cpu_ids(void)
{
  unsigned long data;
  asm volatile("mfs %0, $1" : "=r"(data));
  return data;
}


/**
 * @fn static inline unsigned long get_interrupt_vector_base_address(void)
 * @brief get interrupt vector base address.
 *
 * @return Interrupt vector base address.
 */
static inline unsigned long get_interrupt_vector_base_address(void)
{
  unsigned long addr;
  asm volatile("mfs %0, $2" : "=r"(addr));
  return addr;
}

/**
 * @fn static inline void set_interrupt_vector_base_address(unsigned long addr)
 * @brief set interrupt vector base address.
 *
 * @param addr Address.
 */
static inline void set_interrupt_vector_base_address(unsigned long addr)
{
  asm volatile("mts $2, %0" :: "r"(addr));
}

/**
 * @fn static inline unsigned long get_interrupt_program_counter(void)
 * @brief get interrupt program counter.
 *
 * @return Interrupt program counter.
 */
static inline unsigned long get_interrupt_program_counter(void)
{
  unsigned long addr;
  asm volatile("mfs %0, $3" : "=r"(addr));
  return addr;
}

/**
 * @fn static inline void set_interrupt_program_counter(unsigned long addr)
 * @brief set interrupt program counter.
 *
 * @param addr Address.
 */
static inline void set_interrupt_program_counter(unsigned long addr)
{
  asm volatile("mts $3, %0" :: "r"(addr));
}

/**
 * @fn static inline unsigned long get_interrupt_status(void)
 * @brief get interrupt status.
 *
 * @return Interrupt status.
 */
static inline unsigned long get_interrupt_status(void)
{
  unsigned long status;
  asm volatile("mfs %0, $4" : "=r"(status));
  return status;
}

/**
 * @fn static inline void enable_timer_interrupt(void)
 * @brief enable timer interrupt.
 */
static inline void enable_timer_interrupt(void)
{
  unsigned long data;
  asm volatile("mfs %0, $5" : "=r"(data));
  data |= (0x1 << 0);
  asm volatile("mts $5, %0" :: "r"(data));
}

/**
 * @fn static inline void disable_timer_interrupt(void)
 * @brief disable timer interrupt.
 */
static inline void disable_timer_interrupt(void)
{
  unsigned long data;
  asm volatile("mfs %0, $5" : "=r"(data));
  data &= ~(0x1 << 0);
  asm volatile("mts $5, %0" :: "r"(data));
}

/**
 * @fn static inline void enable_dmac_interrupt(void)
 * @brief enable DMAC interrupt.
 */
static inline void enable_dmac_interrupt(void)
{
  unsigned long data;
  asm volatile("mfs %0, $5" : "=r"(data));
  data |= (0x1 << 1);
  asm volatile("mts $5, %0" :: "r"(data));
}

/**
 * @fn static inline void disable_dmac_interrupt(void)
 * @brief disable DMAC interrupt.
 */
static inline void disable_dmac_interrupt(void)
{
  unsigned long data;
  asm volatile("mfs %0, $5" : "=r"(data));
  data &= ~(0x1 << 1);
  asm volatile("mts $5, %0" :: "r"(data));
}

/**
 * @fn static inline void enable_software_interrupt(unsigned long id)
 * @brief enable software interrupt.
 *
 * @param id ID.
 */
static inline void enable_software_interrupt(unsigned long id)
{
  unsigned long data;

  if (id > NR_SOFTWARE_INTERRUPTS) {
    printk("Error: cpu id %lu exceeds %lu\n", id, NR_SOFTWARE_INTERRUPTS);
    return;
  }

  asm volatile("mfs %0, $5" : "=r"(data));
  data |= (0x1 << (id + SOFTWARE_INTERRUPT_OFFSET));
  asm volatile("mts $5, %0" :: "r"(data));
}

/**
 * @fn static inline void disable_software_interrupt(unsigned long id)
 * @brief disable software interrupt.
 *
 * @param id ID.
 */
static inline void disable_software_interrupt(unsigned long id)
{
  unsigned long data;

  if (id > NR_SOFTWARE_INTERRUPTS) {
    printk("Error: cpu id %lu exceeds %lu\n", id, NR_SOFTWARE_INTERRUPTS);
    return;
  }

  asm volatile("mfs %0, $5" : "=r"(data));
  data &= ~(0x1 << (id + SOFTWARE_INTERRUPT_OFFSET));
  asm volatile("mts $5, %0" :: "r"(data));
}

/**
 * @fn static inline unsigned long get_time_stamp_counter(void)
 * @brief get time stamp counter.
 *
 * @return Time stamp counter.
 */
static inline unsigned long get_time_stamp_counter(void)
{
  unsigned long data;
  asm volatile("mfs %0, $6" : "=r"(data));
  return data;
}

/**
 * @fn static inline void set_time_stamp_counter(unsigned long data)
 * @brief set time stamp counter.
 *
 * @param data Data.
 */
static inline void set_time_stamp_counter(unsigned long data)
{
  asm volatile("mts $6, %0":: "r"(data));
}

/**
 * @fn static inline unsigned long get_current_cpu_time(void)
 * @brief get current CPU time.
 *
 * @return Current CPU time.
 */
static inline unsigned long get_current_cpu_time(void)
{
  return get_time_stamp_counter();
}

/**
 * @fn static inline void enable_timer(void)
 * @brief enable timer.
 */
static inline void enable_timer(void)
{
  asm volatile("mts $8, %0" :: "r"(1));
}

/**
 * @fn static inline void disable_timer(void)
 * @brief disable timer.
 */
static inline void disable_timer(void)
{
  asm volatile("mts $8, %0" :: "r"(0));
}

/**
 * @fn static inline unsigned long get_timer_count(void)
 * @brief get timer count.
 *
 * @return Timer count.
 */
static inline unsigned long get_timer_count(void)
{
  unsigned long data;
  asm volatile("mfs %0, $9" : "=r"(data));
  return data;
}

/**
 * @fn static inline void set_timer_count(unsigned long data)
 * @brief set timer count.
 *
 * @param data Data.
 */
static inline void set_timer_count(unsigned long data)
{
  asm volatile("mts $9, %0" :: "r"(data));
}

/**
 * @fn static inline unsigned long get_timer_period(void)
 * @brief get timer period.
 *
 * @return Timer period.
 */
static inline unsigned long get_timer_period(void)
{
  unsigned long data;
  asm volatile("mfs %0, $10" : "=r"(data));
  return data;
}

/**
 * @fn static inline void set_timer_period(unsigned long data)
 * @brief set timer period.
 *
 * @param data Data.
 */
static inline void set_timer_period(unsigned long data)
{
  asm volatile("mts $10, %0" :: "r"(data));
}

/**
 * @fn static inline unsigned long get_timer_status(void)
 * @brief get timer status.
 *
 * @return Timer status.
 */
static inline unsigned long get_timer_status(void)
{
  unsigned long data;
  asm volatile("mfs %0, $11" : "=r"(data));
  return data;
}

/**
 * @fn static inline void set_timer_status(unsigned long data)
 * @brief set timer status.
 *
 * @param data Data.
 */
static inline void set_timer_status(unsigned long data)
{
  asm volatile("mts $11, %0" :: "r"(data));
}


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AXIS_SPECIAL_REGS__ */
