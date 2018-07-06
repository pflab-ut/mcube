#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @file scripts/misc/line.py
#
# @author Hiroyuki Chishiro
#
"""
get line.
"""

import glob

total_len = 0

def count_line(files):
  global total_len
  for file in files:
    print(file)
    f = open(file)
    total_len += len(f.readlines())
    f.close()

count_line(glob.glob("*/*.c"))
count_line(glob.glob("*/*/*.h"))
count_line(glob.glob("*/*/*.py"))

print(total_len)
