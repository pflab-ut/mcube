/**
 * @file include/x86/ops.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_X86_OPS_H__
#define	__MCUBE_X86_OPS_H__

#ifndef __ASSEMBLY__

//----------------------------------------------------------------------------
//  @struct     registers_t
/// @brief      A record describing all 64-bit general-purpose registers.
//----------------------------------------------------------------------------
typedef struct registers {
   uint64_t rax;
   uint64_t rbx;
   uint64_t rcx;
   uint64_t rdx;
   uint64_t rsi;
   uint64_t rdi;
   uint64_t rbp;
   uint64_t r8;
   uint64_t r9;
   uint64_t r10;
   uint64_t r11;
   uint64_t r12;
   uint64_t r13;
   uint64_t r14;
   uint64_t r15;
} registers_t;

//----------------------------------------------------------------------------
//  @struct     registers4_t
/// @brief      A record describing the first 4 general-purpose registers.
//----------------------------------------------------------------------------
typedef struct registers4 {
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
} __attribute__ ((packed)) registers4_t;

static inline uint8_t inb(uint16_t port)
{
	uint8_t value;
  asm volatile ("inb %0, %1" : "=a" (value) : "Nd" (port));
	return value;
}

static inline uint16_t inw(uint16_t port)
{
	uint16_t value;
  asm volatile ("inw %0, %1" : "=a" (value) : "Nd" (port));
	return value;
}

static inline uint32_t inl(uint32_t port)
{
	uint32_t value;
  asm volatile ("inl %0, %1" : "=a" (value) : "Nd" (port));
	return value;
}

static inline uint32_t ind(uint64_t port)
{
	uint64_t value;
  asm volatile ("ind %0, %1" : "=a" (value) : "Nd" (port));
	return value;
}

static inline void outb(uint16_t port, uint8_t value)
{
  asm volatile ("outb %0, %1" :: "Nd"(port), "a"(value));   
}

static inline void outw(uint16_t port, uint16_t value)
{
  asm volatile ("outw %0, %1" :: "Nd"(port), "a"(value));   
}

static inline void outl(uint32_t port, uint32_t value)
{
  asm volatile ("outl %0, %1" :: "Nd"(port), "a"(value));   
}

static inline void outd(uint64_t port, uint64_t value)
{
  asm volatile ("outd %0, %1" :: "Nd"(port), "a"(value));   
}

static inline void finit(void)
{
	asm volatile("finit");
}


/* clear interrupt */
static inline void cli(void)
{
	asm volatile("cli");
}

/* set interrupt */
static inline void sti(void)
{
	asm volatile("sti");
}

static inline void leave(void)
{
	asm volatile("leave");
}

static inline void pause(void)
{
	asm volatile("pause");
}

static inline void iret(void)
{
	asm volatile("iret");
}

static inline void halt(void)
{
	asm volatile("hlt");
}

static inline void nop(void)
{
	asm volatile("nop");
}

static inline void sfence(void)
{
	asm volatile("sfence");
}

static inline void lfence(void)
{
	asm volatile("lfence");
}

static inline void mfence(void)
{
	asm volatile("mfence");
}


static inline void cpuid(uint32_t code, registers4_t *regs)
{
  asm volatile ("cpuid"
                : "=a" (regs->eax), "=b" (regs->ebx), "=c" (regs->ecx),
                  "=d" (regs->edx)
                : "0" (code));
}


/* read time stamp counter */
static inline uint64_t rdtsc(void)
{
  uint64_t x;

	asm volatile("cpuid" ::: "rax", "rbx", "rcx", "rdx");
	asm volatile("rdtsc" : "=A" (x));
  return x;
}

/* read time stamp counter and processor ID */
static inline uint64_t rdtscp(void)
{
	uint32_t high, low;
	asm volatile("rdtscp" : "=a"(low), "=d"(high));
	return (uint64_t) low | ((uint64_t) high << 32);
}


/* bit search forward */
static inline uint32_t bsf(uint32_t bitmap)
{
	asm volatile("bsf %1, %0" : "=r" (bitmap) : "rm" (bitmap));
	return bitmap;
}

/* bit search reverse */
static inline uint32_t bsr(uint32_t bitmap)
{
	asm volatile("bsr %1, %0" : "=r" (bitmap) : "rm" (bitmap));
	return bitmap;
}

/* load task register */
static inline void ltr(uint32_t tr)
{
	asm volatile("ltr %0" :: "r"(tr));
}

/* write back and invalidate cache */
static inline void wbinvd(void)
{
	asm volatile("wbinvd");
}

/* read memory specific register */
static inline uint64_t rdmsr(uint32_t addr)
{
  uint64_t data;
  asm volatile ("rdmsr"
                : "=A" (data)
                : "c" (addr));
	return data;
}

/* write to model specific register */
static inline void wrmsr(uint32_t addr, uint64_t data)
{
  asm volatile ("wrmsr"
                :
                : "c" (addr), "A" (data));
}

static inline void invalid_opcode(void)
{
  asm volatile ("int 6");
}


static inline void invalidate_page(void *vaddr)
{
  asm volatile ("invlpg %0\n" :: "m" (vaddr) : "memory");
}


static inline void fatal(void)
{
  asm volatile("int 0xff");
}


#endif /* !__ASSEMBLY__ */


#endif	/* __MCUBE_X86_OPS_H__ */
