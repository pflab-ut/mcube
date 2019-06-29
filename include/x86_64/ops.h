/**
 * @file include/x86_64/ops.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_64_OPS_H__
#define __MCUBE_X86_64_OPS_H__

#ifndef __ASSEMBLY__

/**
 * @struct registers
 * @brief A record describing all 64-bit general-purpose registers.
 */
struct registers {
  /**
   * RAX.
   */
  uint64_t rax;

  /**
   * RBX.
   */
  uint64_t rbx;

  /**
   * RCX.
   */
  uint64_t rcx;

  /**
   * RDX.
   */
  uint64_t rdx;

  /**
   * RSI.
   */
  uint64_t rsi;

  /**
   * RDI.
   */
  uint64_t rdi;

  /**
   * RBP.
   */
  uint64_t rbp;

  /**
   * R8.
   */
  uint64_t r8;

  /**
   * R9.
   */
  uint64_t r9;

  /**
   * R10.
   */
  uint64_t r10;

  /**
   * R11.
   */
  uint64_t r11;

  /**
   * R12.
   */
  uint64_t r12;

  /**
   * R13.
   */
  uint64_t r13;

  /**
   * R14.
   */
  uint64_t r14;

  /**
   * R15.
   */
  uint64_t r15;

};

/**
 * @typedef registers_t
 * @brief Typedef of @struct registers.
 */
typedef struct registers registers_t;


/**
 * @fn static inline uint8_t inb(uint16_t port)
 * @brief Wrapper function for @c inb.
 *
 * @param port Port.
 * @return Value.
 */
static inline uint8_t inb(uint16_t port)
{
  uint8_t val;

  asm volatile("inb %[port], %[val]" : [val] "=a"(val) : [port] "Nd"(port));

  return val;
}

/**
 * @fn static inline uint16_t inw(uint16_t port)
 * @brief Wrapper function for @c inw.
 *
 * @param port Port.
 * @return Value.
 */
static inline uint16_t inw(uint16_t port)
{
  uint16_t val;

  asm volatile("inw %[port], %[val]" : [val] "=a"(val) : [port] "Nd"(port));

  return val;
}

/**
 * @fn static inline uint32_t inl(uint32_t port)
 * @brief Wrapper function for @c inl.
 *
 * @param port Port.
 * @return Value.
 */
static inline uint32_t inl(uint32_t port)
{
  uint32_t val;

  asm volatile("inl %[port], %[val]" : [val] "=a"(val) : [port] "Nd"(port));

  return val;
}

/**
 * @fn static inline uint64_t ind(uint64_t port)
 * @brief Wrapper function for @c ind.
 *
 * @param port Port.
 * @return Value.
 */
static inline uint64_t ind(uint64_t port)
{
  uint64_t val;

  asm volatile("ind %[port], %[val]" : [val] "=a"(val) : [port] "Nd"(port));

  return val;
}

/**
 * @fn static inline void outb(uint8_t val, uint16_t port)
 * @brief wrappter function for @c outb.
 *
 * @param val Value.
 * @param port Port.
 */
static inline void outb(uint8_t val, uint16_t port)
{
  asm volatile("outb %[val], %[port]" :: [val] "a"(val), [port] "Nd"(port));
}

/**
 * @fn static inline void outw(uint16_t val, uint16_t port)
 * @brief wrappter function for @c outw.
 *
 * @param val Value.
 * @param port Port.
 */
static inline void outw(uint16_t val, uint16_t port)
{
  asm volatile("outw %[val], %[port]" :: [val] "a"(val), [port] "Nd"(port));
}

/**
 * @fn static inline void outl(uint32_t val, uint32_t port)
 * @brief wrappter function for @c outl.
 *
 * @param val Value.
 * @param port Port.
 */
static inline void outl(uint32_t val, uint32_t port)
{
  asm volatile("outl %[val], %[port]" :: [val] "a"(val), [port] "Nd"(port));
}

/**
 * @fn static inline void outd(uint64_t val, uint64_t port)
 * @brief wrappter function for @c outd.
 *
 * @param val Value.
 * @param port Port.
 */
static inline void outd(uint64_t val, uint64_t port)
{
  asm volatile("outd %[val], %[port]" :: [val] "a"(val), [port] "Nd"(port));
}


/**
 * @fn static inline void io_delay(void)
 * @brief A (hopefully) free port for I/O delay. Port 0x80 causes
 * problems on HP Pavilion laptops.
 */
static inline void io_delay(void)
{
  asm volatile("outb %al, $0xed");
}

/**
 * @fn static inline void finit(void)
 * @brief Wrapper function for @c finit.
 */
static inline void finit(void)
{
  asm volatile("finit");
}

/**
 * @fn static inline void cli(void)
 * @brief Wrapper function for @c cli.
 */
static inline void cli(void)
{
  asm volatile("cli"
               ::
               :"cc", "memory");
}

/**
 * @fn static inline void sti(void)
 * @brief Wrapper function for @c sti.
 */
static inline void sti(void)
{
  asm volatile("sti"
               ::
               :"cc", "memory");
}

/**
 * @fn static inline void leave(void)
 * @brief Wrapper function for @c leave.
 */
static inline void leave(void)
{
  asm volatile("leave");
}

/**
 * @fn static inline void pause(void)
 * @brief Wrapper function for @c pause.
 */
static inline void pause(void)
{
  asm volatile("pause");
}

/**
 * @fn static inline void iret(void)
 * @brief Wrapper function for @c iret.
 */
static inline void iret(void)
{
  asm volatile("iret");
}

/**
 * @fn static inline void hlt(void)
 * @brief Wrapper function for @c hlt.
 */
static inline void hlt(void)
{
  asm volatile("hlt");
}

/**
 * @def halt()
 * @brief Wrapper definition for infinite @c hlt.
 */
#define halt()                                  \
  do {                                          \
    while (true) {                              \
      asm volatile ("hlt":::"memory");          \
    }                                           \
  } while (0);

/**
 * @fn static inline void nop(void)
 * @brief Wrapper function for @c nop.
 */
static inline void nop(void)
{
  asm volatile("nop");
}

/**
 * @fn static inline void sfence(void)
 * @brief Wrapper function for @c sfence.
 */
static inline void sfence(void)
{
  asm volatile("sfence");
}

/**
 * @fn static inline void lfence(void)
 * @brief Wrapper function for @c lfence.
 */
static inline void lfence(void)
{
  asm volatile("lfence");
}

/**
 * @fn static inline void mfence(void)
 * @brief Wrapper function for @c mfence.
 */
static inline void mfence(void)
{
  asm volatile("mfence");
}


/**
 * @fn static inline uint64_t rdtsc(void)
 * @brief read time stamp counter.
 *
 * @return Time stamp counter.
 */
static inline uint64_t rdtsc(void)
{
  uint64_t x;

  asm volatile("cpuid" ::: "rax", "rbx", "rcx", "rdx");
  asm volatile("rdtsc" : "=A"(x));
  return x;
}

/**
 * @fn static inline uint64_t rdtscp(void)
 * @brief read time stamp counter and processor ID.
 *
 * @return Time stamp counter and processor ID.
 */
static inline uint64_t rdtscp(void)
{
  uint64_t x;
  asm volatile("cpuid" ::: "rax", "rbx", "rcx", "rdx");
  asm volatile("rdtscp" : "=A"(x));
  return x;
}


/**
 * @fn static inline uint32_t bsf(uint32_t bitmap)
 * @brief Bit search forward.
 *
 * @param bitmap Bitmap.
 * @return First bit index.
 */
static inline uint32_t bsf(uint32_t bitmap)
{
  asm volatile("bsf %1, %0" : "=r"(bitmap) : "rm"(bitmap));
  return bitmap;
}

/**
 * @fn static inline uint32_t bsr(uint32_t bitmap)
 * @brief Bit search reverse.
 *
 * @param bitmap Bitmap.
 * @return First bit index.
 */
static inline uint32_t bsr(uint32_t bitmap)
{
  asm volatile("bsr %1, %0" : "=r"(bitmap) : "rm"(bitmap));
  return bitmap;
}

/**
 * @fn static inline void ltr(uint32_t tr)
 * @brief Load task register.
 *
 * @param tr Task register.
 */
static inline void ltr(uint32_t tr)
{
  asm volatile("ltr %0" :: "r"(tr));
}

/**
 * @fn static inline void wbinvd(void)
 * @brief write back and invalidate cache.
 */
static inline void wbinvd(void)
{
  asm volatile("wbinvd");
}

/**
 * @fn static inline void wrmsr(uint32_t msr, uint64_t val)
 * @brief write the content of @a val using the %edx:%eax register pair to
 * the @a msr MSR.
 *
 * @param msr MSR.
 * @param val Value.
 */
static inline void wrmsr(uint32_t msr, uint64_t val)
{
  uint32_t high = val >> 32;
  uint32_t low = val & 0xffffffff;

  asm volatile("wrmsr"
               :
               : "a"(low), "d"(high), "c"(msr));
}

/**
 * @fn static inline uint64_t rdmsr(uint32_t msr)
 * @brief return the content of the @a msr MSR (%ecx) using the %edx:%eax
 * register pair.
 *
 * @param msr MSR.
 * @return Content of MSR.
 */
static inline uint64_t rdmsr(uint32_t msr)
{
  uint32_t high, low;

  asm volatile("rdmsr"
               : "=&a"(low), "=&d"(high)
               : "c"(msr));

  return ((uint64_t) high << 32) + low;
}

/**
 * @fn static inline void invalidate_page(void *vaddr)
 * @brief Invalidate page.
 *
 * @param vaddr Virtual address.
 */
static inline void invalidate_page(void *vaddr)
{
  asm volatile("invlpg %0\n" :: "m"(vaddr) : "memory");
}


#endif /* !__ASSEMBLY__ */


#endif /* __MCUBE_X86_64_OPS_H__ */
