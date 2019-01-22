#
#  Mcube Kernel
# 
#  Hiroyuki Chishiro
#

LDFLAGS	+= -lm -lpthread

CFLAGS += -g -ggdb

TARGET = mcube

LD = $(CC)
AS = $(CROSS_PREFIX)as

ifeq ($(CC), clang)
  CROSS_PREFIX = llvm-
  OBJDUMP = $(CROSS_PREFIX)objdump -disassemble -print-imm-hex
#  OBJDUMP = $(CROSS_PREFIX)objdump -disassemble-all -print-imm-hex
  OBJCOPY = $(CROSS_PREFIX)objcopy
else
  CROSS_PREFIX = 
  CC = $(CCACHE) $(CROSS_PREFIX)gcc
  OBJDUMP = $(CROSS_PREFIX)objdump -D
  OBJCOPY = $(CROSS_PREFIX)objcopy
endif


CFLAGS += -D__LITTLE_ENDIAN__

SRCS += \
 $(TOP_DIR)/arch/sim/cpu.c \
 $(TOP_DIR)/arch/sim/init.c \
 $(TOP_DIR)/arch/sim/irq.c \
 $(TOP_DIR)/arch/sim/timer.c \
 $(TOP_DIR)/arch/sim/utils.c \
 $(TOP_DIR)/arch/sim/thread.c \
 $(TOP_DIR)/arch/sim/syscall.c \

SRCS += $(TOP_DIR)/user/sim/user_sim.c \
