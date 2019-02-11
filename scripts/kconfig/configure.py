#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @file scripts/kconfig/configure.py
#
# @author Hiroyuki Chishiro
#
"""
set up configuration.
"""

import sys
import tools

CONFIGURE_FILE = "configure"
KCONFIG_FILE = "Kconfig"

tools.scan_configure(CONFIGURE_FILE)

ARCH = tools.scan_arch_name(KCONFIG_FILE)
COMPILER = tools.scan_compiler_name(KCONFIG_FILE)
ALGO = tools.scan_algo_name(KCONFIG_FILE)
MACHINE = tools.scan_machine_name(KCONFIG_FILE)

tools.scan_kconfig(KCONFIG_FILE)

if not tools.check_exclusives(KCONFIG_FILE):
  sys.exit("Error: check_exclusives()")

tools.scan_dependencies(KCONFIG_FILE)

if not tools.check_conflicts_and_dependencies():
  sys.exit("Error: check_conflicts_and_dependencies()")


FOUT = open("./.sysconfig", "w")

if ARCH == "sim":
  FOUT.write("ARCH_NAME = sim\n")
elif ARCH == "x86":
  FOUT.write("ARCH_NAME = x86\n")
elif ARCH == "arm":
  FOUT.write("ARCH_NAME = arm\n")
  if MACHINE == "raspi3":
    FOUT.write("MACHINE_NAME = raspi3\n")
  elif MACHINE == "synquacer":
    FOUT.write("MACHINE_NAME = synquacer\n")
  elif MACHINE == "none":
    FOUT.write("MACHINE_NAME = none\n")
  else:
    print("Unknown Machine")
elif ARCH == "axis":
  FOUT.write("ARCH_NAME = axis\n")
else:
  print("Unknown Architecture")

if COMPILER == "gcc":
  FOUT.write("CC = gcc\n")
elif COMPILER == "clang":
  FOUT.write("CC = clang\n")
else:
  print("Unknown Compiler")

FOUT.write("ALGO_NAME = " + ALGO + "\n")

FOUT.close()


FOUT2 = open("./.kconfig", "w")
FOUT2.write(tools.DEFS + "\n\n")
FOUT2.write(tools.CFILES + "\n\n")
FOUT2.write(tools.AFILES + "\n")
FOUT2.close()


FOUT3 = open("./include/mcube/config.h", "w")
FOUT3.write("/**\n"
            + " * @file include/mcube/config.h \n"
            + " *\n"
            + " * @author Hiroyuki Chishiro\n"
            + " */\n")
FOUT3.write("#ifndef __MCUBE_MCUBE_CONFIG_H__\n"
            + "#define __MCUBE_MCUBE_CONFIG_H__\n"
            + "/* Don't edit this file.\n"
            + " * Please edit configure then do make configure. */\n")


for k, v, in tools.CONFIGURES.items():
  if k.find("CONFIG_ARCH") == 0:
    FOUT3.write("/** Architecture. */\n")
  elif k.find("CONFIG_COMPILER") == 0:
    FOUT3.write("/** Compiler. */\n")
  elif k.find("CONFIG_ALGO") == 0:
    FOUT3.write("/** Algorithm. */\n")
  elif k.find("CONFIG_PRINT") == 0:
    FOUT3.write("/** Print. */\n")
  elif k.find("CONFIG_TQ") == 0:
    FOUT3.write("/** Task Queue. */\n")
  elif k.find("CONFIG_TIE_BREAK") == 0:
    FOUT3.write("/** Tie-Break. */\n")
  elif k.find("CONFIG_OPTION") == 0:
    FOUT3.write("/** Option. */\n")
  elif k.find("CONFIG_TARGET_DRIVE") == 0:
    FOUT3.write("/** Target Drive. */\n")
  elif k.find("CONFIG_MEMORY") == 0:
    FOUT3.write("/** Memory. */\n")
  if v == "y":
    FOUT3.write("#define " + k + " 1\n")
  else:
    FOUT3.write("#define " + k + " 0\n")

FOUT3.write("#endif /* __MCUBE_MCUBE_CONFIG_H__ */\n")
FOUT3.close()
