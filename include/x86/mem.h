/*
 * @file include/x86/mem.h
 *
 * @author Hiroyuki Chishiro
 */

// Memory layout constants and macros used by boot loader code.
//
// Copyright 2016 Brett Vickers.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.
//=============================================================================

#ifndef __MCUBE_BOOT_MEM_INC__
#define __MCUBE_BOOT_MEM_INC__

//=============================================================================
// Memory layout
//=============================================================================

#define MEM_BIOS_IVT                       0x00000000
#define MEM_BIOS_DATA                      0x00000400
#define MEM_SECTOR_BUFFER                  0x00000800
#define MEM_GDT                            0x00003000
#define MEM_TSS64                          0x00003100
#define MEM_GLOBALS                        0x00003200
#define MEM_STACK_BOTTOM                   0x00004000
#define MEM_STACK_TOP                      0x00007c00
#define MEM_LOADER1                        0x00007c00
#define MEM_LOADER2                        0x00008000
#define MEM_PAGE_TABLE                     0x00010000
#define MEM_PAGE_TABLE_PML4T               0x00010000
#define MEM_PAGE_TABLE_PDPT                0x00011000
#define MEM_PAGE_TABLE_PDT                 0x00012000  // maps first 10MiB
#define MEM_PAGE_TABLE_PT                  0x00013000  // maps first 2MiB
#define MEM_PAGE_TABLE_END                 0x00020000
#define MEM_STACK32_TEMP_BOTTOM            0x0006f000
#define MEM_STACK32_TEMP_TOP               0x00070000
#define MEM_TABLE                          0x00070000  // BIOS-derived layout
#define MEM_KERNEL_LOAD_BUFFER             0x00070000
#define MEM_KERNEL_STACK_NMI_BOTTOM        0x0008a000  // NMI stack
#define MEM_KERNEL_STACK_NMI_TOP           0x0008c000
#define MEM_KERNEL_STACK_DF_BOTTOM         0x0008c000  // double-fault stack
#define MEM_KERNEL_STACK_DF_TOP            0x0008e000
#define MEM_KERNEL_STACK_MC_BOTTOM         0x0008e000  // machine-check stack
#define MEM_KERNEL_STACK_MC_TOP            0x00090000
#define MEM_BIOS_EBDA                      0x0009e000
#define MEM_VIDEO                          0x000a0000
#define MEM_KERNEL_STACK_INTERRUPT_BOTTOM  0x00100000  // PL-change intr stack
#define MEM_KERNEL_STACK_INTERRUPT_TOP     0x001ff000
#define MEM_KERNEL_STACK_BOTTOM            0x00200000  // main kernel stack
#define MEM_KERNEL_STACK_TOP               0x00300000
#define MEM_KERNEL_IMAGE                   0x00300000
#define MEM_KERNEL_CODE                    0x00301000

// Layout region sizes
#define MEM_BIOS_IVT_SIZE                  0x00000400
#define MEM_BIOS_DATA_SIZE                 0x00000100
#define MEM_LOADER1_SIZE                   0x00000200
#define MEM_LOADER2_SIZE                   0x00008000
#define MEM_SECTOR_BUFFER_SIZE             0x00000800
#define MEM_TABLE_SIZE                     0x00006000  // Up to 1023 regions
#define MEM_KERNEL_LOADBUFFER_SIZE         0x00010000

// Real mode segment addresses
#define MEM_LOADER1_SEGMENT (MEM_LOADER1 >> 4)
#define MEM_LOADER2_SEGMENT (MEM_LOADER2 >> 4)


#endif /* __MCUBE_BOOT_MEM_INC__ */

