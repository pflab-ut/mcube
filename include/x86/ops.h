/**
 * @file include/x86/ops.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_OPS_H__
#define __MCUBE_X86_OPS_H__

#ifndef __ASSEMBLY__

/**
 * @struct registers
 * @brief  A record describing all 64-bit general-purpose registers.
 */
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


static inline uint8_t inb(uint16_t port)
{
  uint8_t val;

  asm volatile("inb %[port], %[val]" : [val] "=a"(val) : [port] "Nd"(port));

  return val;
}

static inline uint16_t inw(uint16_t port)
{
  uint16_t val;

  asm volatile("inw %[port], %[val]" : [val] "=a"(val) : [port] "Nd"(port));

  return val;
}

static inline uint32_t inl(uint32_t port)
{
  uint32_t val;

  asm volatile("inl %[port], %[val]" : [val] "=a"(val) : [port] "Nd"(port));

  return val;
}

static inline uint64_t ind(uint64_t port)
{
  uint64_t val;

  asm volatile("ind %[port], %[val]" : [val] "=a"(val) : [port] "Nd"(port));

  return val;
}

static inline void outb(uint8_t val, uint16_t port)
{
  asm volatile("outb %[val], %[port]" :: [val] "a"(val), [port] "Nd"(port));
}

static inline void outw(uint16_t val, uint16_t port)
{
  asm volatile("outw %[val], %[port]" :: [val] "a"(val), [port] "Nd"(port));
}

static inline void outl(uint32_t val, uint32_t port)
{
  asm volatile("outl %[val], %[port]" :: [val] "a"(val), [port] "Nd"(port));
}

static inline void outd(uint64_t val, uint64_t port)
{
  asm volatile("outd %[val], %[port]" :: [val] "a"(val), [port] "Nd"(port));
}


/*
 * A (hopefully) free port for I/O delay. Port 0x80 causes
 * problems on HP Pavilion laptops.
 */
static inline void io_delay(void)
{
  asm volatile("outb %al, $0xed");
}

static inline void finit(void)
{
  asm volatile("finit");
}

/* clear interrupt */
static inline void cli(void)
{
  asm volatile("cli"
               ::
               :"cc", "memory");
}

/* set interrupt */
static inline void sti(void)
{
  asm volatile("sti"
               ::
               :"cc", "memory");
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

static inline void hlt(void)
{
  asm volatile("hlt");
}

#define halt()                                  \
  do {                                          \
    while (true)                                \
      asm volatile ("hlt":::"memory");          \
  } while (0);

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


/* read time stamp counter */
static inline uint64_t rdtsc(void)
{
  uint64_t x;

  asm volatile("cpuid" ::: "rax", "rbx", "rcx", "rdx");
  asm volatile("rdtsc" : "=A"(x));
  return x;
}

/* read time stamp counter and processor ID */
static inline uint64_t rdtscp(void)
{
  uint64_t x;
  asm volatile("cpuid" ::: "rax", "rbx", "rcx", "rdx");
  asm volatile("rdtscp" : "=A"(x));
  return x;
}


/* bit search forward */
static inline uint32_t bsf(uint32_t bitmap)
{
  asm volatile("bsf %1, %0" : "=r"(bitmap) : "rm"(bitmap));
  return bitmap;
}

/* bit search reverse */
static inline uint32_t bsr(uint32_t bitmap)
{
  asm volatile("bsr %1, %0" : "=r"(bitmap) : "rm"(bitmap));
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

/*
 * Write the content of @val using the %edx:%eax register pair to
 * the @msr MSR.
 */
static inline void wrmsr(uint32_t msr, uint64_t val)
{
  uint32_t high = val >> 32;
  uint32_t low = val & 0xffffffff;

  asm volatile("wrmsr"
               :
               : "a"(low), "d"(high), "c"(msr));
}
/*
 * Return the content of the @msr MSR (%ecx) using the %edx:%eax
 * register pair.
 */
static inline uint64_t rdmsr(uint32_t msr)
{
  uint32_t high, low;

  asm volatile("rdmsr"
               : "=&a"(low), "=&d"(high)
               : "c"(msr));

  return ((uint64_t) high << 32) + low;
}


static inline void invalidate_page(void *vaddr)
{
  asm volatile("invlpg %0\n" :: "m"(vaddr) : "memory");
}


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_X86_OPS_H__ */
