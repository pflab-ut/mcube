#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @file scripts/kconfig/testconfig.py
#
# @author Hiroyuki Chishiro
#
"""
test configuration.
"""

import os
import shutil
import subprocess
import sys
import multiprocessing
import datetime
import errno
import tools

VALS = [0, 0, 0, 0, 0, 0, 0, 0]
INDEX = 0
MESSAGE = ""
PATH = ""
CINFO = []
NUM = 0

def do_testconfig(cinfo, val):
  "do testconfig"
  global INDEX
  global NUM
  global MESSAGE
  print(INDEX, "/", NUM)
  configure_file = open("configure", "w")
  configure_file.write(tools.CONFIG_HEADER)
  tools.write_configure(configure_file, cinfo, val)
  configure_file.close()
  INDEX += 1
  cmd = "make buildclean"
  proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  stdout, stderr = proc.communicate()
  sys.stdout.write(stderr.decode())

  cmd = "./scripts/kconfig/configure.py"
  proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  stdout, stderr = proc.communicate()
  sys.stdout.write(stderr.decode())

  if stderr == b"":
    print("make configure success")
    cmd = "make -j" + str(multiprocessing.cpu_count())
    proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = proc.communicate()
    if stderr == b"":
      print("make success")
    else:
      print("make failure")
      # save configure IDs if 'make failure' occurs.
      if MESSAGE == "":
        MESSAGE = "Configure IDs of 'make failure' are as follows.\n\n"
      MESSAGE += str(INDEX) + "\n"
      fout = open(PATH + str(INDEX) + "_make.log", "w")
      fout.write(stdout.decode() + "\n" + stderr.decode())
      fout.close()
      shutil.copyfile("configure", PATH + str(INDEX) + "_configure")
  else:
    print("make configure failure")
    # fout = open(PATH + str(INDEX) + "_make_configure.log", "w")
    # fout.write(stdout.decode() + "\n" + stderr.decode())
    # fout.close()


def do_recursive_for_loop(index, depth):
  "do recursive for loop"
  global CINFO
  global VALS
  for VALS[index] in range(0, CINFO[index].get_num()):
    if depth > 0:
      do_recursive_for_loop(index + 1, depth - 1)
    else:
#      print(VALS)
      do_testconfig(CINFO, VALS)

def main():
  "main function"
  global PATH
  global NUM
  global CINFO
  global MESSAGE

  begin = datetime.datetime.now()

  # mkdir testconfig directory
  cwd = os.getcwd()
  PATH = cwd + "/testconfig/"

  if os.path.exists(os.path.dirname(PATH)):
    try:
      shutil.rmtree(PATH)
    except OSError as exc:
      if exc.errno != errno.EEXIST:
        raise

  if not os.path.exists(os.path.dirname(PATH)):
    try:
      os.makedirs(os.path.dirname(PATH))
    except OSError as exc:
      if exc.errno != errno.EEXIST:
        raise

  # count number of configs
  CINFO, NUM = tools.scan_all_configures("Kconfig")
  print("NUM = ", NUM)

  print(len(CINFO))

  do_recursive_for_loop(0, 7)

  if MESSAGE == "":
    MESSAGE = "Build Test is successful."

  end = datetime.datetime.now()

  elapsed = end - begin
  MESSAGE = "Begin Time: " + str(begin) + "\n" \
            + "End Time: " + str(end) + "\n" \
            + "Elapsed Time: " + str(elapsed) + "\n\n" \
            + MESSAGE
  print(MESSAGE)

  fin = open(cwd + "/message.log", "w")
  fin.write(MESSAGE)
  fin.close()


  if os.environ.get("EMAIL_FROM_ADDRESS"):
    email_from_address = os.environ["EMAIL_FROM_ADDRESS"]
  else:
    sys.exit("Unknown Environment Variable: EMAIL_FROM_ADDRESS")

  if os.environ.get("EMAIL_FROM_ADDRESS"):
    email_to_address = os.environ["EMAIL_TO_ADDRESS"]
  else:
    sys.exit("Unknown Environment Variable: EMAIL_TO_ADDRESS")

  subject = "Build Test"

  tools.sendmail(email_to_address, email_from_address, subject, MESSAGE)

if __name__ == "__main__":
  main()
