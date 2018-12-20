#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @file scripts/kconfig/tools.py
#
# @author Hiroyuki Chishiro
#
"""
Tool for kconfig.
"""

import re
import sys
import smtplib

from email.mime.text import MIMEText

class ConfigInfo:
  """
  ConfigInfo class has config information.
  """
  def __init__(self):
    self.__menu = ""
    self.__type = ""
    self.__configure = []
    self.__num = 0

  def get_menu(self):
    "get menu"
    return self.__menu
  def get_type(self):
    "get type"
    return self.__type
  def get_configure(self):
    "get configure"
    return self.__configure
  def get_num(self):
    "get num"
    return self.__num
  def set_menu(self, menu):
    "set menu"
    self.__menu = menu
  def set_type(self, _type):
    "set type"
    self.__type = _type
  def set_configure(self, configure):
    "set configure"
    self.__configure = configure
  def set_num(self, num):
    "set num"
    self.__num = num


CONFIGURES = {}
CONFIGURE_DEFS = {}
TITLES = {}
DEFS = "CFLAGS += "
CFILES = "SRCS += "
AFILES = "ASRCS += "
VPATHS = "vpath %.cpp"
DEPENDENCIES = {}
CONFLICTS = {}
TYPE = "none"

CONFIGURE_LINE = r"^(.*)=(.)"
RE_CONFIGURE_LINE = re.compile(CONFIGURE_LINE)

COMMENT_LINE = r"^\#"
RE_COMMENT_LINE = re.compile(COMMENT_LINE)

ALGO_LINE = r"\s+alname\s+(.*)"
RE_ALGO_LINE = re.compile(ALGO_LINE)

MACHINE_LINE = r"\s+mname\s+(.*)"
RE_MACHINE_LINE = re.compile(MACHINE_LINE)

KCONFIG_LINE = r"\s+config\s+(.*)"
RE_KCONFIG_LINE = re.compile(KCONFIG_LINE)

ARCH_LINE = r"\s+arname\s+(.*)"
RE_ARCH_LINE = re.compile(ARCH_LINE)

ARCH_LINE = r"\s+aname\s+(.*)"
RE_ARCH_LINE = re.compile(ARCH_LINE)

CC_LINE = r"\s+ccname\s+(.*)"
RE_CC_LINE = re.compile(CC_LINE)

SKIP_LINE = r"^\#|^\n|^menu|^\s+title|^\s+default"
RE_SKIP_LINE = re.compile(SKIP_LINE)

DEFS_LINE = r"^\s+defs\s+(.*)"
RE_DEFS_LINE = re.compile(DEFS_LINE)

CFILES_LINE = r"^\s+cfiles\s+(.*)"
RE_CFILES_LINE = re.compile(CFILES_LINE)

AFILES_LINE = r"^\s+afiles\s+(.*)"
RE_AFILES_LINE = re.compile(AFILES_LINE)

VPATHS_LINE = r"^\s+vpaths\s+(.*)"
RE_VPATHS_LINE = re.compile(VPATHS_LINE)

TYPE_LINE = r"^\s+type\s+(.*)"
RE_TYPE_LINE = re.compile(TYPE_LINE)

MENU_LINE = r"^menu\s+(.*)"
RE_MENU_LINE = re.compile(MENU_LINE)

ENDMENU_LINE = r"^endmenu\s+(.*)"
RE_ENDMENU_LINE = re.compile(ENDMENU_LINE)

DEPENDENCIES_LINE = r"^\s+(depends\s+)"
RE_DEPENDENCIES_LINE = re.compile(DEPENDENCIES_LINE)

CONFLICTS_LINE = r"^\s+(conflicts\s+)"
RE_CONFLICTS_LINE = re.compile(CONFLICTS_LINE)

TITLE_LINE = r"^\s+(title\s+)"
RE_TITLE_LINE = re.compile(TITLE_LINE)

SPLIT_LINE = r"[ \t]"
RE_SPLIT_LINE = re.compile(SPLIT_LINE)

INIT_SKIP_LINE = r"^\#|^\n|^\s+title|^\s+type|^\s+cfiles|^\s+afiles|^\s+defs"
RE_INIT_SKIP_LINE = re.compile(INIT_SKIP_LINE)

DEFAULT_LINE = r"^\s+default\s+(.*)"
RE_DEFAULT_LINE = re.compile(DEFAULT_LINE)

CONFIG_HEADER = "#\n" \
                + "#  Mcube Kernel\n" \
                + "#\n" \
                + "#  Hiroyuki Chishiro\n" \
                + "#\n" \
                + "\n" \
                + "#\n" \
                + "#  Mcube Kernel Configuration File\n" \
                + "#\n"

def scan_configure(filename):
  "scan configure"
  fin = open(filename, "r")
  for line in fin:
#    print("line = %s" % (line))
    if line.strip() == "":
      continue
    if RE_COMMENT_LINE.match(line):
      continue
    configure_list = RE_CONFIGURE_LINE.findall(line)
#    print(list)
    key = configure_list[0][0]
    value = configure_list[0][1]
    CONFIGURES[key] = value
#    print(key, value)
  fin.close()


def scan_algo_name(filename):
  "scan algorithm name"
  fin = open(filename, "r")
  this_algo = False
  algo_name = "rmwp"
  for line in fin:
    if line.strip() == "":
      continue
    if RE_COMMENT_LINE.match(line):
      continue
    index = line.find("menu")
    if index == 0:
      this_algo = False
    kconfig_list = RE_KCONFIG_LINE.findall(line)
    if kconfig_list:
      if CONFIGURES[kconfig_list[0]] == "y":
        this_algo = True
    elif this_algo:
      algo_list = RE_ALGO_LINE.findall(line)
      if algo_list:
        algo_name = algo_list[0]
        break
  fin.close()
  return algo_name


def scan_arch_name(filename):
  "scan architecture name"
  fin = open(filename, "r")
  this_arch = False
  arch_name = "axis"
  for line in fin:
    if line.strip() == "":
      continue
    if RE_COMMENT_LINE.match(line):
      continue
    index = line.find("menu")
    if index == 0:
      this_arch = False
    kconfig_list = RE_KCONFIG_LINE.findall(line)
    if kconfig_list:
      if CONFIGURES[kconfig_list[0]] == "y":
        this_arch = True
    elif this_arch:
      arch_list = RE_ARCH_LINE.findall(line)
      if arch_list:
        arch_name = arch_list[0]
        break
  fin.close()
#  print(arch_name)
  return arch_name

def scan_compiler_name(filename):
  "scan compiler name"
  fin = open(filename, "r")
  this_cc = False
  cc_name = "clang"
  for line in fin:
    if line.strip() == "":
      continue
    if RE_COMMENT_LINE.match(line):
      continue
    index = line.find("menu")
#    print(line)
    if index == 0:
      this_cc = False
    kconfig_list = RE_KCONFIG_LINE.findall(line)
    if kconfig_list:
      if CONFIGURES[kconfig_list[0]] == "y":
        this_cc = True
    elif this_cc:
      cc_list = RE_CC_LINE.findall(line)
      if cc_list:
        cc_name = cc_list[0]
        break
  fin.close()
#  print(cc_name)
  return cc_name

def scan_machine_name(filename):
  "scan machine name"
  fin = open(filename, "r")
  this_machine = False
  machine_name = "none"
  for line in fin:
    if line.strip() == "":
      continue
    if RE_COMMENT_LINE.match(line):
      continue
    index = line.find("menu")
    if index == 0:
      this_machine = False
    kconfig_list = RE_KCONFIG_LINE.findall(line)
    if kconfig_list:
      if CONFIGURES[kconfig_list[0]] == "y":
        this_machine = True
    elif this_machine:
      machine_list = RE_MACHINE_LINE.findall(line)
      if machine_list:
        machine_name = machine_list[0]
        break
  fin.close()
  return machine_name



def scan_kconfig(filename):
  "scan Kconfig"
  global DEFS, CFILES, AFILES, TYPE, VPATHS, CONFIGURE_DEFS
  current_config_val = "n"
#  print("scan_configs()")
  fin = open(filename, "r")
  for line in fin:
    if RE_SKIP_LINE.match(line):
      continue
    kconfig_list = RE_KCONFIG_LINE.findall(line)
#    print(line)
#    print(kconfig_list)
#    print(CONFIGURES)
    if kconfig_list:
      current_config_val = CONFIGURES[kconfig_list[0]]
    elif current_config_val == "y":
      defs_list = RE_DEFS_LINE.findall(line)
      if defs_list:
        DEFS += " \\\n" + defs_list[0]
#        print("DEFS = %s" % DEFS)
        for val in defs_list:
#          print("val = %s " % val)
          index = val.find("-D")
          if index != 0:
            sys.exit("Error: invalid defs " + val)
#          print(val[2:len(val)])
          CONFIGURE_DEFS[val[2:len(val)]] = "y"


      cfiles_list = RE_CFILES_LINE.findall(line)
      if cfiles_list:
        CFILES += " \\\n" + cfiles_list[0]
#        print("CFILES = %s" % cfiles_list)

      afiles_list = RE_AFILES_LINE.findall(line)
      if afiles_list:
        AFILES += " \\\n" + afiles_list[0]
#        print("AFILES = %s" % afiles_list)

      type_list = RE_TYPE_LINE.findall(line)
      if type_list:
        TYPE += " \\\n" + type_list[0]
        # print("type = %s" % type_list)
      vpaths_list = RE_VPATHS_LINE.findall(line)
      if vpaths_list:
        VPATHS += " \\\n" + vpaths_list[0]
#        print("VPATHS = %s" % vpaths_list)
  fin.close()


def check_exclusives(filename):
  "check exclusives"
  ret = True
  ex_menu = False
  ynum = 0
  current_menu = ""
  fin = open(filename, "r")
  for line in fin:
    if RE_COMMENT_LINE.match(line):
      continue
#    print(line)
    menu_list = RE_MENU_LINE.findall(line)
    if menu_list:
      ex_menu = True
#      print("list = %s" % list)
      current_menu = menu_list[0]
      ynum = 0

    type_list = RE_TYPE_LINE.findall(line)
    if type_list:
#      print("type_list = %s" % type_list)
      if type_list[0] != "exclusive":
        ex_menu = False

    endmenu_list = RE_ENDMENU_LINE.findall(line)
    if endmenu_list:
      ex_menu = False

    if ex_menu:
      kconfig_list = RE_KCONFIG_LINE.findall(line)
      if kconfig_list:
        if CONFIGURES[kconfig_list[0]] == "y":
          ynum += 1
        if ynum > 1:
          print("Exclusive Error: %s" % current_menu)
          ex_menu = False
          ret = False
  fin.close()
  return ret


def scan_dependencies(filename):
  "scan dependencies"
  global DEPENDENCIES, CONFLICTS, TITLES
  fin = open(filename, "r")
  current_config = ""
  for line in fin:
    if RE_COMMENT_LINE.match(line):
      continue

    kconfig_list = RE_KCONFIG_LINE.findall(line)
#    print(list)
    if kconfig_list:
      current_config = kconfig_list[0]
#      print("current_config = %s" % current_config)
      continue

#    print(line)
    dependencies_list = RE_DEPENDENCIES_LINE.findall(line)
    if dependencies_list:
      dep_str = "depends"
      index = line.find(dep_str)
      val = line[index+len(dep_str):len(line)]
      val = val.rstrip("\n").strip()
      val = RE_SPLIT_LINE.split(val)
      # print(val)
      # print(current_config)
      if current_config in DEPENDENCIES:
        DEPENDENCIES[current_config] += val
      else:
        DEPENDENCIES[current_config] = val

    conflicts_list = RE_CONFLICTS_LINE.findall(line)
    if conflicts_list:
      conf_str = "conflicts"
      index = line.find(conf_str)
      val = line[index+len(conf_str):len(line)]
      val = val.rstrip("\n").strip()
      val = RE_SPLIT_LINE.split(val)
#      print("val = ", val)
      if current_config in CONFLICTS:
        CONFLICTS[current_config] += val
      else:
        CONFLICTS[current_config] = val

    title_list = RE_TITLE_LINE.findall(line)
#    print(title_list)
    if title_list:
      title_str = "title"
#      print(line)
      index = line.find(title_str)
      val = line[index+len(title_str):len(line)]
      val = val.rstrip("\n").strip()
      val = RE_SPLIT_LINE.split(val)
#      print(val)
      TITLES[current_config] = val
  fin.close()

def check_conflicts(key):
  "check conflicts"
  no_error = True;
#  print(CONFLICTS)
  if key in CONFLICTS:
#    print("key = ", key)
    for con in CONFLICTS[key]:
#      print(con)
      if con in CONFIGURE_DEFS and CONFIGURE_DEFS[con] == "y":
        if no_error:
          no_error = False
        print("Conflict Error", key, "conflicts with", con)
  return no_error


def check_dependencies(key):
  "check dependencies"
  no_error = True;
#  print("key = %s" % key)
#  print(DEPENDENCIES)
  if key in DEPENDENCIES:
    for dep in DEPENDENCIES[key]:
      # print("dep = %s" % dep)
      # print("CONFIGURE_DEFS = %s" % CONFIGURE_DEFS)
      if not dep in CONFIGURE_DEFS:
        if no_error:
          no_error = False
        print("Dependency Error:", key, "depends on", dep)
  return no_error

def check_conflicts_and_dependencies():
  "check conflicts and dependencies"
  no_error = True
#  print("check_conflicts_and_dependencies()")
  for key, value in CONFIGURE_DEFS.items():
#    print(key, value)
    if value == "y":
      if not check_conflicts(key):
        no_error = False
      if not check_dependencies(key):
        no_error = False

  return no_error


def save_configure(filename):
  "save configure"
  fin = open(filename, "r")
  configure_file = open("configure", "w")
  configure_file.write(CONFIG_HEADER)
  current_configure = ""
  for line in fin:
    if RE_INIT_SKIP_LINE.match(line):
      continue
    menu_list = RE_MENU_LINE.findall(line)
    if menu_list:
      configure_file.write("\n# " + menu_list[0] + "\n")

    kconfig_list = RE_KCONFIG_LINE.findall(line)
    if kconfig_list:
      current_configure = kconfig_list[0]
      configure_file.write(current_configure)

    default_list = RE_DEFAULT_LINE.findall(line)
    if default_list:
      configure_file.write("=" + default_list[0] + "\n")

  configure_file.close()
  fin.close()

def scan_all_configures(filename):
  "scan all configures"
  cinfo = []
  fin = open(filename, "r")
  for line in fin:
    if RE_COMMENT_LINE.match(line):
      continue
    menu_list = RE_MENU_LINE.findall(line)
    if menu_list:
#      print(menu_list)
      cinfo.append(ConfigInfo())
      cinfo[len(cinfo)-1].set_menu(menu_list[0])

    type_list = RE_TYPE_LINE.findall(line)
#    print(line)
#    print(type_list)
    if type_list:
#      print(type_list)
      cinfo[len(cinfo)-1].set_type(type_list[0])

    kconfig_list = RE_KCONFIG_LINE.findall(line)
    if kconfig_list:
#      print(kconfig_list)
      cinfo[len(cinfo)-1].get_configure().append(kconfig_list[0])

  fin.close()

  num = 1
  for cif in cinfo:
    if cif.get_type() == "exclusive":
      cif.set_num(len(cif.get_configure()))
    elif cif.get_type() == "none":
      cif.set_num(pow(2, len(cif.get_configure())))
    else:
      sys.exit("Error: unknown type " + cif.get_type() + "\n")
    print(cif.get_num())
    num *= cif.get_num()

  return cinfo, num

def write_menu(filename, cinfo, index):
  "write menu"
#  print("index = ", index)
  filename.write("\n# " + cinfo.get_menu() + "\n")
  if cinfo.get_type() == "exclusive":
    for i in range(0, len(cinfo.get_configure())):
      if i == index:
        filename.write(cinfo.get_configure()[i] + "=y\n")
      else:
        filename.write(cinfo.get_configure()[i] + "=n\n")
  elif cinfo.get_type() == "none":
#    print("index = ", index)
    for i in range(0, len(cinfo.get_configure())):
      if bin((0b1 << i) & index) == bin(0b1 << i):
        filename.write(cinfo.get_configure()[i] + "=y\n")
      else:
        filename.write(cinfo.get_configure()[i] + "=n\n")
  else:
    sys.exit("Unknown type " + cinfo.get_type() + "\n")

def write_configure(filename, cinfo, vals):
  "write configure"
  for i, val in enumerate(vals):
    write_menu(filename, cinfo[i], val)

def sendmail(to_address, from_address, subject, message):
  "send e-mail"
  msg = MIMEText(message)
  msg["Subject"] = subject
  msg["From"] = from_address
  msg["To"] = to_address

  smtp = smtplib.SMTP()
  smtp.connect()
  smtp.sendmail(from_address, to_address, msg.as_string())
  smtp.close()

def check_argc(argc, argv, correct_argc):
  "check argc"
  if argc != correct_argc:
    print("Usage: %s filename eb[el]" % argv[0])
    quit()
