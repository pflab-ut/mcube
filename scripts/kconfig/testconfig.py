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


BEGIN = datetime.datetime.now()


# mkdir testconfig directory
CWD = os.getcwd()
PATH = CWD + "/testconfig/"

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

INDEX = 0
MESSAGE = ""

def do_testconfig(cinfo, val):
  "do testconfig"
  global INDEX
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

VALS = [0, 0, 0, 0, 0, 0, 0, 0]

# nested for loop
"""
#for a in range(1, CINFO[0].num-1):
#for a in range(3, CINFO[0].num):
for VALS[0] in range(0, CINFO[0].num):
  for VALS[1] in range(0, CINFO[1].num):
    for VALS[2] in range(0, CINFO[2].num):
      for VALS[3] in range(0, CINFO[3].num):
        for VALS[4] in range(0, CINFO[4].num):
          for VALS[5] in range(0, CINFO[5].num):
            for VALS[6] in range(0, CINFO[6].num):
              for VALS[7]  in range(0, CINFO[7].num):
                do_testconfig(CINFO, VALS)


"""
def recursive_for_loop(index, depth):
  "Recursive for loop"
  global VALS
  for VALS[index] in range(0, CINFO[index].get_num()):
    if depth > 0:
      recursive_for_loop(index + 1, depth - 1)
    else:
#      print(VALS)
      do_testconfig(CINFO, VALS)

recursive_for_loop(0, 7)


if MESSAGE == "":
  MESSAGE = "Build Test is successful."

END = datetime.datetime.now()

ELAPSED = END - BEGIN
MESSAGE = "Begin Time: " + str(BEGIN) + "\n" \
          + "End Time: " + str(END) + "\n" \
          + "Elapsed Time: " + str(ELAPSED) + "\n\n" \
          + MESSAGE
print(MESSAGE)

FIN = open(CWD + "/MESSAGE.log", "w")
FIN.write(MESSAGE)
FIN.close()


if os.environ.get("EMAIL_FROM_ADDRESS"):
  EMAIL_FROM_ADDRESS = os.environ["EMAIL_FROM_ADDRESS"]
else:
  sys.exit("Unknown Environment Variable: EMAIL_FROM_ADDRESS")

if os.environ.get("EMAIL_FROM_ADDRESS"):
  EMAIL_TO_ADDRESS = os.environ["EMAIL_TO_ADDRESS"]
else:
  sys.exit("Unknown Environment Variable: EMAIL_TO_ADDRESS")


SUBJECT = "Build Test"

tools.sendmail(EMAIL_TO_ADDRESS, EMAIL_FROM_ADDRESS, SUBJECT, MESSAGE)
