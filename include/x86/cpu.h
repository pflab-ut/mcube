/**
 * @file include/x86/cpu.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_X86_CPU_H__
#define	__MCUBE_X86_CPU_H__


#ifdef __ASSEMBLY__

.macro get_cpu_id
  movl $0x1, %eax
	cpuid
  movl %ebx, %eax
	andl $0x0f000000, %eax
	/* virtual machine */
  shrl $0x18, %eax
  /* real machine */
  /* omit logical cpu id */
   //  shrl $0x19, %eax
.endm

#else

static inline unsigned int get_lapic_id(void)
{
  unsigned int eax, ebx, ecx, edx;
  cpuid(0x1, &eax, &ebx, &ecx, &edx);

	/* real machine */
	/* NOTE: omit logical processor id */
  return (ebx & 0x0f000000) >> 25;
	/* virtual machine */
  //  return (ebx & 0x0f000000) >> 24;
}

#if 0
static inline uint32_t get_lapic_id(void)
{
  return mmio_in32(LAPIC_LOCAL_APIC_ID);
}
#endif

#define get_cpu_id() get_lapic_id()

   
/**
 * @brief CPU ID information
 *
 * The cpuid_info structure has CPU ID information.
 */
struct cpuid_info {
	/** EAX register. */
  unsigned int eax;
	/** EBX register. */
	unsigned int ebx;
	/** ECX register. */
	unsigned int ecx;
	/** EDX register. */
	unsigned int edx;
};

typedef struct cpuid_info cpuid_info;

extern cpuid_info cpu_info;


static inline int get_nr_cpu_cores(void)
{
	cpuid_info cinfo;
  cpuid(0x4, &cinfo.eax, &cinfo.ebx, &cinfo.ecx, &cinfo.edx);
  if (cinfo.eax & 0x1f) {
		/* omit logical cpu id */
    return ((cinfo.eax >> 27) + 1);
		/* virtual machine */
    //    return ((cinfo.eax >> 26) + 1);
  }
  return 1;
}
 

extern void print_vendor_id(void);
extern void save_cpu_info(void);
extern unsigned int get_cpu_frequency(void);
extern void print_cpu_brand(void);



extern int ap_main(void);

extern int ap_usermain(void);


#endif /* __ASSEMBLY__ */

#define VENDOR_ID_LENGTH 12
#define CPU_BRAND_LENGTH 48


/* EFLAGS bits */
/* 31-22: reserved */
#define EFLAGS_ID (0x1 << 21)
#define EFLAGS_VIP (0x1 << 20)
#define EFLAGS_VIF (0x1 << 19)
#define EFLAGS_AC (0x1 << 18)
#define EFLAGS_VM (0x1 << 17)
#define EFLAGS_RF (0x1 << 16)
#define EFLAGS_BIT15 (0x0 << 15) /* bit15 is always 0 */
#define EFLAGS_NT (0x1 << 14)
/* 13: reserved */
#define EFLAGS_IOPL (0x3 << 12) /* Mask */
#define EFLAGS_OF (0x1 << 11)
#define EFLAGS_DF (0x1 << 10)
#define EFLAGS_IF (0x1 << 9)
#define EFLAGS_TF (0x1 << 8)
#define EFLAGS_SF (0x1 << 7)
#define EFLAGS_ZF (0x1 << 6)
#define EFLAGS_BIT5 (0x0 << 5) /* bit5 is always 0 */
#define EFLAGS_AF (0x1 << 4)
#define EFLAGS_BIT3 (0x0 << 3) /* bit3 is always 0 */
#define EFLAGS_PF (0x1 << 2)
#define EFLAGS_BIT1 (0x1 << 1) /* bit1 is always 1 */
#define EFLAGS_CF (0x1 << 0)


#define EFLAGS_BIT (EFLAGS_BIT1 | EFLAGS_BIT3 | EFLAGS_BIT5 | EFLAGS_BIT15)

#define INIT_EFLAGS (EFLAGS_IF | EFLAGS_BIT)

#define REG_LENGTH 64


#endif	/* __MCUBE_X86_CPU_H__ */
