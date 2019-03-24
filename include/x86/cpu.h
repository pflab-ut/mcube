/**
 * @file include/x86/cpu.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_X86_CPU_H__
#define	__MCUBE_X86_CPU_H__

#define USER_LEVEL 0
#define KERNEL_LEVEL 3

#define REGISTER_LENGTH 64

#ifdef __ASSEMBLY__

.macro get_cpu_id
movl $0x1, % eax
cpuid
movl % ebx, % eax
andl $0x0f000000, % eax
/* virtual machine */
shrl $0x18, % eax
/* real machine */
/* omit logical cpu id */
//  shrl $0x19, %eax
.endm

#else


// CPU EFLAGS register values
#define CPU_EFLAGS_CARRY       (1 << 0)
#define CPU_EFLAGS_BIT1        (1 << 1) /* bit1 is always 1 */
#define CPU_EFLAGS_PARITY      (1 << 2)
#define CPU_EFLAGS_BIT3        (0 << 3) /* bit3 is always 0 */
#define CPU_EFLAGS_ADJUST      (1 << 4)
#define CPU_EFLAGS_BIT5        (0 << 5) /* bit5 is always 0 */
#define CPU_EFLAGS_ZERO        (1 << 6)
#define CPU_EFLAGS_SIGN        (1 << 7)
#define CPU_EFLAGS_TRAP        (1 << 8)
#define CPU_EFLAGS_INTERRUPT   (1 << 9)
#define CPU_EFLAGS_DIRECTION   (1 << 10)
#define CPU_EFLAGS_OVERFLOW    (1 << 11)
#define CPU_EFLAGS_IOPL1       (1 << 12)
#define CPU_EFLAGS_IOPL0       (1 << 13)
#define CPU_EFLAGS_NESTED      (1 << 14)
#define EFLAGS_BIT15           (0 << 15) /* bit15 is always 0 */
#define CPU_EFLAGS_RESUME      (1 << 16)
#define CPU_EFLAGS_V8086       (1 << 17)
#define CPU_EFLAGS_ALIGNCHECK  (1 << 18)
#define CPU_EFLAGS_VINTERRUPT  (1 << 19)
#define CPU_EFLAGS_VPENDING    (1 << 20)
#define CPU_EFLAGS_CPUID       (1 << 21)
/* 31-22: reserved */

#define CPU_EFLAGS_BIT (CPU_EFLAGS_BIT1 | CPU_EFLAGS_BIT3 | CPU_EFLAGS_BIT5 | CPU_EFLAGS_BIT15)

#define INIT_CPU_EFLAGS (CPU_EFLAGS_IF | CPU_EFLAGS_BIT)


#if 0
static inline unsigned long get_lapic_id(void)
{
  registers4_t regs4;
  cpuid(0x1, &regs4);

  /* real machine */
  /* NOTE: omit logical processor id */
  return (regs4.ebx & 0x0f000000) >> 25;
  /* virtual machine */
  //  return (regs4.ebx & 0x0f000000) >> 24;
}
#endif

#if 0
static inline uint32_t get_lapic_id(void)
{
  return mmio_in32(LAPIC_LOCAL_APIC_ID);
}
#endif

//#define get_cpu_id() get_lapic_id()
#define get_cpu_id() 0

#if 0
static inline int get_nr_cpu_cores(void)
{
  registers4_t regs4;
  cpuid(0x4, &regs4);

  if (regs4.eax & 0x1f) {
    /* omit logical cpu id */
    return ((regs4.eax >> 27) + 1);
    /* virtual machine */
    //    return ((regs4.eax >> 26) + 1);
  }

  return 1;
}
#endif

void init_syscall(void);


void print_vendor_id(void);
void print_simd_info(void);
void print_cpu_brand(void);



#endif /* __ASSEMBLY__ */

#define VENDOR_ID_LENGTH 12
#define CPU_BRAND_LENGTH 48


#define REG_LENGTH 64


#endif	/* __MCUBE_X86_CPU_H__ */
