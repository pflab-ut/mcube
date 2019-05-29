#
#  Mcube Kernel
# 
#  Hiroyuki Chishiro
#


ifeq ($(CC), clang)
  CROSS_PREFIX = llvm-
  CC	= $(CCACHE) clang
  LD = ld.bfd
  OBJDUMP = objdump -D -M intel
  OBJCOPY = $(CROSS_PREFIX)objcopy
  LDFLAGS += -T scripts/linker/x86_64-elf.ld --Map $(MAP)
else
  CROSS_PREFIX = 
  CC = $(CCACHE) $(CROSS_PREFIX)gcc
  LD = ld
  ASFLAGS = -f elf64
  OBJDUMP = $(CROSS_PREFIX)objdump -D
  OBJCOPY = $(CROSS_PREFIX)objcopy
  LDFLAGS += -nostdinc -nostdlib -T scripts/linker/x86_64-elf.ldp
endif


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


# Share headers between assembly, C, and LD files
CPPFLAGS = -D__KERNEL__
AFLAGS = -D__ASSEMBLY__

# Warn about the sloppy UNIX linkers practice of
# merging global common variables
LDFLAGS += --warn-common


BOOTSECT_ASMS =		\
 $(TOP_DIR)/arch/x86_64/bootsect.S

BOOTSECT_TARGET = $(TOP_DIR)/build/bootsect
BOOTSECT_BIN = $(BOOTSECT_TARGET).bin


#
# Object files listings
#

# Core (unmapped)
ASMS = \
 $(TOP_DIR)/arch/x86_64/head.S	\
 $(TOP_DIR)/arch/x86_64/rmcall.S	\
 $(TOP_DIR)/arch/x86_64/e820.S	\
 $(TOP_DIR)/arch/x86_64/load_ramdisk.S

# Core and Secondary CPUs bootstrap (mapped as C codes)
ASMS += $(TOP_DIR)/arch/x86_64/utils.S \
 $(TOP_DIR)/arch/x86_64/trampoline.S	\


# Memory management
SRCS +=	\
 $(TOP_DIR)/arch/x86_64/e820_map.c	\
 $(TOP_DIR)/arch/x86_64/page_alloc.c	\
 $(TOP_DIR)/arch/x86_64/vm_map.c	\

# Devices
SRCS +=	\
 $(TOP_DIR)/arch/x86_64/apic.c	\
 $(TOP_DIR)/arch/x86_64/ioapic.c	\
 $(TOP_DIR)/drivers/misc/keyboard.c \
 $(TOP_DIR)/drivers/uart/uart_x86_64.c	\
 $(TOP_DIR)/arch/x86_64/pic.c	\
 $(TOP_DIR)/arch/x86_64/pit.c


# All other kernel objects
SRCS +=		\
 $(TOP_DIR)/arch/x86_64/idt.c	\
 $(TOP_DIR)/arch/x86_64/mptables.c	\
 $(TOP_DIR)/arch/x86_64/smpboot.c	\
 $(TOP_DIR)/arch/x86_64/sched.c	\
 $(TOP_DIR)/arch/x86_64/kthread.c	\
 $(TOP_DIR)/arch/x86_64/percpu.c	\
 $(TOP_DIR)/arch/x86_64/ramdisk.c	\
 $(TOP_DIR)/arch/x86_64/thread.c \
 $(TOP_DIR)/arch/x86_64/syscall.c \
 $(TOP_DIR)/arch/x86_64/timer.c \
 $(TOP_DIR)/arch/x86_64/init.c



SRCS += \
 $(TOP_DIR)/user/x86_64/test_apic.c \
 $(TOP_DIR)/user/x86_64/test_percpu.c \
 $(TOP_DIR)/user/x86_64/test_pit.c \
 $(TOP_DIR)/user/x86_64/user_x86_64.c \
