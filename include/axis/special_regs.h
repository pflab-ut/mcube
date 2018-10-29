/**
 * @file include/axis/special_regs.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_AXIS_SPECIAL_REGS_H__
#define __MCUBE_AXIS_SPECIAL_REGS_H__

#ifndef __ASSEMBLY__

#define NR_SOFTWARE_INTERRUPTS 8UL
#define SOFTWARE_INTERRUPT_OFFSET 8UL


static inline unsigned long get_cpu_id(void)
{
  unsigned long data;
  asm volatile("mfs %0, $1" : "=r"(data));
  return data;
}


static inline unsigned long get_interrupt_vector_base_address(void)
{
  unsigned long addr;
  asm volatile("mfs %0, $2" : "=r"(addr));
  return addr;
}

static inline void set_interrupt_vector_base_address(unsigned long addr)
{
  asm volatile("mts $2, %0" :: "r"(addr));
}

static inline unsigned long get_interrupt_program_counter(void)
{
  unsigned long addr;
  asm volatile("mfs %0, $3" : "=r"(addr));
  return addr;
}

static inline void set_interrupt_program_counter(unsigned long addr)
{
  asm volatile("mts $3, %0" :: "r"(addr));
}

static inline unsigned long get_interrupt_status(void)
{
  unsigned long status;
  asm volatile("mfs %0, $4" : "=r"(status));
  return status;
}

static inline void enable_timer_interrupt(void)
{
  unsigned long data;
  asm volatile("mfs %0, $5" : "=r"(data));
  data |= (0x1 << 0);
  asm volatile("mts $5, %0" :: "r"(data));
}

static inline void disable_timer_interrupt(void)
{
  unsigned long data;
  asm volatile("mfs %0, $5" : "=r"(data));
  data &= ~(0x1 << 0);
  asm volatile("mts $5, %0" :: "r"(data));
}

static inline void enable_dmac_interrupt(void)
{
  unsigned long data;
  asm volatile("mfs %0, $5" : "=r"(data));
  data |= (0x1 << 1);
  asm volatile("mts $5, %0" :: "r"(data));
}

static inline void disable_dmac_interrupt(void)
{
  unsigned long data;
  asm volatile("mfs %0, $5" : "=r"(data));
  data &= ~(0x1 << 1);
  asm volatile("mts $5, %0" :: "r"(data));
}


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

static inline unsigned long get_time_stamp_counter(void)
{
  unsigned long data;
  asm volatile("mfs %0, $6" : "=r"(data));
  return data;
}

static inline void set_time_stamp_counter(unsigned long data)
{
  asm volatile("mts $6, %0":: "r"(data));
}

static inline unsigned long get_current_cpu_time(void)
{
  return get_time_stamp_counter();
}

static inline void enable_timer(void)
{
  asm volatile("mts $8, %0" :: "r"(1));
}

static inline void disable_timer(void)
{
  asm volatile("mts $8, %0" :: "r"(0));
}


static inline unsigned long get_timer_count(void)
{
  unsigned long data;
  asm volatile("mfs %0, $9" : "=r"(data));
  return data;
}

static inline void set_timer_count(unsigned long data)
{
  asm volatile("mts $9, %0" :: "r"(data));
}

static inline unsigned long get_timer_period(void)
{
  unsigned long data;
  asm volatile("mfs %0, $10" : "=r"(data));
  return data;
}

static inline void set_timer_period(unsigned long data)
{
  asm volatile("mts $10, %0" :: "r"(data));
}

static inline unsigned long get_timer_status(void)
{
  unsigned long data;
  asm volatile("mfs %0, $11" : "=r"(data));
  return data;
}

static inline void set_timer_status(unsigned long data)
{
  asm volatile("mts $11, %0" :: "r"(data));
}


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_AXIS_SPECIAL_REGS__ */
