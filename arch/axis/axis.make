#
#  Mcube Kernel
# 
#  Hiroyuki Chishiro
#

# text size: 32KB [0x00000000,0x00008000)
TEXT_ADDR = 0x00000000
# data size: 16KB [0x00008000,0x0000c000)
DATA_ADDR = 0x00008000
# bss size: 64KB [0x0000c000,0x0001c000)
BSS_ADDR  = 0x0000c000

LDFLAGS += -Ttext=$(TEXT_ADDR) -Tdata=$(DATA_ADDR) -Tbss=$(BSS_ADDR)

ifeq ($(CC), clang)
  CFLAGS += -target maxis
  # use release version
  CROSS_PREFIX = llvm-
  LD = ld.lld
  AS = $(CROSS_PREFIX)as
#  OBJDUMP = $(CROSS_PREFIX)objdump -disassemble -print-imm-hex
  OBJDUMP = $(CROSS_PREFIX)objdump -disassemble-all -print-imm-hex
  OBJCOPY = $(CROSS_PREFIX)objcopy
  LDFLAGS += -T scripts/linker/axis-elf.ld --Map $(MAP)
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


CFLAGS += -fno-strict-aliasing


CFLAGS += -D__BIG_ENDIAN__

ASMS = \
	$(TOP_DIR)/arch/axis/boot.S \
	$(TOP_DIR)/arch/axis/vector.S \
	$(TOP_DIR)/arch/axis/utils.S \

SRCS += \
 $(TOP_DIR)/arch/axis/interrupt.c \
 $(TOP_DIR)/arch/axis/thread.c \
 $(TOP_DIR)/arch/axis/timer.c \
 $(TOP_DIR)/arch/axis/stdio.c \
 $(TOP_DIR)/arch/axis/init.c \
 $(TOP_DIR)/arch/axis/cpu.c \

SRCS += $(TOP_DIR)/user/axis/test_integer_instructions.c \
 $(TOP_DIR)/user/axis/test_transfer_instructions.c \
 $(TOP_DIR)/user/axis/test_control_instructions.c \
 $(TOP_DIR)/user/axis/test_special_instructions.c \
 $(TOP_DIR)/user/axis/user_axis.c \
