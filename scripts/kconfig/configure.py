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

def init_sysconfig(algo, arch, compiler, machine, output):
  "initialize sysconfig"
  fout = open("./.sysconfig", "w")

  if arch == "sim":
    fout.write("ARCH_NAME = sim\n")
  elif arch == "x86":
    fout.write("ARCH_NAME = x86\n")
  elif arch == "arm":
    fout.write("ARCH_NAME = arm\n")
    if machine == "raspi3":
      fout.write("MACHINE_NAME = raspi3\n")
    elif machine == "synquacer":
      fout.write("MACHINE_NAME = synquacer\n")
    elif machine == "none":
      fout.write("MACHINE_NAME = none\n")
    else:
      print("Unknown Machine")
  elif arch == "axis":
    fout.write("ARCH_NAME = axis\n")
  else:
    print("Unknown Architecture")

  if compiler == "gcc":
    fout.write("CC = gcc\n")
  elif compiler == "clang":
    fout.write("CC = clang\n")
  else:
    print("Unknown Compiler")

  if output == "console":
    fout.write("OUTPUT_NAME = console\n")
  elif output == "uart":
    fout.write("OUTPUT_NAME = uart\n")
  else:
    print("Unknown Output")

  fout.write("ALGO_NAME = " + algo + "\n")

  fout.close()

def init_kconfig():
  "initialize kconfig"
  fout = open("./.kconfig", "w")
  fout.write(tools.DEFS + "\n\n")
  fout.write(tools.CFILES + "\n\n")
  fout.write(tools.AFILES + "\n")
  fout.close()

def init_config():
  "initialize config"
  fout = open("./include/mcube/config.h", "w")
  fout.write("/**\n"
             + " * @file include/mcube/config.h\n"
             + " *\n"
             + " * @author Hiroyuki Chishiro\n"
             + " */\n")
  fout.write("#ifndef __MCUBE_MCUBE_CONFIG_H__\n"
             + "#define __MCUBE_MCUBE_CONFIG_H__\n"
             + "/* Don't edit this file.\n"
             + " * Please edit configure then do make configure. */\n")

  for key, value, in tools.CONFIGURES.items():
    if key.find("CONFIG_ARCH") == 0:
      fout.write("/** Architecture. */\n")
    elif key.find("CONFIG_COMPILER") == 0:
      fout.write("/** Compiler. */\n")
    elif key.find("CONFIG_ALGO") == 0:
      fout.write("/** Algorithm. */\n")
    elif key.find("CONFIG_PRINT") == 0:
      fout.write("/** Print. */\n")
    elif key.find("CONFIG_TQ") == 0:
      fout.write("/** Task Queue. */\n")
    elif key.find("CONFIG_TIE_BREAK") == 0:
      fout.write("/** Tie-Break. */\n")
    elif key.find("CONFIG_OPTION") == 0:
      fout.write("/** Option. */\n")
    elif key.find("CONFIG_TARGET_DRIVE") == 0:
      fout.write("/** Target Drive. */\n")
    elif key.find("CONFIG_MEMORY") == 0:
      fout.write("/** Memory. */\n")
    if value == "y":
      fout.write("#define " + key + " 1\n")
    else:
      fout.write("#define " + key + " 0\n")

  fout.write("#endif /* __MCUBE_MCUBE_CONFIG_H__ */\n")
  fout.close()


def main():
  "main function"
  configure_file = "configure"
  kconfig_file = "Kconfig"

  tools.scan_configure(configure_file)

  algo = tools.scan_algo_name(kconfig_file)
  arch = tools.scan_arch_name(kconfig_file)
  compiler = tools.scan_compiler_name(kconfig_file)
  machine = tools.scan_machine_name(kconfig_file)
  output = tools.scan_output_name(kconfig_file)

  tools.scan_kconfig(kconfig_file)

  if not tools.check_exclusives(kconfig_file):
    sys.exit("Error: check_exclusives()")

  tools.scan_dependencies(kconfig_file)

  if not tools.check_conflicts_and_dependencies():
    sys.exit("Error: check_conflicts_and_dependencies()")

  init_sysconfig(algo, arch, compiler, machine, output)

  init_kconfig()
  init_config()


if __name__ == "__main__":
  main()
