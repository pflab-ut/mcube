/*
 * @file bios.inc
 *
 * @author Hiroyuki Chishiro
 */

// BIOS constants and structures, usable in real mode only.
//
// Copyright 2016 Brett Vickers.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.
//=============================================================================

#ifndef __MCUBE_BOOT_BIOS_INC__
#define __MCUBE_BOOT_BIOS_INC__


//=============================================================================
// Disk Address Packet (DAP)
//=============================================================================
.struct 0
BiosMap:
  .comm BiosMapBytes, 2   // size of DAP in bytes (16)
  .comm BiosMapReadSectors, 2   // number of sectors to read
  .comm BiosMapTargetBufferOffset, 2 // target buffer address offset
  .comm BiosMapTargetBufferSegment, 2   // target buffer address segment
  .comm BiosMapFirstSector, 8   // first sector to read (up to 8 bytes)
BiosMapEnd:
BiosMapLength = BiosMapEnd - BiosMap


#endif /* __MCUBE_BOOT_BIOS_INC__ */

