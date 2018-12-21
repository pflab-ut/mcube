#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @file scripts/misc/common.py
#
# @author Hiroyuki Chishiro
#
"""
Common function for misc.
"""

def check_argc(argc, argv, correct_argc):
  "check argc"
  if argc != correct_argc:
    print("Usage: %s filename eb[el]" % argv[0])
    quit()
