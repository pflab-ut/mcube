#
#  Mcube Kernel
# 
#  Hiroyuki Chishiro
#


ifeq ($(CC), clang)
  CFLAGS += -target maxis
  CROSS_PREFIX = llvm-
		CC = clang
  LD = ld.lld
  AS = $(CROSS_PREFIX)as
  OBJDUMP = $(CROSS_PREFIX)objdump -disassemble -print-imm-hex
#  OBJDUMP = $(CROSS_PREFIX)objdump -disassemble-all -print-imm-hex
  OBJCOPY = $(CROSS_PREFIX)objcopy
  LDFLAGS += -nostdlib -T scripts/linker/axis-elf.ld --Map $(MAP)
else
#  CFLAGS += -falign-functions=8
  CROSS_PREFIX = dom-elf-
  CC = $(CCACHE) $(CROSS_PREFIX)gcc
  LD = $(CROSS_PREFIX)ld
  AS = $(CROSS_PREFIX)as
  OBJDUMP = $(CROSS_PREFIX)objdump -D
  OBJCOPY = $(CROSS_PREFIX)objcopy
  CFLAGS += -G0
  LDFLAGS += -T scripts/linker/dom-elf.ld
endif


CFLAGS += -D__BIG_ENDIAN__

ASMS = \
	$(TOP_DIR)/arch/axis/boot.S \
	$(TOP_DIR)/arch/axis/vector.S \
	$(TOP_DIR)/arch/axis/utils.S \

SRCS += \
 $(TOP_DIR)/arch/axis/irq.c \
 $(TOP_DIR)/arch/axis/dump.c \
 $(TOP_DIR)/arch/axis/thread.c \
 $(TOP_DIR)/arch/axis/timer.c \
 $(TOP_DIR)/arch/axis/stdio.c \
 $(TOP_DIR)/arch/axis/dmac.c \
 $(TOP_DIR)/arch/axis/init.c \
 $(TOP_DIR)/arch/axis/cpu.c \
	$(TOP_DIR)/arch/axis/syscall.c \

SRCS += \
 $(TOP_DIR)/user/axis/cluster.c \
	$(TOP_DIR)/user/axis/test_integer_instructions.c \
 $(TOP_DIR)/user/axis/test_transfer_instructions.c \
 $(TOP_DIR)/user/axis/test_control_instructions.c \
 $(TOP_DIR)/user/axis/test_special_instructions.c \
 $(TOP_DIR)/user/axis/user_axis.c \
