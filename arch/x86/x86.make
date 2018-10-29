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


#PRIVATE_LDFLAGS = -Ttext=$(TEXT_ADDR) -N -Bstatic

CFLAGS += -fPIC
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
  CFLAGS += -masm=intel
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
  LDFLAGS += -nostdlib
  CFLAGS += -masm=intel
# MonkOS's CFLAGS and LDFLAGS
#CFLAGS +=  -Qn -g  -m64 -mno-red-zone -mno-mmx -mfpmath=sse  \
#     -ffreestanding -fno-asynchronous-unwind-tables -Wall  -fPIC
#LDFLAGS  += -g -nostdlib -m64 -mno-red-zone -ffreestanding -lgcc -z max-page-size=0x1000   
LDFLAGS  +=  -z max-page-size=0x1000   
  LDFLAGS += -T scripts/linker/x86-elf.ld
endif


BOOT_TARGET = build/arch/x86/boot.sys
LOADER_TARGET = build/arch/x86/loader.sys
BOOT_ASM = arch/x86/boot.asm
LOADER_ASM = arch/x86/loader.asm


CFLAGS += -D__LITTLE_ENDIAN__


ASMS = \
	$(TOP_DIR)/arch/x86/start.asm \
 $(TOP_DIR)/arch/x86/interrupt.asm \
	$(TOP_DIR)/arch/x86/utils.asm \

SRCS += \
 $(TOP_DIR)/arch/x86/mm.c \
 $(TOP_DIR)/arch/x86/acpi.c \
 $(TOP_DIR)/arch/x86/apic.c \
 $(TOP_DIR)/arch/x86/dump.c \
 $(TOP_DIR)/arch/x86/exception.c \
 $(TOP_DIR)/arch/x86/syscall.c \
 $(TOP_DIR)/arch/x86/keyboard.c \
 $(TOP_DIR)/arch/x86/page.c \
 $(TOP_DIR)/arch/x86/cache.c \
 $(TOP_DIR)/arch/x86/cpu.c \
 $(TOP_DIR)/arch/x86/heap.c \
 $(TOP_DIR)/arch/x86/init.c \
 $(TOP_DIR)/arch/x86/irq.c \
 $(TOP_DIR)/arch/x86/thread.c \
 $(TOP_DIR)/arch/x86/pmap.c \
 $(TOP_DIR)/arch/x86/mmu.c \
 $(TOP_DIR)/arch/x86/stdio.c \
 $(TOP_DIR)/arch/x86/shell.c \
 $(TOP_DIR)/arch/x86/timer.c \
	$(TOP_DIR)/arch/x86/tty.c \
 $(TOP_DIR)/arch/x86/hpet.c \
 $(TOP_DIR)/arch/x86/lapic.c \
 $(TOP_DIR)/arch/x86/pit.c \
	$(TOP_DIR)/drivers/uart/uart.c \
	$(TOP_DIR)/drivers/uart/uart_x86.c \


SRCS += \
 $(TOP_DIR)/user/x86/user_x86.c \
