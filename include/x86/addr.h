/**
 * @file include/x86/addr.h
 *
 * @author Hiroyuki Chishiro
 */
#ifndef __MCUBE_X86_ADDR_H__
#define __MCUBE_X86_ADDR_H__


/* upper bound of kernel size including bss is about 600KB */
/* real kernel address is from 0x7c00
 * but this addr is set to 0x00000000 for gdtr */
#define KERNEL_START 0x00000000
#define KERNEL_END   0x000a0000


#define CUI_VRAM_START 0x000b8000
#define CUI_VRAM_END 0x000b8fa0 /* 80 * 25 * 2 = 4000 byte */
#define GUI_VRAM_START 0x000a0000
#define GUI_VRAM_END   0x000c0000



#define EXBIOS_START 0x000e0000
#define EXBIOS_END   0x000f0000
#define BIOS_START 0x000f0000
#define BIOS_END   0x00100000


#define IDT_START 0x0026f800
#define IDT_END 0x00270000
#define GDT_START 0x00270000
#define GDT_END 0x00280000

#define IOMAP_START 0x40000000

#define virt_to_phys(addr) (addr)
#define phys_to_virt(addr) (addr)

#define CYLS_ADDR 0xff0
#define LEDS_ADDR 0xff1
#define VMODE_ADDR 0xff2
#define SCRNX_ADDR 0xff4
#define SCRNY_ADDR 0xff6
#define VRAM_ADDR 0xff8



//#define IOREGSEL_START 0xfec00000 // 0xfec0xy00
//#define IOWIN_START 0xfec00010 // 0xfec0xy10

#define IOAPIC_START 0xfec00000
#define LAPIC_START 0xfee00000


#endif /* __MCUBE_X86_ADDR_H__ */
