#
#  Mcube Kernel
# 
#  Hiroyuki Chishiro
#

CFLAGS += -mcpu=cortex-a53 -fpic -ffreestanding

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
  LDFLAGS += -T scripts/linker/arm-elf.ld --Map $(MAP)
else
#  CFLAGS += -nostartfiles
  CROSS_PREFIX = aarch64-linux-gnu-
  CC = $(CCACHE) $(CROSS_PREFIX)gcc
  LD = $(CROSS_PREFIX)ld
  AS = $(CROSS_PREFIX)as
  OBJDUMP = $(CROSS_PREFIX)objdump -D
  OBJCOPY = $(CROSS_PREFIX)objcopy
  LDFLAGS += -T scripts/linker/arm-elf.ld
endif


ASMS	+=	$(TOP_DIR)/arch/$(ARCH_NAME)/boot.S \
	$(TOP_DIR)/arch/$(ARCH_NAME)/utils.S \
	$(TOP_DIR)/arch/$(ARCH_NAME)/vector.S \
	$(TOP_DIR)/arch/$(ARCH_NAME)/mmu_and_cache.S \

SRCS	+=	$(TOP_DIR)/arch/$(ARCH_NAME)/init.c \
	$(TOP_DIR)/arch/$(ARCH_NAME)/cpu.c \
	$(TOP_DIR)/arch/$(ARCH_NAME)/thread.c \
	$(TOP_DIR)/arch/$(ARCH_NAME)/timer.c \
	$(TOP_DIR)/arch/$(ARCH_NAME)/irq.c \
	$(TOP_DIR)/arch/$(ARCH_NAME)/mm.c \
 $(TOP_DIR)/drivers/uart/uart.c \
	$(TOP_DIR)/drivers/uart/uart_arm.c \
