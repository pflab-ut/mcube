/**
 * @file include/sim/ops.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_SIM_OPS_H__
#define __MCUBE_SIM_OPS_H__

#ifndef __ASSEMBLY__

static inline void cpuid(uint64_t op,
                         cpuid_info_t *cinfo)
{
  cinfo->rax = op;
  cinfo->rcx = 0;
  asm volatile("cpuid"
               : "=a"(cinfo->rax),
               "=b"(cinfo->rbx),
               "=c"(cinfo->rcx),
               "=d"(cinfo->rdx)
               : "0"(cinfo->rax), "2"(cinfo->rcx));
}

static inline uint64_t rdtsc(void)
{
  uint64_t x;

  asm volatile("cpuid" : : : "rax", "rbx", "rcx", "rdx");
  asm volatile("rdtsc\n\t"
               "shlq       $32, %%rdx\n\t"
               "orq        %%rdx, %%rax\n\t"
               "movq       %%rax, %0" : "=g"(x) : : "rax", "rdx");

  return x;
}

static inline uint64_t rdtscp(void)
{
  uint32_t high, low;
  asm volatile("rdtscp" : "=a"(low), "=d"(high));
  return (uint64_t) low | ((uint64_t) high << 32);
}




static inline void trap(__unused uint32_t trap_code)
{
  /* do nothing */
}


__noreturn static inline void halt(void)
{
  printk("halt\n");
  inf_loop();
}

static inline void nop(void)
{
  asm volatile("nop");
}

#endif /* !__ASSEMBLY__ */

#endif /* __MCUBE_SIM_OPS_H__ */
