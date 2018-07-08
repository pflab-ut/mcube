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


configures = {}
configure_defs = {}
titles = {}
defs = "CFLAGS += "
cfiles = "SRCS += "
afiles = "ASRCS += "
vpaths = "vpath %.cpp"
dependencies = {}
conflicts = {}
__type = "none"

CONFIGURE_LINE = r"^(.*)=(.)"
RE_CONFIGURE_LINE = re.compile(CONFIGURE_LINE)

comment_line = r"^\#"
RE_COMMENT_LINE = re.compile(comment_line)

ALGO_LINE = r"\s+alname\s+(.*)"
RE_ALGO_LINE = re.compile(ALGO_LINE)

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
  f = open(filename, "r")
  for line in f:
#    print("line = %s" % (line))
    if line.strip() == "":
      continue
    if RE_COMMENT_LINE.match(line):
      continue
    configure_list = RE_CONFIGURE_LINE.findall(line)
#    print(list)
    key = configure_list[0][0]
    value = configure_list[0][1]
    configures[key] = value
#    print(key, value)
  f.close()


def scan_algo_name(filename):
  f = open(filename, "r")
  this_algo = False
  algo_name = "rmwp"
  for line in f:
    if line.strip() == "":
      continue
    if RE_COMMENT_LINE.match(line):
      continue
    index = line.find("menu")
    if index == 0:
      this_algo = False
    kconfig_list = RE_KCONFIG_LINE.findall(line)
    if kconfig_list:
      if configures[kconfig_list[0]] == "y":
        this_algo = True
    elif this_algo:
      algo_list = RE_ALGO_LINE.findall(line)
      if algo_list:
        algo_name = algo_list[0]
        break
  f.close()
  return algo_name


def scan_arch_name(filename):
  f = open(filename, "r")
  this_arch = False
  arch_name = "axis"
  for line in f:
    if line.strip() == "":
      continue
    if RE_COMMENT_LINE.match(line):
      continue
    index = line.find("menu")
    if index == 0:
      this_arch = False
    kconfig_list = RE_KCONFIG_LINE.findall(line)
    if kconfig_list:
      if configures[kconfig_list[0]] == "y":
        this_arch = True
    elif this_arch:
      arch_list = RE_ARCH_LINE.findall(line)
      if arch_list:
        arch_name = arch_list[0]
        break
  f.close()
#  print(arch_name)
  return arch_name

def scan_compiler_name(filename):
  f = open(filename, "r")
  this_cc = False
  cc_name = "clang"
  for line in f:
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
      if configures[kconfig_list[0]] == "y":
        this_cc = True
    elif this_cc:
      cc_list = RE_CC_LINE.findall(line)
      if cc_list:
        cc_name = cc_list[0]
        break
  f.close()
#  print(cc_name)
  return cc_name



def scan_configs(filename):
  global defs, cfiles, afiles, __type, vpaths, configure_defs
  current_config_val = "n"
#  print("scan_configs()")
  f = open(filename, "r")
  for line in f:
    if RE_SKIP_LINE.match(line):
      continue
    kconfig_list = RE_KCONFIG_LINE.findall(line)
#    print(line)
#    print(kconfig_list)
#    print(configures)
    if kconfig_list:
      current_config_val = configures[kconfig_list[0]]
    elif current_config_val == "y":
      defs_list = RE_DEFS_LINE.findall(line)
      if defs_list:
        defs += " \\\n" + defs_list[0]
#        print("defs = %s" % defs)
        for val in defs_list:
#          print("val = %s " % val)
          index = val.find("-D")
          if index != 0:
            sys.exit("Error: invalid defs " + val)
#          print(val[2:len(val)])
          configure_defs[val[2:len(val)]] = "y"
                         
          
      cfiles_list = RE_CFILES_LINE.findall(line)
      if cfiles_list:
        cfiles += " \\\n" + cfiles_list[0]
#        print("cfiles = %s" % cfiles_list)
        
      afiles_list = RE_AFILES_LINE.findall(line)
      if afiles_list:
        afiles += " \\\n" + afiles_list[0]
#        print("afiles = %s" % afiles_list)
      
      type_list = RE_TYPE_LINE.findall(line)
      if type_list:
        __type += " \\\n" + type_list[0]
        # print("type = %s" % type_list)
      vpaths_list = RE_VPATHS_LINE.findall(line)
      if vpaths_list:
        vpaths += " \\\n" + vpaths_list[0]
#        print("vpaths = %s" % vpaths_list)
  f.close()


def check_exclusives(filename):
  ret = True
  ex_menu = False
  ynum = 0
  current_menu = ""
  f = open(filename, "r")
  for line in f:
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
        if configures[kconfig_list[0]] == "y":
          ynum += 1
        if ynum > 1:
          print("Exclusive Error: %s" % current_menu)
          ex_menu = False
          ret = False
  f.close()
  return ret


def scan_dependencies(filename):
  global dependencies, conflicts, titles
  f = open(filename, "r")
  current_config = ""
  for line in f:
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
      if current_config in dependencies:
        dependencies[current_config] += val
      else:
        dependencies[current_config] = val
    
    conflicts_list = RE_CONFLICTS_LINE.findall(line)
    if conflicts_list:
      conf_str = "conflicts"
      index = line.find(conf_str)
      val = line[index+len(conf_str):len(line)]
      val = val.rstrip("\n").strip()
      val = RE_SPLIT_LINE.split(val)
#      print("val = ", val)
      if current_config in conflicts:
        conflicts[current_config] += val
      else:
        conflicts[current_config] = val
      
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
      titles[current_config] = val

def check_conflicts_and_dependencies():
  no_error = True
#  print("check_conflicts_and_dependencies()")
  for k, v in configure_defs.items():
#    print(k, v)
    if v == "y":
#      print("k = %s" % k)
#      print(dependencies)
      if k in dependencies:
        for d in dependencies[k]:
#          print("d = %s" % d)
#          print("configure_defs = %s" % configure_defs)
          if not d in configure_defs:
            if no_error:
              no_error = False
            print("Dependency Error:", k, "depends on", d)
#      print(conflicts)
      if k in conflicts:
#        print("k = ", k)
        for c in conflicts[k]:
#          print(c)
          if c in configure_defs and configure_defs[c] == "y":
            if no_error:
              no_error = False
            print("Conflict Error", k, "conflicts with", c)
            
  return no_error


def save_config(filename):
  configure_file = open("configure", "w")
  configure_file.write(CONFIG_HEADER)
  current_configure = ""
  f = open(filename, "r")
  for line in f:
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
        
  f.close()
  configure_file.close()

class ConfigInfo:
  """
  ConfigInfo class has config information.
  """
  def __init__(self):
    self.menu = ""
    self.__type = ""
    self.config = []
    self.num = 0
    
def scan_all_configs(filename):
  cinfo = []
  f = open(filename, "r")
  for line in f:
    if RE_COMMENT_LINE.match(line):
      continue
    menu_list = RE_MENU_LINE.findall(line)
    if menu_list:
      print(menu_list)
      cinfo.append(ConfigInfo())
      cinfo[len(cinfo)-1].menu = menu_list[0]

    type_list = RE_TYPE_LINE.findall(line)
#    print(line)
#    print(type_list)
    if type_list:
      print(type_list)
      cinfo[len(cinfo)-1].__type = type_list[0]
      
    kconfig_list = RE_KCONFIG_LINE.findall(line)
    if kconfig_list:
      print(kconfig_list)
      cinfo[len(cinfo)-1].config.append(kconfig_list[0])

  f.close()

  num = 1
  for ci in cinfo:
    if ci.__type == "exclusive":
      ci.num = len(ci.config)
    elif ci.__type  == "none":
      ci.num = pow(2, len(ci.config))
    else:
      sys.exit("Error: unknown type " + ci.__type + "\n")
    print(ci.num)
    num *= ci.num

  return cinfo, num

def write_menu(filename, ci, index):
#  print("index = ", index)
  filename.write("\n# " + ci.menu + "\n")
  if ci.__type == "exclusive":
    for i in range(0, len(ci.config)):
      if i == index:
        filename.write(ci.config[i] + "=y\n")
      else:
        filename.write(ci.config[i] + "=n\n")          
  elif ci.__type == "none":
#    print("index = ", index)
    for i in range(0, len(ci.config)):
      if bin((0b1 << i) & index) == bin(0b1 << i):
        filename.write(ci.config[i] + "=y\n")
      else:
        filename.write(ci.config[i] + "=n\n")    
  else:
    sys.exit("Unknown type " + ci.__type + "\n")
    
def write_config(filename, cinfo, val):
  for i in range(0, len(val)):
    write_menu(filename, cinfo[i], val[i])
  
def sendmail(to_address, from_address, subject, message):
  msg = MIMEText(message)
  msg["Subject"] = "Build Test"
  msg["From"] = from_address
  msg["To"] = to_address

  s = smtplib.SMTP()
  s.connect()
  s.sendmail(from_address, to_address, msg.as_string())
  s.close()
