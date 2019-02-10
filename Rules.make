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
#CFLAGS += -O3
#CFLAGS += -O2
#CFLAGS += -O0

CFLAGS += -std=gnu11
CFLAGS += -fno-strict-aliasing
CFLAGS += -fPIC

#
# GCC C dialect flags:
#
# We're a freestanding environment by ISO C99 definition:
# - Code get executed without benefit of an OS (it's a kernel).
# - Program startup and termination is implementation-defined.
# - Any library facilities outside C99 'strict conformance'
#   options are also implementation defined.
#
# Poking GCC with the 'freestanding' flag assures it won't
# presume availability of any 'hosted' facilities like libc.
#
# After using -nostdinc, we add compiler's specific includes
# back (stdarg.h, etc) using the -iwithprefix flag.
#
CDIALECT_FLAGS =			\
  -ffreestanding			\
  -fno-stack-protector			\
  -fno-builtin				\
  -iwithprefix include

ifneq ($(ARCH_NAME), sim)
CDIALECT_FLAGS += \
	-fno-pie		\
	-nostdlib	\

#CDIALECT_FLAGS += \
	-nostdinc	

endif


#  -std=gnu99				\

#-I ../../include/

#
# C Optimization flags:
#
# Use -O3 to catch any weird bugs early on
#
# Note-1! Fallback to -O2 at official releases
# Note-2! Shouldn't we disable strict aliasing?
#
COPT_FLAGS =				\
  -O3					\
  -pipe

#
# Warnings request and dismissal flags:
#
# - We've previously caught 2 bugs causeed by an implicit cast
# to a smaller-width type: carefully inspect warnings reported
# by the '-Wconversion' flag.
#
# - We may like to warn about aggregate returns cause we don't
# want to explode the stack if the structure type returned got
# _innocently_ bigger over time. Check '-Waggregate-return'.
#
# Options are printed in GCC HTML documentation order.
#
CWARN_FLAGS =				\
  -Wall					\

# -Wextra and other warning
#CWARN_FLAGS += -Wextra				\
		-Wchar-subscripts			\
  -Wformat=2				\
  -Wmissing-include-dirs		\
  -Wparentheses				\
  -Wtrigraphs				\
  -Wunused				\
  -Wstrict-aliasing=2			\
  -Wpointer-arith			\
  -Wcast-qual				\
  -Wwrite-strings			\
  -Waddress				\
  -Wlogical-op				\
  -Wstrict-prototypes			\
  -Wmissing-prototypes			\
  -Wmissing-declarations		\
  -Wmissing-noreturn			\
  -Wnormalized=nfc			\
  -Wredundant-decls			\
  -Wvla					\
  -Wdisabled-optimization		\
  -Wno-type-limits			\
  -Wno-missing-field-initializers \

#  -Wundef				\


CFLAGS +=				\
  $(CMACH_FLAGS)			\
  $(CDIALECT_FLAGS)			\
  $(COPT_FLAGS)				\
  $(CWARN_FLAGS)


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
