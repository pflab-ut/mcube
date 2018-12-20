#!/usr/bin/env python3
#
# @file scripts/misc/vmdk.py
#
# @author Hiroyuki Chishiro
#
"""
create vmdk file.
"""

import sys
import struct
import os

import tools

ARGV = sys.argv
ARGC = len(ARGV)

tools.check_argc(ARGC, ARGV, 4)

FIN = open(ARGV[1], "rb")
FOUT = open(ARGV[2], "wb")
FOUT2 = open(ARGV[3], "w")

while True:
  BYTE_DATA = FIN.read(4)
  if not BYTE_DATA:
    break
  FOUT.write(BYTE_DATA)

SECTOR_SIZE = 512
NR_SECTORS = 63
NR_HEADS = 4
#NR_CYLINDERS = 16
NR_CYLINDERS = 1
#NR_CYLINDERS = 2


# kernel size's upper bound
KBOUND = SECTOR_SIZE * NR_SECTORS * NR_HEADS * NR_CYLINDERS

KSIZE = os.path.getsize(ARGV[1])
PADSIZE = KBOUND - KSIZE
#PADSIZE = SECTOR_SIZE - KSIZE % SECTOR_SIZE
#print("PADSIZE = ", PADSIZE)
if PADSIZE >= 0:
  while PADSIZE > 0:
    #FOUT.putc(0x5a)
    FOUT.write(struct.pack("B", 0))
    PADSIZE -= 1
  KSIZE += 1
else:
  print("Error: too much kernel size > vmdksize: {0} > {1}".format(KSIZE, KBOUND))
  quit()

#CYLINDERS = (KSIZE / SECTOR_SIZE / NR_SECTORS / NR_HEADS).ceil
VAL = int(KBOUND / SECTOR_SIZE)

STRING = f"""
# Disk DescriptorFile
version=1
encoding="Shift_JIS"
CID=fffffffe
parentCID=ffffffff
isNativeSnapshot="no"
createType="monolithicFlat"

# Extent description
RW {VAL} FLAT "mcube-flat.vmdk" 0

# The Disk Data Base 
#DDB

ddb.virtualHWVersion = "7"
ddb.longContentID = "43f26955e1fda75f021ce55bfffffffe"
ddb.uuid = "60 00 C2 97 a9 2c 72 18-0e 68 57 c2 ea 07 7a 6a"
ddb.geometry.sectors = "{NR_SECTORS}"
ddb.geometry.heads = "{NR_HEADS}"
ddb.geometry.cylinders = "{NR_CYLINDERS}"
ddb.adapterType = "ide"
"""

FOUT2.write(STRING)

FOUT2.close()
FOUT.close()
FIN.close()
