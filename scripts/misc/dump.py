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


ARGV = sys.argv
ARGC = len(ARGV)

if ARGC != 4:
  print("Usage: %s filename eb[el]" % ARGV[0])
  quit()


#print(argv[1])
FIN = open(ARGV[1], "rb")
FOUT = open(ARGV[3], "w")
#data = fin.read()
ADDR = 0
while True:
#  bytes_data = fin.read(4)
  DATA = FIN.read(4)
  if not DATA:
    break
  if ARGV[2] == "eb":
    DATA = int.from_bytes(DATA, 'big')
  else:
    DATA = int.from_bytes(DATA, 'little')
  FOUT.write("{:08x}".format(DATA))
  FOUT.write(" // 0x{:08x}".format(ADDR))
  FOUT.write('\n')
  ADDR += 4
#  print(format(data, "08x"))
#  print("str = ", s)
#  FOUT.write("{:08x}".format(data))
#  FOUT.write('\n')
#FOUT.close()
FIN.close()
