/**
 * @file include/x86/ops.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_X86_OPS_H__
#define	__MCUBE_X86_OPS_H__

#ifndef __ASSEMBLY__

static inline uint8_t inb(uint16_t port)
{
	uint8_t value;
	asm volatile("inb %1, %0" : "=a"(value) : "dN"(port)); 
	return value;
}

static inline uint16_t inw(uint16_t port)
{
	uint16_t value;
	asm volatile("inw %1, %0" : "=a" (value) : "dN" (port));
	return value;
}

static inline uint32_t inl(uint32_t port)
{
	uint32_t value;
	asm volatile("inl %1, %0" : "=a" (value) : "dN" (port));
	return value;
}

static inline uint32_t ind(uint64_t port)
{
	uint64_t value;
	asm volatile("ind %1, %0" : "=a" (value) : "dN" (port));
	return value;
}

static inline void outb(uint16_t port, uint8_t value)
{
  asm volatile("outb %0, %1" :: "a"(value), "dN"(port));
}

static inline void outw(uint16_t port, uint16_t value)
{
	asm volatile("outw %0, %1" :: "a" (value), "dN" (port));
}

static inline void outl(uint32_t port, uint32_t value)
{
	asm volatile("outl %0, %1" :: "a" (value), "dN" (port));
}

static inline void outd(uint64_t port, uint64_t value)
{
	asm volatile("outd %0, %1" :: "a" (value), "dN" (port));
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




static inline void cpuid(unsigned int op,
												 unsigned int *eax, unsigned int *ebx,
												 unsigned int *ecx, unsigned int *edx)
{
	*eax = op;
	*ecx = 0;
	asm volatile("cpuid"
							 : "=a" (*eax),
								 "=b" (*ebx),
								 "=c" (*ecx),
								 "=d" (*edx)
							 : "0" (*eax), "2" (*ecx));
}

/* read time stamp counter */
static inline uint64_t rdtsc(void)
{
  uint64_t x;

	asm volatile("cpuid" : : : "rax", "rbx", "rcx", "rdx");
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
	uint64_t x;
	asm volatile("rdmsr" : "=A"(x) :"c"(addr));
	return x;
}

/* write to model specific register */
static inline void wrmsr(uint64_t data, uint32_t addr)
{
	asm volatile("wrmsr" : "=c"(addr) : "A"(data));
}


static inline void set_pagetable(uint64_t paddr)
{
  asm volatile ("mov    rdi,    %[paddr]\n"
                "mov    cr3,    rdi\n"
                :
                : [paddr] "m" (paddr)
                : "rdi");
}

static inline void invalidate_page(void *vaddr)
{
  asm volatile (
                "invlpg     %[v]\n"
                :
                : [v] "m" (vaddr)
                : "memory");
}


static inline void fatal(void)
{
  //  asm volatile("int 0xff");
  asm volatile("int %0" :: "i"(0xff));
}


#endif /* !__ASSEMBLY__ */


#endif	/* __MCUBE_X86_OPS_H__ */
