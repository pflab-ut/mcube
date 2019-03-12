#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @file scripts/kconfig/defaultconfig.py
#
# @author Hiroyuki Chishiro
#
"""
set up default configuration.
"""

import tools

def main():
  "main function"
  tools.save_configure("Kconfig")

if __name__ == "__main__":
  main()
