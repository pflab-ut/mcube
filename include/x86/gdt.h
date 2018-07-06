/*
 * @file include/x86/gdt.h
 *
 * @author Hiroyuki Chishiro
 */
// Global Descriptor Table and Task State Segment structure definitions
// for 32-bit and 64-bit modes.
//
// Copyright 2016 Brett Vickers.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.
//=============================================================================

#ifndef __MCUBE_BOOT_GDT_INC__
#define __MCUBE_BOOT_GDT_INC__

//=============================================================================
// Constants
//=============================================================================

// GDT selectors, which should be used in protected mode to set segment
// register values. See loader_iso.asm for the descriptors referenced by these
// selectors.
#define GDT32_SELECTOR_CODE32       0x08    // 32-bit protected mode (code)
#define GDT32_SELECTOR_DATA32       0x10    // 32-bit protected mode (data)
#define GDT32_SELECTOR_CODE16       0x18    // 16-bit protected mode (code)
#define GDT32_SELECTOR_DATA16       0x20    // 16-bit protected mode (data)

#define GDT64_SELECTOR_KERNEL_DATA  0x08    // 64-bit mode (kernel data)
#define GDT64_SELECTOR_KERNEL_CODE  0x10    // 64-bit mode (kernel code)
#define GDT64_SELECTOR_USER_DATA    0x18    // 64-bit mode (user data)
#define GDT64_SELECTOR_USER_CODE    0x20    // 64-bit mode (user code)
#define GDT64_SELECTOR_TSS          0x28    // 64-bit task state segment


//=============================================================================
// GDT
//
// Each GDT entry is an 8-byte structure organized as follows:
//
//     31                   16 15                    0
//    +-----------------------+-----------------------+
//    |         Base          |         Limit         |
//    |         0:15          |          0:15         |
//    +-------+-------+-------+-----------+-----------+
//    | Base  | Flags | Limit |  Access   |    Base   |
//    | 24:31 |       | 16:19 |           |   16:23   |
//    +-------+-------+-------+-----------+-----------+
//
//         Bits
//       [0:15]      Limit bits [0:15]
//      [16:31]      Base address bits [0:15]
//      [32:39]      Base address bits [16:23]
//          40       Access: AC (Accessed) bit
//          41       Access: RW (Readable/Writable) bit
//          42       Access: DC (Direction/Conforming) bit
//          43       Access: EX (Executable) bit
//          44       Access: must be 1
//      [45:46]      Access: Privilege ring (0 = highest, 3 = lowest)
//          47       Access: PR (Present) bit
//      [48:51]      Limit bits [16:19]
//          52       Flags: must be 0
//          53       Flags: LM (Long Mode)
//          54       Flags: SZ (Size) bit (0=16-bit, 1=32-bit)
//          55       Flags: GR (Granularity) (0=1B, 1=4KiB limit granularity)
//      [56:63]      Base address bits [24:31]
//
//=============================================================================
.struct 0
GdtDescriptor:
  .comm GdtDescriptorLimitLow, 2
  .comm GdtDescriptorBaseLow, 2
  .comm GdtDescriptorBaseMiddle, 1
  .comm GdtDescriptorAccess, 1
  .comm GdtDescriptorLimitHighFlags, 1 // LimitHigh (4 bits) + Flags (4 bits)
  .comm GdtDescriptorBaseHigh, 1
GdtDescriptorEnd:
GdtDescriptorLength = GdtDescriptorEnd - GdtDescriptor:


//=============================================================================
// 64-bit TSS Descriptor
//
// Similar layout to GDT.Descriptor, but with two additional dwords, one of
// which is used to extend the base address.
//=============================================================================
.struct 0
TSS64Descriptor:
  .comm TSS64DescriptorLimitLow, 2
  .comm TSS64DescriptorBaseLow, 2
  .comm TSS64DescriptorBaseMiddle, 1
  .comm TSS64DescriptorAccess, 1
  .comm TSS64DescriptorLimitHighFlags, 1 // LimitHigh (4 bits) + Flags (4 bits)
  .comm TSS64DescriptorBaseHigh, 1
  .comm TSS64DescriptorBaseHighest, 4
  .comm TSS64DescriptorReserved, 4
TSS64DescriptorEnd:
TSS64DescriptorLength = TSS64DescriptorEnd - TSS64Descriptor:


//=============================================================================
// 64-bit TSS
//
// The 64-bit Task State Segment data structure.
//
// Contains information used during privilege mode changes.
//=============================================================================
.struct 0
TSS64:
  .comm TSS64Reserved, 4 // Reserved
  .comm TSS64RSP0, 8   // Stack pointer (priv 0)
  .comm TSS64RSP1, 8   // ...
  .comm TSS64RSP2, 8
  .comm TSS64Reserved2, 8   // Reserved
  .comm TSS64IST1, 8   // Interrupt Stack Table pointer 1
  .comm TSS64IST2, 8   // ...
  .comm TSS64IST3, 8
  .comm TSS64IST4, 8
  .comm TSS64IST5, 8
  .comm TSS64IST6, 8
  .comm TSS64IST7, 8
  .comm TSS64Reserved3, 8   // Reserved
  .comm TSS64Reserved4, 2   // Reserved
  .comm TSS64IOPB, 2   // I/O permission bitmap offset
TSS64End:
TSS64Legnth = TSS64End - TSS64:


#endif /* __MCUBE_BOOT_GDT_INC__ */
    
