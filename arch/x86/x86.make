#
#  Mcube Kernel
# 
#  Hiroyuki Chishiro
#

#LDFLAGS	=  $(shell $(CC) $(CFLAGS) -print-file-name=libgcc_eh.a)
TEXT_ADDR = 0x00007c00
#TEXT_ADDR = 0x00017c00

#DATA_ADDR = 0x00010000
#BSS_ADDR = 0x00018000

#LDFLAGS +=

#PRIVATE_LDFLAGS = -Ttext=$(TEXT_ADDR) -N -Bstatic

CFLAGS += -m64
#LDFLAGS += -Ttext=$(TEXT_ADDR)

ifeq ($(CC), clang)
  CROSS_PREFIX = llvm-
  CC	= $(CCACHE) clang
  LD = ld.bfd
#  AS = $(CROSS_PREFIX)as
  AS = nasm
  ASFLAGS = -f elf64
#  OBJDUMP = $(CROSS_PREFIX)objdump -disassemble -print-imm-hex
  OBJDUMP = objdump -D -M intel
  OBJCOPY = $(CROSS_PREFIX)objcopy
  LDFLAGS += -T scripts/linker/x86-elf.ld --Map $(MAP)
else
  CROSS_PREFIX = 
  CC = $(CCACHE) $(CROSS_PREFIX)gcc
  LD = gcc
#  AS = $(CROSS_PREFIX)as
  AS = nasm
  ASFLAGS = -f elf64
  BOOT_ASFLAGS = -f bin
  OBJDUMP = $(CROSS_PREFIX)objdump -D -M intel
  OBJCOPY = $(CROSS_PREFIX)objcopy
#  LDFLAGS += -nostdlib -Ttext=$(TEXT_ADDR) scripts/linker/x86-elf.ld
  LDFLAGS += -g -nostdlib -m64 -mno-red-zone -ffreestanding -lgcc -z max-page-size=0x1000 -T scripts/linker/x86-elf.ld
endif


BOOT_TARGET = build/arch/x86/boot.sys
LOADER_TARGET = build/arch/x86/loader.sys
BOOT_ASM = arch/x86/boot.asm
LOADER_ASM = arch/x86/loader.asm


CFLAGS += -D__LITTLE_ENDIAN__


ASMS = \
	$(TOP_DIR)/arch/x86/start.asm \
	$(TOP_DIR)/arch/x86/memzero.asm \
	$(TOP_DIR)/arch/x86/utils.asm \


SRCS += \
 $(TOP_DIR)/arch/x86/main.c \
 $(TOP_DIR)/arch/x86/mm.c \
 $(TOP_DIR)/arch/x86/acpi.c \
 $(TOP_DIR)/arch/x86/apic.c \
 $(TOP_DIR)/arch/x86/cache.c \
 $(TOP_DIR)/arch/x86/cpu.c \
 $(TOP_DIR)/arch/x86/dsctbl.c \
 $(TOP_DIR)/arch/x86/init.c \
 $(TOP_DIR)/arch/x86/irq.c \
 $(TOP_DIR)/arch/x86/thread.c \
 $(TOP_DIR)/arch/x86/key.c \
 $(TOP_DIR)/arch/x86/pmap.c \
 $(TOP_DIR)/arch/x86/mmu.c \
 $(TOP_DIR)/arch/x86/stdio.c \
 $(TOP_DIR)/arch/x86/timer.c \
	$(TOP_DIR)/arch/x86/tty.c \
 $(TOP_DIR)/arch/x86/hpet.c \
 $(TOP_DIR)/arch/x86/lapic.c \
 $(TOP_DIR)/arch/x86/pit.c \
	$(TOP_DIR)/drivers/uart/uart.c \
	$(TOP_DIR)/drivers/uart/uart_x86.c \

