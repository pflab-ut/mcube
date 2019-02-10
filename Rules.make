#
#  Mcube Kernel
# 
#  Hiroyuki Chishiro
#


include $(TOP_DIR)/.sysconfig
include $(TOP_DIR)/.kconfig
include $(TOP_DIR)/arch/$(ARCH_NAME)/$(ARCH_NAME).make

vpath %.h $(TOP_DIR)/include
vpath %.c $(TOP_DIR)/arch/$(ARCH_NAME) \
 $(TOP_DIR)/arch/arm/raspi3 \
 $(TOP_DIR)/arch/arm/synquacer \
 $(TOP_DIR)/kernel \
 $(TOP_DIR)/kernel/queue \
 $(TOP_DIR)/lib \
 $(TOP_DIR)/fs \
 $(TOP_DIR)/fs/ext2 \
 $(TOP_DIR)/fs/fat \
 $(TOP_DIR)/user \
 $(TOP_DIR)/user/$(ARCH_NAME)/ \
 $(TOP_DIR)/user/test/ \

vpath %.S $(TOP_DIR)/arch/$(ARCH_NAME)
vpath %.asm $(TOP_DIR)/arch/$(ARCH_NAME)

ifeq ($(ARCH_NAME), x86)
IMG_TARGET = $(TOP_DIR)/build/mcube.img
endif

TARGET = $(TOP_DIR)/build/mcube

DMPFILE = $(TARGET).dmp
MAP = $(TARGET).map
BIN = $(TARGET).bin

CCACHE = $(shell which ccache)


ifeq ($(CC), $(CCACHE) clang)
  CLANG = $(shell which clang)
  CC = $(CCACHE) $(CLANG)
endif

CFLAGS += -Iinclude -Wall
#CFLAGS += -Wextra
#CFLAGS += -O3
#CFLAGS += -O2
#CFLAGS += -O0

CFLAGS += -std=gnu11
CFLAGS += -nostdlib -fno-stack-protector -ffreestanding
CFLAGS += -fno-builtin 
CFLAGS += -fno-strict-aliasing


SIZE = $(CROSS_PREFIX)size

DOXYGEN = doxygen
PYTHON = python3
DUMPARG = eb
RM = rm -f
FIND = find
CAT = cat
CLEANFILES = *.o *~ *.o.* *.log *.pyc *.tmp __pycache__


DUMP = $(TOP_DIR)/scripts/misc/dump.py
RUN_AXIS = $(TOP_DIR)/scripts/misc/run_axis.sh
RUN_AXIS_CLUSTER = $(TOP_DIR)/scripts/misc/run_axis_cluster.sh
MAKEFLAGS += --no-print-directory -rR
MAKE = make

ROMFILE = rom.txt

DOC_DIR = docs


BUILD_DIR = $(TOP_DIR)/build

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

ASM_OBJS = $(ASMS:%.S=$(BUILD_DIR)/%.o)
ASM_DEPS = $(ASMS:%.S=$(BUILD_DIR)/%.d)

DEPS = $(SRCS:%.c=$(BUILD_DIR)/%.d)

BOOTSECT_OBJS = $(BOOTSECT_ASMS:%.S=$(BUILD_DIR)/%.o)
