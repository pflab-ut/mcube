#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @file scripts/kconfig/dump.py
#
# @author Hiroyuki Chishiro
#
"""
transform binary file to dump file with considering big/little endian.
"""

import sys
import binascii
import struct

argv = sys.argv
argc = len(argv)

if argc != 4:
  print("Usage: %s filename eb[el]" % argv[0])
  quit()

#print(argv[1])
fin = open(argv[1], "rb")
fout = open(argv[3], "w")
#data = fin.read()
addr = 0
while True:
#  bytes_data = fin.read(4)
  data = fin.read(4)
  if not data:
    break  
  if argv[2] == "eb":
    data = int.from_bytes(data, 'big')
  else:
    data = int.from_bytes(data, 'little')
  fout.write("{:08x}".format(data))
  fout.write(" // 0x{:08x}".format(addr))
  fout.write('\n')
  addr += 4
#  print(format(data, "08x"))
#  print("str = ", s)
#  fout.write("{:08x}".format(data))
#  fout.write('\n')
#fout.close()
fin.close()
