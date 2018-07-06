/*
 * @file iso9660.inc
 *
 * @author Hiroyuki Chishiro
 */

// ISO9660 constants and structures.
//
// Copyright 2016 Brett Vickers.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.
//=============================================================================

#ifndef __MCUBE_BOOT_ISO9660_INC__
#define __MCUBE_BOOT_ISO9660_INC__


//=============================================================================
// PrimaryVolumeDescriptor
//=============================================================================
.struct 0
IsoPrimaryVolumeDescriptor:
  .comm IsoPrimaryVolumeDescriptorType, 1
  .comm IsoPrimaryVolumeDescriptorIdentifier, 5
  .comm IsoPrimaryVolumeDescriptorVersion, 1
  .comm IsoPrimaryVolumeDescriptorUnused, 1   // unused
   //.align 8
  .comm IsoPrimaryVolumeDescriptorSystemId, 32
  .comm IsoPrimaryVolumeDescriptorVolumeId, 32
  .comm IsoPrimaryVolumeDescriptorUnused2, 8 // unused
   // .align 8
  .comm IsoPrimaryVolumeDescriptorVolumeSpaceSize, 8 // int32 LSB-MSB
  .comm IsoPrimaryVolumeDescriptorUnused3, 32  // unused
  .comm IsoPrimaryVolumeDescriptorVolumeSetSize, 4 // int16 LSB-MSB
  .comm IsoPrimaryVolumeDescriptorVolumeSeqNum, 4 // int16 LSB-MSB
  .comm IsoPrimaryVolumeDescriptorBlockSize, 4 // int16 LSB-MSB
  .comm IsoPrimaryVolumeDescriptorPathTableSize, 8 // int32 LSB-MSB
  .comm IsoPrimaryVolumeDescriptorPathTableLba, 4 // int32 LSB
  .comm IsoPrimaryVolumeDescriptorPathTableLbaOpt, 4 // int32 LSB
  .comm IsoPrimaryVolumeDescriptorMpathTableLba, 4 // int32 MSB
  .comm IsoPrimaryVolumeDescriptorMpathTableLbaOpt, 4 // int32 MSB
  .comm IsoPrimaryVolumeDescriptorRootDirEntry, 34 // contains an ISO.DirectoryEntry
  .comm IsoPrimaryVolumeDescriptorVolumeSetId, 128
  .comm IsoPrimaryVolumeDescriptorPublisher, 128
  .comm IsoPrimaryVolumeDescriptorDataPreparer, 128
  .comm IsoPrimaryVolumeDescriptorAppId, 128
  .comm IsoPrimaryVolumeDescriptorCopyright, 38
  .comm IsoPrimaryVolumeDescriptorAbstractFileId, 36
  .comm IsoPrimaryVolumeDescriptorBiblioFileId, 37
  .comm IsoPrimaryVolumeDescriptorCreationDateTime, 17
  .comm IsoPrimaryVolumeDescriptorModifyDateTime, 17
  .comm IsoPrimaryVolumeDescriptorExpireDateTime, 17
  .comm IsoPrimaryVolumeDescriptorEffectiveDateTime, 17
  .comm IsoPrimaryVolumeDescriptorFileStructVersion, 1
  .comm IsoPrimaryVolumeDescriptorUnused4, 1   // unused
  .comm IsoPrimaryVolumeDescriptorAppData, 512
  .comm IsoPrimaryVolumeDescriptorReserved, 653
IsoPrimaryVolumeDescriptorEnd:
IsoPrimaryVolumeDescriptorLength = IsoPrimaryVolumeDescriptorEnd - IsoPrimaryVolumeDescriptor


//=============================================================================
// DirectoryEntry
//=============================================================================
.struct 0
IsoDirectoryEntry:
  .comm IsoDirectoryEntryRecordLength, 1
  .comm IsoDirectoryEntryExtAttribLength, 1
  .comm IsoDirectoryEntryLocationLba, 8 // int32 LSB-MSB
  .comm IsoDirectoryEntryLocationSize, 8 // int32 LSB-MSB
  .comm IsoDirectoryEntryRecordingDateTime, 7
  .comm IsoDirectoryEntryFileFlags, 1
  .comm IsoDirectoryEntryFileUnitSize, 1
  .comm IsoDirectoryEntryGapSize, 1
  .comm IsoDirectoryEntryVolume, 4 // int16 LSB-MSB
  .comm IsoDirectoryEntryNameLength, 1
  .comm IsoDirectoryEntryName, 32 // variable length (up to 32 in this implementation)
IsoDirectoryEntryEnd:
IsoDirectoryEntryLength = IsoDirectoryEntryEnd - IsoDirectoryEntry

  
#endif /* __MCUBE_BOOT_ISO9660_INC__ */

