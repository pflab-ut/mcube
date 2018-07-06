/*
 * @file globals.inc
 *
 * @author Hiroyuki Chishiro
 */

// Global variables used by boot loaders.
//
// Copyright 2016 Brett Vickers.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.
//=============================================================================

#ifndef __MONK_BOOT_GLOBALS_INC__
#define __MONK_BOOT_GLOBALS_INC__


//=============================================================================
// Global variables (stored at Mem.Boot.Globals)
//=============================================================================
.struct 0
Globals:
  .comm GlobalsDriveNumber, 4
  .comm GlobalsRootDirectorySector, 2
  .comm GlobalsKernelSector, 2
  .comm GlobalsKernelSize, 4  
  .comm GlobalsCpuFeatureBitsEcx, 4
  .comm GlobalsCpuFeatureBitsEdx, 4
GlobalsEnd:
GlobalsLength = GlobalsEnd - Globals

#endif /* __MONK_BOOT_GLOBALS_INC__ */
   
