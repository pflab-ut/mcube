/**
 * @file arch/x86/cache.c
 *
 * @author Hiroyuki Chishiro
 */
#include <mcube/mcube.h>

void set_mtrr_def_type_msr(uint8_t mem_type)
{
	uint64_t val;
	val = rdmsr(MSR_MTRR_DEF_TYPE);
	val = (val & ~0xff) | mem_type;
	wrmsr(val, MSR_MTRR_DEF_TYPE);
}


/* disable memory type range register */
void disable_mtrr(void)
{
	uint64_t val;
	val = rdmsr(MSR_MTRR_DEF_TYPE);
	val &= ~MSR_MTRR_DEF_TYPE_ENABLE;
	wrmsr(val, MSR_MTRR_DEF_TYPE);
}

/* enable memory type range register */
void enable_mtrr(void)
{
	uint64_t val;
	val = rdmsr(MSR_MTRR_DEF_TYPE);
	val |= MSR_MTRR_DEF_TYPE_ENABLE;
	wrmsr(val, MSR_MTRR_DEF_TYPE);
}

void disable_l3_cache(void)
{
	uint64_t val;
	val = rdmsr(MSR_IA32_MISC_ENABLE);
	val |= MSR_IA32_MISC_ENABLE_L3CACHE_DISABLE;
	wrmsr(val, MSR_IA32_MISC_ENABLE);
}


void enable_l3_cache(void)
{
	uint64_t val;
	val = rdmsr(MSR_IA32_MISC_ENABLE);
	val &= ~MSR_IA32_MISC_ENABLE_L3CACHE_DISABLE;
	wrmsr(val, MSR_IA32_MISC_ENABLE);
}


void disable_cache(void)
{
#if 1
	disable_l3_cache();
	reset_cache();
	set_cr0((get_cr0() | CR0_DISABLE_CACHE) & ~CR0_NOT_WRITE_THROUGH);
	reset_cache();
	disable_mtrr();
	set_mtrr_def_type_msr(MSR_MTRR_DEF_TYPE_MEM_TYPE_UNCACHED);
	reset_cache();
#else
	set_cr0(get_cr0() | (CR0_DISABLE_CACHE | CR0_NOT_WRITE_THROUGH));
#endif
}

void enable_cache(void)
{
	set_mtrr_def_type_msr(MSR_MTRR_DEF_TYPE_MEM_TYPE_WRITE_BACK);
	enable_mtrr();
	set_cr0(get_cr0() & ~(CR0_DISABLE_CACHE | CR0_NOT_WRITE_THROUGH));
	enable_l3_cache();
}

void reset_cache(void)
{
	wbinvd();
}

void init_cache(void)
{
	set_mtrr_def_type_msr(MSR_MTRR_DEF_TYPE_MEM_TYPE_UNCACHED);
	reset_cache();
}
