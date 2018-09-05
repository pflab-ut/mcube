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
import tools
import errno


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
cinfo, NUM = tools.scan_all_configs("Kconfig")
print("NUM = ", NUM)

# TODO: change recursive call
print(len(cinfo))

index = 0
message = ""

def do_testconfig(cinfo, val):
  global index
  global message
  print(index, "/", NUM)
  configure_file = open("configure", "w")
  configure_file.write(tools.CONFIG_HEADER)
  tools.write_config(configure_file, cinfo, val)
  configure_file.close()
  index += 1
  cmd = "make buildclean"
  p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  stdout, stderr = p.communicate()
  sys.stdout.write(stderr.decode())
          
  cmd = "./scripts/kconfig/configure.py"
  p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
  stdout, stderr = p.communicate()
  sys.stdout.write(stderr.decode())
              
  if stderr == b"":
    print("make configure success")
    cmd = "make -j" + str(multiprocessing.cpu_count())
    p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = p.communicate()
    if stderr == b"":
      print("make success")
    else:
      print("make failure")
      # save configure IDs if 'make failure' occurs.
      if message == "":
        message = "Configure IDs of 'make failure' are as follows.\n\n"
      message += str(index) + "\n"
      fw = open(PATH + str(index) + "_make.log", "w")
      fw.write(stdout.decode() + "\n" + stderr.decode())
      fw.close()
      shutil.copyfile("configure", PATH + str(index) + "_configure")
  else:
    print("make configure failure")
    # fw = open(PATH + str(index) + "_make_configure.log", "w")
    # fw.write(stdout.decode() + "\n" + stderr.decode())
    # fw.close()

val = [0, 0, 0, 0, 0, 0, 0, 0]
    
# nested for loop    
"""
#for a in range(1, cinfo[0].num-1):
#for a in range(3, cinfo[0].num):
for val[0] in range(0, cinfo[0].num):
  for val[1] in range(0, cinfo[1].num):
    for val[2] in range(0, cinfo[2].num):
      for val[3] in range(0, cinfo[3].num):
        for val[4] in range(0, cinfo[4].num):
          for val[5] in range(0, cinfo[5].num):
            for val[6] in range(0, cinfo[6].num):
              for val[7]  in range(0, cinfo[7].num):
                do_testconfig(cinfo, val)


"""
# recursive for loop
def recursive_for_loop(index, depth):
  global val
  for val[index] in range(0, cinfo[index].num):
    if depth > 0:
      recursive_for_loop(index + 1, depth - 1)
    else:
#      print(val)
      do_testconfig(cinfo, val)
      
recursive_for_loop(0, 7)


if message == "":
  message = "Build Test is successful."

END = datetime.datetime.now()

ELAPSED = END - BEGIN
message = "Begin Time: " + str(BEGIN) + "\n" \
          + "End Time: " + str(END) + "\n" \
          + "Elapsed Time: " + str(ELAPSED) + "\n\n" \
          + message
print(message)

f = open(CWD + "/message.log", "w")
f.write(message)
f.close()


if os.environ.get("EMAIL_FROM_ADDRESS"):
  EMAIL_FROM_ADDRESS = os.environ["EMAIL_FROM_ADDRESS"]
else:
  sys.exit("Unknown Environment Variable: EMAIL_FROM_ADDRESS")

if os.environ.get("EMAIL_FROM_ADDRESS"):
  EMAIL_TO_ADDRESS = os.environ["EMAIL_TO_ADDRESS"]
else:
  sys.exit("Unknown Environment Variable: EMAIL_TO_ADDRESS")


subject = "Configure Test"

tools.sendmail(EMAIL_TO_ADDRESS, EMAIL_FROM_ADDRESS, subject, message)

