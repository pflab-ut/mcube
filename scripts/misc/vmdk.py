#!/usr/bin/env python3
# -*- coding: utf-8 -*-
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

def main():
  "main function"
  argv = sys.argv
  argc = len(argv)

  if argc != 4:
    print("Usage: %s filename eb[el]" % argv[0])
    quit()

  fin = open(argv[1], "rb")
  fout = open(argv[2], "wb")
  fout2 = open(argv[3], "w")

  while True:
    byte_data = fin.read(4)
    if not byte_data:
      break
    fout.write(byte_data)

  sector_size = 512
  nr_sectors = 63
  nr_heads = 4
  #nr_cylinders = 16
  nr_cylinders = 1
  #nr_cylinders = 2

  # kernel size's upper bound
  kbound = sector_size * nr_sectors * nr_heads * nr_cylinders
  ksize = os.path.getsize(argv[1])
  padsize = kbound - ksize
  #padsize = sector_size - ksize % sector_size
  #print("padsize = ", padsize)
  if padsize >= 0:
    while padsize > 0:
      #fout.putc(0x5a)
      fout.write(struct.pack("B", 0))
      padsize -= 1
    ksize += 1
  else:
    print("Error: too much kernel size > vmdksize: {0} > {1}".format(ksize, kbound))
    quit()

  #CYLINDERS = (ksize / sector_size / nr_sectors / nr_heads).ceil
  val = int(kbound / sector_size)

  string = f"""
  # Disk DescriptorFile
  version=1
  encoding="Shift_JIS"
  CID=fffffffe
  parentCID=ffffffff
  isNativeSnapshot="no"
  createType="monolithicFlat"
  
  # Extent description
  RW {val} FLAT "mcube-flat.vmdk" 0
  
  # The Disk Data Base 
  #DDB
  
  ddb.virtualHWVersion = "7"
  ddb.longContentID = "43f26955e1fda75f021ce55bfffffffe"
  ddb.uuid = "60 00 C2 97 a9 2c 72 18-0e 68 57 c2 ea 07 7a 6a"
  ddb.geometry.sectors = "{nr_sectors}"
  ddb.geometry.heads = "{nr_heads}"
  ddb.geometry.cylinders = "{nr_cylinders}"
  ddb.adapterType = "ide"
  """

  fout2.write(string)

  fout2.close()
  fout.close()
  fin.close()

if __name__ == "__main__":
  main()
