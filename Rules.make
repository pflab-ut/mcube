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
 $(TOP_DIR)/kernel \
 $(TOP_DIR)/kernel/queue \
 $(TOP_DIR)/user \
 $(TOP_DIR)/user/$(ARCH_NAME)/ \

vpath %.S $(TOP_DIR)/arch/$(ARCH_NAME)


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
CFLAGS += -O2
CFLAGS += -nostdlib -fno-stack-protector -ffreestanding
CFLAGS += -fno-builtin 

SIZE = $(CROSS_PREFIX)size

DOXYGEN = doxygen
PYTHON = python3
DUMPARG = eb
RM = rm -f
FIND = find
CLEANFILES = *.o *~ *.o.* *.log *.pyc *.tmp __pycache__


DUMP = $(TOP_DIR)/scripts/misc/dump.py
RUN_AXIS = $(TOP_DIR)/scripts/misc/run.sh
MAKEFLAGS += --no-print-directory -rR
MAKE = make

ROMFILE = rom.txt

DOC_DIR = doc


BUILD_DIR = $(TOP_DIR)/build

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

ifeq ($(ARCH_NAME), x86)
ASM_OBJS = $(ASMS:%.asm=$(BUILD_DIR)/%.o)
ASM_DEPS = $(ASMS:%.asm=$(BUILD_DIR)/%.d)
else
ASM_OBJS = $(ASMS:%.S=$(BUILD_DIR)/%.o)
ASM_DEPS = $(ASMS:%.S=$(BUILD_DIR)/%.d)
endif

DEPS = $(SRCS:%.c=$(BUILD_DIR)/%.d)
