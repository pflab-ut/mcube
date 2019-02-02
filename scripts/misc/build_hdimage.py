#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @file scripts/misc/build_hdimage.py
#
# @author Hiroyuki Chishiro
#
# Copyright (C) 2012 Ahmed S. Darwish <darwish.07@gmail.com>
#
# Usage: $script
#        dd if=data/hd-image of=/dev/disk/by-label/$CUTE-DISK-LABEL
#
# Build the Cute kernel final image, which is going to be directly
# written to disk. That final image is a concatenation of:
#  o a (possibly extended) 512-KB Kernel image
#  o and, a 24-byte ramdisk header
#  o and, the Ramdisk image itself (if any)
#
# The 24-byte Ramdisk header consists of:
#  o start signature, "CUTE-STA", 8-bytes
#  o ramdisk size in number of 512-byte sectors, 4-bytes
#  o ramdisk length (minus the header) in bytes, 4-bytes
#  o end signature, "CUTE-END", 8-bytes
#
# As seen above, ramdisk length is expressed in two terms:
# o in 512-byte sectors (including header len), for the real-mode
#   assembly code loading the header and its ramdisk image to RAM.
# o in plain bytes (without  header len), for the C code accssing
#   the ramdisk image directly.
#
# Python-2.7 _AND_ Python-3.0+ compatible
# NOTE! Always read & write the files in binary mode.
#
"""
build HD image.
"""

import os.path
import struct

BUILD_FOLDER = 'build/'
KERNEL_FILE = BUILD_FOLDER + 'mcube.img'
RAMDISK_FILE = './ramdisk'
FINAL_IMAGE = BUILD_FOLDER + 'mcube-hd.img'

if not os.path.exists(BUILD_FOLDER):
  os.makedirs(BUILD_FOLDER)

# Expand kernel image to 512 Kbytes
SIZE_512K = 512 * 1024
with open(KERNEL_FILE, 'rb') as f:
  BUF1 = f.read()
with open(FINAL_IMAGE, 'wb') as f:
  f.write(BUF1)
  f.write(b'0' * SIZE_512K)
  f.truncate(SIZE_512K)

# Build ramdisk header, and its buffer
HEADER_LENGTH = 8 + 4 + 4 + 8
RAMDISK_LENGTH = HEADER_LENGTH
if os.path.exists(RAMDISK_FILE):
  RAMDISK_LENGTH += os.path.getsize(RAMDISK_FILE)
  RAMDISK_BUFFER = open(RAMDISK_FILE, 'rb').read()
else:
  RAMDISK_BUFFER = b''
RAMDISK_SECTORS = (RAMDISK_LENGTH - 1)//512 + 1
RAMDISK_HEADER = struct.pack('=8cII8c',
                             b'C', b'U', b'T', b'E', b'-', b'S', b'T', b'A',
                             RAMDISK_SECTORS,
                             RAMDISK_LENGTH - HEADER_LENGTH,
                             b'C', b'U', b'T', b'E', b'-', b'E', b'N', b'D')
assert len(RAMDISK_HEADER) == HEADER_LENGTH

# Expand final disk image beyond 1MB: some virtual
# machine BIOSes fail if that image len is < 1MB
SIZE_1MB = 1024 * 1024
with open(FINAL_IMAGE, 'ab') as f:
  f.write(RAMDISK_HEADER)
  f.write(RAMDISK_BUFFER)
  f.write(b'#' * SIZE_1MB)
