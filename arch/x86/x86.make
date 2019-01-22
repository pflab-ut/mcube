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

#CFLAGS += -fPIC
#CFLAGS += -fPIE
#CFLAGS += -m64
#LDFLAGS += -Ttext=$(TEXT_ADDR)

ifeq ($(CC), clang)
  CROSS_PREFIX = llvm-
  CC	= $(CCACHE) clang
  LD = ld.bfd
#  AS = $(CROSS_PREFIX)as
  AS = nasm
#  ASFLAGS = -f elf64
#  OBJDUMP = $(CROSS_PREFIX)objdump -disassemble -print-imm-hex
  OBJDUMP = objdump -D -M intel
  OBJCOPY = $(CROSS_PREFIX)objcopy
#  CFLAGS += -masm=intel
  LDFLAGS += -T scripts/linker/x86-elf.ld --Map $(MAP)
else
  CROSS_PREFIX = 
  CC = $(CCACHE) $(CROSS_PREFIX)gcc
  LD = ld
#  AS = $(CROSS_PREFIX)as
  AS = nasm
  ASFLAGS = -f elf64
  OBJDUMP = $(CROSS_PREFIX)objdump -D -M intel
  OBJCOPY = $(CROSS_PREFIX)objcopy
#  LDFLAGS += -nostdlib -Ttext=$(TEXT_ADDR) scripts/linker/x86-elf.ld
#  LDFLAGS += -nostdlib
#  CFLAGS += -masm=intel
# MonkOS's CFLAGS and LDFLAGS
#CFLAGS +=  -Qn -g  -m64 -mno-red-zone -mno-mmx -mfpmath=sse  \
#     -ffreestanding -fno-asynchronous-unwind-tables -Wall  -fPIC
#LDFLAGS  += -g -nostdlib -m64 -mno-red-zone -ffreestanding -lgcc -z max-page-size=0x1000   
#		LDFLAGS  +=  -z max-page-size=0x1000
#  LDFLAGS += -N -T scripts/linker/x86-elf.ldp
  LDFLAGS += -nostdinc -nostdlib -T scripts/linker/x86-elf.ldp
#  LDFLAGS += -N -T scripts/linker/x86-elf.ld.org
endif

#CFLAGS += -D__LITTLE_ENDIAN__
#CFLAGS += -mcmodel=large -nostdlib -nodefaultlibs -fno-builtin -fno-stack-protector
#CFLAGS += -fno-pic
#CFLAGS += -fno-pie
#CFLAGS += -fno-pie -Wno-pointer-sign -fno-stack-protector -mfentry

#
# Machine-dependent C Flags:
#
# Use the AMD64 'kernel' code model for reasons stated in our
# head.S bootstrap code.
#
# Disable SSE floating point ops. They need special CPU state
# setup, or several #UD and #NM exceptions will be triggered.
#
# Do not use the AMD64 ABI 'red zone'. This was a bug that
# costed me an entire week to fix!
#
# Basically the zone is 128-byte area _below_ the stack pointer
# that can be used for temporary local state, especially for
# leaf functions.
#
# Ofcouse this red zone is disastrous to be used in the kernel
# since it's where the CPU pushes %ss, %rsp, %rflags, %cs and
# %rip on the event of invoking an interrupt handler. If used,
# it will simply lead to kernel stack corruption.
#
# Check attached docs for extra info on -mcmodel and the zone.
#
CMACH_FLAGS =				\
  -m64					\
  -mcmodel=kernel			\
  -mno-sse				\
  -mno-sse2				\
  -mno-sse3				\
	 -mno-mmx				\
  -mno-3dnow				\
  -mno-red-zone
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
  -fno-pie				\
  -fno-stack-protector			\
  -fno-builtin				\
  -nostdlib				\
  -nostdinc				\
  -iwithprefix include

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


# Share headers between assembly, C, and LD files
CPPFLAGS = -D__KERNEL__
AFLAGS = -D__ASSEMBLY__

# Warn about the sloppy UNIX linkers practice of
# merging global common variables
LDFLAGS += --warn-common

# Our global kernel linker script, after being
# 'cpp' pre-processed from the *.ld source
PROCESSED_LD_SCRIPT = kern/kernel.ldp


#
# Object files listings
#

# Core and Secondary CPUs bootstrap
ASMS = \
	$(TOP_DIR)/arch/x86/boot/head.S	\
	$(TOP_DIR)/arch/x86/boot/trampoline.S	\
	$(TOP_DIR)/arch/x86/boot/rmcall.S	\
	$(TOP_DIR)/arch/x86/boot/e820.S	\
	$(TOP_DIR)/arch/x86/boot/load_ramdisk.S

# Memory management
SRCS +=	\
 $(TOP_DIR)/arch/x86/mm/e820.c	\
 $(TOP_DIR)/arch/x86/mm/page_alloc.c	\
 $(TOP_DIR)/arch/x86/mm/vm_map.c	\

# Devices
SRCS +=	\
 $(TOP_DIR)/arch/x86/dev/serial.c	\
	$(TOP_DIR)/arch/x86/dev/pic.c	\
	$(TOP_DIR)/arch/x86/dev/apic.c	\
 $(TOP_DIR)/arch/x86/dev/ioapic.c	\
 $(TOP_DIR)/arch/x86/dev/pit.c	\
 $(TOP_DIR)/arch/x86/dev/keyboard.c

# Ext2 file system
SRCS +=	\
 $(TOP_DIR)/arch/x86/ext2/ext2.c	\
 $(TOP_DIR)/arch/x86/ext2/ext2_tests.c	\
 $(TOP_DIR)/arch/x86/ext2/file.c	\
 $(TOP_DIR)/arch/x86/ext2/file_tests.c	\
 $(TOP_DIR)/arch/x86/ext2/files_list.c

# Isolated library code
SRCS +=	\
 $(TOP_DIR)/arch/x86/lib/list.c	\
 $(TOP_DIR)/arch/x86/lib/unrolled_list.c	\
 $(TOP_DIR)/arch/x86/lib/hash.c	\
 $(TOP_DIR)/arch/x86/lib/printf.c	\
 $(TOP_DIR)/arch/x86/lib/buffer_dumper.c	\
 $(TOP_DIR)/arch/x86/lib/spinlock.c

# All other kernel objects
SRCS +=		\
 $(TOP_DIR)/arch/x86/kern/idt.c	\
 $(TOP_DIR)/arch/x86/kern/mptables.c	\
 $(TOP_DIR)/arch/x86/kern/smpboot.c	\
 $(TOP_DIR)/arch/x86/kern/sched.c	\
 $(TOP_DIR)/arch/x86/kern/kthread.c	\
 $(TOP_DIR)/arch/x86/kern/percpu.c	\
 $(TOP_DIR)/arch/x86/kern/ramdisk.c	\
 $(TOP_DIR)/arch/x86/kern/main.c

BOOTSECT_ASMS =		\
 $(TOP_DIR)/arch/x86/boot/bootsect.S

BOOTSECT_TARGET = $(TOP_DIR)/build/bootsect
BOOTSECT_BIN = $(BOOTSECT_TARGET).bin


#MBR_TARGET = $(TOP_DIR)/build/mbr
#BOOTMON_TARGET = $(TOP_DIR)/build/bootmon

#MBR_ASMS = $(TOP_DIR)/arch/x86/mbr.S

#BOOTMON_ASMS = $(TOP_DIR)/arch/x86/bootmon.S \
	$(TOP_DIR)/arch/x86/entry16.S \
	$(TOP_DIR)/arch/x86/entry32.S \
	$(TOP_DIR)/arch/x86/entry64.S \


#ASMS = \
	$(TOP_DIR)/arch/x86/asm.S \
	$(TOP_DIR)/arch/x86/trampoline.S \
	$(TOP_DIR)/arch/x86/ap_entry32.S \

#SRCS += \
	$(TOP_DIR)/arch/x86/memory.c \
	$(TOP_DIR)/arch/x86/physmem.c \
	$(TOP_DIR)/arch/x86/slab.c \
	$(TOP_DIR)/arch/x86/arch.c \
	$(TOP_DIR)/arch/x86/acpi.c \
	$(TOP_DIR)/arch/x86/apic.c \
	$(TOP_DIR)/arch/x86/desc.c \
	$(TOP_DIR)/arch/x86/i8254.c \
	$(TOP_DIR)/arch/x86/pgt.c \
	$(TOP_DIR)/arch/x86/strfmt.c \
	$(TOP_DIR)/arch/x86/kernel.c \

#SRCS += \
	$(TOP_DIR)/arch/x86/stdio.c \
	$(TOP_DIR)/arch/x86/init.c \
	$(TOP_DIR)/arch/x86/syscall.c \
	$(TOP_DIR)/arch/x86/thread.c \
	$(TOP_DIR)/arch/x86/timer.c \
	$(TOP_DIR)/arch/x86/cpu.c \
	$(TOP_DIR)/arch/x86/irq.c \


#SRCS += \
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

SRCS += \
	$(TOP_DIR)/arch/x86/init.c \
	$(TOP_DIR)/arch/x86/syscall.c \
	$(TOP_DIR)/arch/x86/thread.c \
	$(TOP_DIR)/arch/x86/timer.c \


#SRCS += \
	$(TOP_DIR)/drivers/uart/uart.c \
	$(TOP_DIR)/drivers/uart/uart_x86.c \


SRCS += \
 $(TOP_DIR)/user/x86/user_x86.c \
