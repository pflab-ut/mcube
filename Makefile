#
#  Mcube Kernel
# 
#  Hiroyuki Chishiro
#

.SUFFIXES:
.SUFFIXES: .c .o .S .h

TOP_DIR = .

include $(TOP_DIR)/Rules.make

KERNEL_SRCS = \
 $(TOP_DIR)/kernel/main.c \
 $(TOP_DIR)/kernel/print.c \
 $(TOP_DIR)/kernel/thread.c \
 $(TOP_DIR)/kernel/task.c \
 $(TOP_DIR)/kernel/kmalloc.c \
 $(TOP_DIR)/kernel/sched.c \
 $(TOP_DIR)/kernel/job.c \
 $(TOP_DIR)/kernel/state.c \
 $(TOP_DIR)/kernel/syscall.c \
 $(TOP_DIR)/kernel/queue.c \
 $(TOP_DIR)/kernel/irq.c \

LIB_SRCS = \
 $(TOP_DIR)/lib/buffer.c \
 $(TOP_DIR)/lib/buffer_dumper.c \
 $(TOP_DIR)/lib/ctype.c \
 $(TOP_DIR)/lib/hash.c \
 $(TOP_DIR)/lib/heap.c \
 $(TOP_DIR)/lib/llist.c \
 $(TOP_DIR)/lib/math.c \
 $(TOP_DIR)/lib/mm.c \
 $(TOP_DIR)/lib/spinlock.c \
 $(TOP_DIR)/lib/stdlib.c \
 $(TOP_DIR)/lib/string.c \
 $(TOP_DIR)/lib/unrolled_list.c \

IPC_SRCS = \
 $(TOP_DIR)/ipc/semaphore.c \

FS_SRCS = \
 $(TOP_DIR)/fs/init.c \

USER_SRCS = \
 $(TOP_DIR)/user/user.c \
 $(TOP_DIR)/user/test/test.c \
 $(TOP_DIR)/user/test/test_atomic.c \
 $(TOP_DIR)/user/test/test_bitmap.c \
 $(TOP_DIR)/user/test/test_ext2.c \
 $(TOP_DIR)/user/test/test_hash.c \
 $(TOP_DIR)/user/test/test_kmalloc.c \
 $(TOP_DIR)/user/test/test_list.c \
 $(TOP_DIR)/user/test/test_page_alloc.c \
 $(TOP_DIR)/user/test/test_printk.c \
 $(TOP_DIR)/user/test/test_ring_buf.c \
 $(TOP_DIR)/user/test/test_sched.c \
 $(TOP_DIR)/user/test/test_string.c \
 $(TOP_DIR)/user/test/test_unrolled_list.c \
 $(TOP_DIR)/user/test/test_vm.c \

SRCS += \
	$(KERNEL_SRCS) \
	$(LIB_SRCS) \
	$(IPC_SRCS) \
	$(FS_SRCS) \
	$(USER_SRCS) \


ifeq ($(ARCH_NAME), arm)
FONT_PSF = $(TOP_DIR)/lib/font.psf
FONT_OBJ = $(TOP_DIR)/build/$(FONT_PSF:.psf=.o)
else
FONT_PSF =
FONT_OBJ =
endif


.PHONY: all configure testconfig defaultconfig docker setup
all: $(TARGET)


$(TARGET): $(ASM_OBJS) $(OBJS) $(MBR_TARGET) $(BOOTSECT_TARGET) $(FONT_OBJ)
	$(LD) $(LDFLAGS) $(ASM_OBJS) $(FONT_OBJ) $(OBJS) -o $@
	$(SIZE) $@
	$(OBJDUMP) $@ > $(DMPFILE)
ifeq ($(ARCH_NAME), x86)
	$(OBJCOPY) -O binary $(BOOTSECT_TARGET) $(BOOTSECT_BIN)
	$(OBJCOPY) -O binary $(TARGET) $(BIN)
	$(CAT) $(BOOTSECT_BIN) $(BIN) > $(TARGET).img
	$(PYTHON) scripts/misc/build_hdimage.py
#	$(TOP_DIR)/scripts/misc/vmdk.py $(BIN) $(TARGET)-flat.vmdk $(TARGET).vmdk
else ifeq ($(ARCH_NAME), axis)
	$(OBJCOPY) -O binary $(TARGET) $(BIN)
	$(DUMP) $(BIN) $(DUMPARG) $(ROMFILE)
endif

ifeq ($(ARCH_NAME), x86)
$(BOOTSECT_TARGET): $(BOOTSECT_OBJS)
	$(LD) -N -Ttext 0x0 -o $@ $^
else
$(BOOTSECT_TARGET): # do nothing
endif


ifeq ($(ARCH_NAME), arm)
$(FONT_OBJ): $(FONT_PSF)
ifeq ($(CC), gcc)
	$(LD) -r -b binary -o $@ $(FONT_PSF)
else
	$(LD) --format=binary -r -o $@ $(FONT_PSF) -m aarch64elf
endif
else
$(FONT_OBJ):
endif


$(BUILD_DIR)/%.o: %.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c -MMD -MP -MF $(@:%.o=%.d) $<

$(BUILD_DIR)/%.o: %.S
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -o $@ -c -MMD -MP -MF $(@:%.o=%.d) $<


configure: buildclean
	@$(PYTHON) $(TOP_DIR)/scripts/kconfig/configure.py

testconfig:
	@$(PYTHON) $(TOP_DIR)/scripts/kconfig/testconfig.py

defaultconfig:
	@$(PYTHON) $(TOP_DIR)/scripts/kconfig/defaultconfig.py
	@$(MAKE) configure

docker:
	@$(TOP_DIR)/scripts/docker/docker.sh

setup: 
	@$(TOP_DIR)/scripts/misc/setup_ubuntu18.04.sh

.PHONY: cppcheck flawfinder scan-build pylint pyflakes

cppcheck:
	@$(FIND) $(TOP_DIR) -name "*.c" -or -name "*.h" | xargs cppcheck

flawfinder:
	@$(FIND) $(TOP_DIR) -name "*.c" -or -name "*.h" | xargs flawfinder

scan-build:
	@scan-build make -j$(JOBS)

pylint:
	@$(FIND) $(TOP_DIR) -name "*.py" | xargs pylint #--py3k

pyflakes:
	@$(FIND) $(TOP_DIR) -name "*.py" | xargs pyflakes3



.PHONY: run grun mrun crun cmrun

run:
ifeq ($(ARCH_NAME), sim)
	$(TARGET)
else ifeq ($(ARCH_NAME), x86)
#	qemu-system-x86_64 -cpu core2duo -cdrom $(TARGET).iso -nographic -curses -clock hpet
#	qemu-system-x86_64 -cpu qemu64 -cdrom $(TARGET).iso -nographic -curses
#	qemu-system-x86_64 -m 1024 -smp cores=4,threads=1,sockets=2 \
	-numa node,nodeid=0,cpus=0-3 \
	-numa node,nodeid=1,cpus=4-7 \
	-drive id=disk,format=raw,file=./build/mcube.img,if=none \
	-device ahci,id=ahci \
	-device ide-drive,drive=disk,bus=ahci.0 \
	-boot a -display curses
	qemu-system-x86_64 build/mcube-hd.img -nographic -curses -smp 4
#	qemu-system-x86_64 build/mcube-hd.img -curses -smp 4 -nographic
#	qemu-system-x86_64 -m 1024 -smp cores=2,threads=1,sockets=1 \
	-drive id=disk,format=raw,file=./build/mcube.img,if=none \
	-device ahci,id=ahci \
	-device ide-drive,drive=disk,bus=ahci.0 \
	-boot a -display curses

else ifeq ($(MACHINE_NAME), raspi3)
# for UART011
	qemu-system-aarch64 -M raspi3 -serial mon:stdio -nographic -kernel $(TARGET)
# with dd file
#	qemu-system-aarch64 -M raspi3 -drive file=test.dd,if=sd,format=raw -serial mon:stdio -nographic -kernel $(TARGET)
# for MINI UART
#	qemu-system-aarch64 -M raspi3 -serial null -serial mon:stdio -nographic -kernel $(TARGET)
else ifeq ($(ARCH_NAME), axis)
#	$(RUN_AXIS)
	$(RUN_AXIS) "+define+PRINT_ALL"
#	$(RUN_AXIS_CLUSTER) "+define+PRINT_ALL"
else
	echo "Unknown Architecture"
endif

grun:
ifeq ($(ARCH_NAME), x86)
#	qemu-system-x86_64 -cdrom $(TARGET).iso
#	qemu-system-x86_64 build/mcube-hd.img -curses -smp 4
	qemu-system-x86_64 build/mcube-hd.img -smp 4
else ifeq ($(MACHINE_NAME), raspi3)
# for UART011
	qemu-system-aarch64 -M raspi3 -serial mon:stdio -kernel $(TARGET)
# for MINI UART
#	qemu-system-aarch64 -M raspi3 -serial null -serial mon:stdio -kernel $(TARGET)
endif

mrun:
	$(RUN_AXIS) "+define+MONITOR_ALL +define+PRINT_ALL"

crun:
	$(RUN_AXIS_CLUSTER) "+define+PRINT_ALL"

cmrun:
	$(RUN_AXIS_CLUSTER) "+define+MONITOR_ALL +define+PRINT_ALL"


.PHONY: line size

line:
	@$(PYTHON) $(TOP_DIR)/scripts/misc/line.py

size:
	$(SIZE) $(TARGET)

.PHONY: doxygen clean buildclean doxygenclean docclean

doxygen: doxygenclean
	@$(DOXYGEN) Doxyfile

clean: doxygenclean buildclean
	@for file in $(CLEANFILES); do $(FIND) . -name $$file -delete || exit 1; done
	@$(RM) $(TARGET) $(TARGET).elf $(OBJS) $(DEPS) $(DMPFILE) $(MAP)
	@$(RM) $(ROMFILE) $(BIN) irun* $(TARGET)-flat.vmdk $(TARGET).vmdk
	@$(RM) -r INCA_libs testconfig

buildclean:
	@$(RM) -r $(BUILD_DIR)

doxygenclean:
	@$(RM) -r html latex

# dependencies
ifneq ($(DEPS),)
-include $(DEPS)
endif

# assembler dependencies
ifneq ($(ASM_DEPS),)
-include $(ASM_DEPS)
endif
