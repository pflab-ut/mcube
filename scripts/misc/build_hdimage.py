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
#  o start signature, "McubeSta", 8-bytes
#  o ramdisk size in number of 512-byte sectors, 4-bytes
#  o ramdisk length (minus the header) in bytes, 4-bytes
#  o end signature, "McubeEnd", 8-bytes
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

def main():
  "main function"
  build_folder = 'build/'
  kernel_file = build_folder + 'mcube.img'
  ramdisk_file = './ramdisk'
  final_image = build_folder + 'mcube-hd.img'

  if not os.path.exists(build_folder):
    os.makedirs(build_folder)

  # Expand kernel image to 512 Kbytes
  size_512k = 512 * 1024
  with open(kernel_file, 'rb') as kfile:
    buf = kfile.read()
  with open(final_image, 'wb') as fimage:
    fimage.write(buf)
    fimage.write(b'0' * size_512k)
    fimage.truncate(size_512k)

  # Build ramdisk header, and its buffer
  header_length = 8 + 4 + 4 + 8
  ramdisk_length = header_length
  if os.path.exists(ramdisk_file):
    ramdisk_length += os.path.getsize(ramdisk_file)
    ramdisk_buffer = open(ramdisk_file, 'rb').read()
  else:
    ramdisk_buffer = b''
  ramdisk_sectors = (ramdisk_length - 1)//512 + 1
  ramdisk_header = struct.pack('=8cII8c',
                               b'M', b'c', b'u', b'b', b'e', b'S', b't', b'a',
                               ramdisk_sectors,
                               ramdisk_length - header_length,
                               b'M', b'c', b'u', b'b', b'e', b'E', b'n', b'd')
  assert len(ramdisk_header) == header_length

  # Expand final disk image beyond 1MB: some virtual
  # machine BIOSes fail if that image len is < 1MB
  size_1mb = 1024 * 1024
  with open(final_image, 'ab') as fimage:
    fimage.write(ramdisk_header)
    fimage.write(ramdisk_buffer)
    fimage.write(b'#' * size_1mb)

if __name__ == "__main__":
  main()
