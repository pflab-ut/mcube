/**
 * @file include/x86/x86_header.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef	__MCUBE_X86_X86_HEADER_H__
#define	__MCUBE_X86_X86_HEADER_H__

#include <x86/kernel.h>
#include <x86/list.h>
#include <x86/mmio.h>
#include <x86/msr.h>
#include <x86/paging.h>
#include <x86/x86.h>
#include <x86/sched.h>
#include <x86/segment.h>
#include <x86/spinlock.h>
#include <x86/stat.h>
#include <x86/stdint.h>
#include <x86/string.h>
#include <x86/unrolled_list.h>
#include <x86/vm.h>

#include <x86/apic.h>
#include <x86/atomic.h>
#include <x86/bitmap.h>
#include <x86/buffer_dumper.h>
#include <x86/conf_sched.h>
#include <x86/e820.h>
#include <x86/errno.h>
#include <x86/ext2.h>
#include <x86/fcntl.h>
#include <x86/file.h>
#include <x86/hash.h>
#include <x86/idt.h>
#include <x86/io.h>
#include <x86/ioapic.h>
#include <x86/keyboard.h>
#include <x86/kmalloc.h>
#include <x86/mm.h>
#include <x86/mptables.h>
#include <x86/percpu.h>
#include <x86/pic.h>
#include <x86/pit.h>
#include <x86/proc.h>
#include <x86/ramdisk.h>
#include <x86/sections.h>
#include <x86/serial.h>
#include <x86/smpboot.h>
#include <x86/tests.h>
#include <x86/tsc.h>
#include <x86/unistd.h>
#include <x86/vectors.h>
#include <x86/vga.h>

#ifndef __ASSEMBLY__
struct cpu_context {
};
typedef struct interrupt_context {
} interrupt_context_t;

static inline void enable_local_irq(void)
{
}
static inline void disable_local_irq(void)
{
}
static inline void save_local_irq(unsigned long *flags)
{
}
static inline void restore_local_irq(unsigned long *flags)
{
}
static inline void mcube_spin_lock(volatile atomic_int *lock)
{
}
static inline void mcube_spin_unlock(volatile atomic_int *lock)
{
}
static inline int mcube_spin_trylock(volatile atomic_int *lock)
{
  return 0;
}
static inline unsigned long get_current_cpu_time(void)
{
  return 0;
}
static inline unsigned long tsc2usec(unsigned long tsc)
{
  return 0;
}
static inline unsigned long tsc2nsec(unsigned long tsc)
{
  return 0;
}

#endif


#endif	/* __MCUBE_X86_X86_HEADER_H__ */
