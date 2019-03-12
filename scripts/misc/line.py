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

TOTAL_LEN = 0

def count_line(files):
  """
  Count line.
  """
  global TOTAL_LEN
  for filename in files:
    print(filename)
    fname = open(filename)
    TOTAL_LEN += len(fname.readlines())
    fname.close()

def main():
  "main function"
  count_line(glob.glob("*/*.c"))
  count_line(glob.glob("*/*/*.h"))
  count_line(glob.glob("*/*/*.py"))

  print(TOTAL_LEN)

if __name__ == "__main__":
  main()
