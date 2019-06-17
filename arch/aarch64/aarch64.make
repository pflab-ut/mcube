#
#  Mcube Kernel
# 
#  Hiroyuki Chishiro
#

CFLAGS += -mcpu=cortex-a53 -fpic -ffreestanding
CFLAGS += -fPIC

ifeq ($(CC), clang)
  CFLAGS += -target aarch64
  CROSS_PREFIX = llvm-
  CC	= $(CCACHE) clang
  LD = ld.lld
  AS = $(CROSS_PREFIX)as
#  OBJDUMP = $(CROSS_PREFIX)objdump -disassemble -print-imm-hex
#  OBJDUMP = $(CROSS_PREFIX)objdump -disassemble-all -print-imm-hex
  OBJDUMP = aarch64-linux-gnu-objdump -D
  OBJCOPY = $(CROSS_PREFIX)objcopy
  LDFLAGS += -T scripts/linker/aarch64-elf.ld --Map $(MAP)
else
  CROSS_PREFIX = aarch64-linux-gnu-
  CC = $(CCACHE) $(CROSS_PREFIX)gcc
  LD = $(CROSS_PREFIX)ld
  AS = $(CROSS_PREFIX)as
  OBJDUMP = $(CROSS_PREFIX)objdump -D
  OBJCOPY = $(CROSS_PREFIX)objcopy
  LDFLAGS += -T scripts/linker/aarch64-elf.ld
endif


ASMS	+=	$(TOP_DIR)/arch/aarch64/boot.S \
	$(TOP_DIR)/arch/aarch64/utils.S \
	$(TOP_DIR)/arch/aarch64/vector.S \
	$(TOP_DIR)/arch/aarch64/mmu_and_cache.S \
	$(TOP_DIR)/arch/aarch64/syscall.S \

SRCS	+=	$(TOP_DIR)/arch/aarch64/init.c \
	$(TOP_DIR)/arch/aarch64/cpu.c \
	$(TOP_DIR)/arch/aarch64/thread.c \
	$(TOP_DIR)/arch/aarch64/timer.c \
	$(TOP_DIR)/arch/aarch64/irq.c \
	$(TOP_DIR)/arch/aarch64/stdio.c \
	$(TOP_DIR)/arch/aarch64/dump.c \
 $(TOP_DIR)/drivers/uart/uart.c \
	$(TOP_DIR)/drivers/uart/uart_aarch64.c \

ifeq ($(MACHINE_NAME), raspi3)
SRCS +=	$(TOP_DIR)/arch/aarch64/raspi3/dmac.c \
	$(TOP_DIR)/arch/aarch64/raspi3/emmc.c \
	$(TOP_DIR)/arch/aarch64/raspi3/irq.c \
	$(TOP_DIR)/arch/aarch64/raspi3/mm.c \
	$(TOP_DIR)/arch/aarch64/raspi3/power.c \
	$(TOP_DIR)/arch/aarch64/raspi3/rand.c \
	$(TOP_DIR)/arch/aarch64/raspi3/timer.c \
	$(TOP_DIR)/arch/aarch64/raspi3/videocore_mailbox.c \

SRCS += $(TOP_DIR)/user/aarch64/raspi3/mailbox.c \

endif


SRCS += $(TOP_DIR)/user/aarch64/user_aarch64.c \


