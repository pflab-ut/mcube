/**
 * @file include/x86/addr.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_ADDR_H__
#define __MCUBE_X86_ADDR_H__


#define BOOTLOADER_STACK_START       0x00000500
#define BOOTLOADER_STACK_END         0x00007c00
#define MBR_START                    0x00007c00
#define MBR_END                      0x00007e00
/* [0x00007e00, 0x00008000) reserved */
#define BOOTINFO_START               0x00008000
#define BOOTINFO_END                 0x00009000
#define BOOTMON_START                0x00009000
#define BOOTMON_END                  0x0000d000
/* [0x0000d000, 0x00010000) reserved */
#define KERNEL_START                 0x00010000
#define KERNEL_END                   0x00030000
/* [0x00030000, 0x00069000) reserved */
#define KERNEL_BASE_PAGE_TABLE_START 0x00069000
#define KERNEL_BASE_PAGE_TABLE_END   0x00070000
#define TRAMPOLINE_START             0x00070000
#define TRAMPOLINE_END               0x00074000
#define GDT_START                    0x00074000
#define GDT_END                      0x00076000
#define IDT_START                    0x00076000
#define IDT_END                      0x00078000
/* [0x00078000, 0x00079000) reserved */
#define KERNEL_PAGE_TABLE64_START    0x00079000
#define KERNEL_PAGE_TABLE64_END      0x00080000



//#define IOREGSEL_START 0xfec00000 // 0xfec0xy00
//#define IOWIN_START 0xfec00010 // 0xfec0xy10

#define IOAPIC_START 0xfec00000
#define LAPIC_START 0xfee00000


#endif /* __MCUBE_X86_ADDR_H__ */
