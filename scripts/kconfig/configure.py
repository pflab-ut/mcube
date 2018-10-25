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

configure_file = "configure"
kconfig_file = "Kconfig"

tools.scan_configure(configure_file)

arch = tools.scan_arch_name(kconfig_file)
compiler = tools.scan_compiler_name(kconfig_file)
algo = tools.scan_algo_name(kconfig_file)
machine = tools.scan_machine_name(kconfig_file)

tools.scan_configs(kconfig_file)

if not tools.check_exclusives(kconfig_file):
  sys.exit("Error: check_exclusives()")

tools.scan_dependencies(kconfig_file)

if not tools.check_conflicts_and_dependencies():
  sys.exit("Error: check_conflicts_and_dependencies()")


f = open("./.sysconfig", "w")

if arch == "sim":
  f.write("ARCH_NAME = sim\n")
elif arch == "x86":
  f.write("ARCH_NAME = x86\n")
elif arch == "arm":
  f.write("ARCH_NAME = arm\n")
  if machine == "raspi3":
    f.write("MACHINE_NAME = raspi3\n")
  elif machine == "synquacer":
    f.write("MACHINE_NAME = synquacer\n")
  elif machine == "none":
    f.write("MACHINE_NAME = none\n")
  else:
    print("Unknown Machine")
elif arch == "axis":
  f.write("ARCH_NAME = axis\n")
else:
  print("Unknown Architecture")

if compiler == "gcc":
  f.write("CC = gcc\n")
elif compiler == "clang":
  f.write("CC = clang\n")
else:
  print("Unknown Compiler")

f.write("ALGO_NAME = " + algo + "\n")

f.close()


f = open("./.kconfig", "w")
f.write(tools.defs + "\n\n")
f.write(tools.cfiles + "\n\n")
f.write(tools.afiles + "\n")
f.close()


f = open("./include/mcube/config.h", "w")
f.write("/**\n"
        + " * @file include/mcube/config.h \n"
        + " *\n"
        + " * @author Hiroyuki Chishiro\n"
        + " */\n")
f.write("#ifndef __MCUBE_MCUBE_CONFIG_H__\n"
        + "#define __MCUBE_MCUBE_CONFIG_H__\n"
        + "/* Don't edit this file.\n"
        + " * Please edit configure then do make configure. */\n")


for k, v, in tools.configures.items():
  if v == "y":
    if k.find("CONFIG_ARCH") == 0:
      f.write("/** Architecture. */\n")
    elif k.find("CONFIG_COMPILER") == 0:
      f.write("/** Compiler. */\n")
    elif k.find("CONFIG_ALGO") == 0:
      f.write("/** Algorithm. */\n")
    elif k.find("CONFIG_PRINT") == 0:
      f.write("/** Print. */\n")
    elif k.find("CONFIG_TQ") == 0:
      f.write("/** Task Queue. */\n")
    elif k.find("CONFIG_TIE_BREAK") == 0:
      f.write("/** Tie-Break. */\n")
    elif k.find("CONFIG_OPTION") == 0:
      f.write("/** Option. */\n")
    elif k.find("CONFIG_TARGET_DRIVE") == 0:
      f.write("/** Target Drive. */\n")
    elif k.find("CONFIG_MEMORY") == 0:
      f.write("/** Memory. */\n")
    f.write("#define " + k + " 1\n")

f.write("#endif /* __MCUBE_MCUBE_CONFIG_H__ */\n")
f.close()

