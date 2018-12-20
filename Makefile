#
#  Mcube Kernel
# 
#  Hiroyuki Chishiro
#

.SUFFIXES:
.SUFFIXES: .c .o .S .asm

TOP_DIR = .

SRCS += \
 $(TOP_DIR)/kernel/main.c \
	$(TOP_DIR)/kernel/print.c \
	$(TOP_DIR)/kernel/thread.c \
 $(TOP_DIR)/kernel/task.c \
 $(TOP_DIR)/kernel/sched.c \
 $(TOP_DIR)/kernel/job.c \
 $(TOP_DIR)/kernel/state.c \
 $(TOP_DIR)/kernel/syscall.c \
 $(TOP_DIR)/kernel/queue.c \
 $(TOP_DIR)/kernel/irq.c \
 $(TOP_DIR)/lib/buffer.c \
 $(TOP_DIR)/lib/ctype.c \
 $(TOP_DIR)/lib/math.c \
 $(TOP_DIR)/lib/mm.c \
 $(TOP_DIR)/lib/stdlib.c \
 $(TOP_DIR)/lib/string.c \
 $(TOP_DIR)/ipc/semaphore.c \
 $(TOP_DIR)/user/user.c \


include $(TOP_DIR)/Rules.make


ifeq ($(ARCH_NAME), arm)
FONT_PSF = $(TOP_DIR)/lib/font.psf
FONT_OBJ = $(TOP_DIR)/build/$(FONT_PSF:.psf=.o)
else
FONT_PSF =
FONT_OBJ =
endif


.PHONY: all configure testconfig defaultconfig pylint pyflakes
all: $(TARGET)


$(TARGET): $(ASM_OBJS) $(OBJS) $(BOOT_TARGET) $(LOADER_TARGET) $(FONT_OBJ)
	$(LD) -o $@ $(ASM_OBJS) $(FONT_OBJ) $(OBJS) $(LDFLAGS)
	$(SIZE) $@
ifeq ($(ARCH_NAME), x86)
	$(OBJCOPY) -O binary $(TARGET) $(BIN)
	$(TOP_DIR)/scripts/misc/vmdk.py $(BIN) $(TARGET)-flat.vmdk $(TARGET).vmdk
	$(TOP_DIR)/scripts/misc/mkcdrom.sh
else ifeq ($(ARCH_NAME), axis)
	$(OBJCOPY) -O binary $(TARGET) $(BIN)
	$(DUMP) $(BIN) $(DUMPARG) $(ROMFILE)
endif
	$(OBJDUMP) $@ > $(DMPFILE)

ifeq ($(ARCH_NAME), x86)
$(BOOT_TARGET): $(BOOT_ASM)
	nasm -D__ASSEMBLY__ -f bin -o $@ -MP -MF $(@:%.sys=%.d) $(BOOT_ASM)
$(LOADER_TARGET): $(LOADER_ASM)
	nasm -D__ASSEMBLY__ -f bin -o $@ -MP -MF $(@:%.sys=%.d) $(LOADER_ASM)
else
$(BOOT_TARGET): # do nothing
$(LOADER_TARGET): # do nothing
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

# except x86
$(BUILD_DIR)/%.o: %.S
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
#	$(CC) -D__ASSEMBLY__ $(CFLAGS) -o $@ -c -MMD -MP -MF $(@:%.o=%.d) $<
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -o $@ -c -MMD -MP -MF $(@:%.o=%.d) $<

# x86
$(BUILD_DIR)/%.o: %.asm
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(AS) -D__ASSEMBLY__ $(ASFLAGS) -o $@ -MP -MF $(@:%.o=%.d) $<



setup: 
	@$(TOP_DIR)/scripts/misc/setup_ubuntu18.04.sh

configure: buildclean
	@$(PYTHON) $(TOP_DIR)/scripts/kconfig/configure.py

testconfig:
	@$(PYTHON) $(TOP_DIR)/scripts/kconfig/testconfig.py

defaultconfig:
	@$(PYTHON) $(TOP_DIR)/scripts/kconfig/defaultconfig.py
	@$(MAKE) configure

docker:
	@$(TOP_DIR)/scripts/docker/docker.sh

pylint:
	@$(FIND) $(TOP_DIR) -name "*.py" | xargs pylint #--py3k

pyflakes:
	@$(FIND) $(TOP_DIR) -name "*.py" | xargs pyflakes3

#	@$(FIND) $(TOP_DIR) -name "*.py" | xargs pep8 --ignore=E111
#	@$(FIND) $(TOP_DIR) -name "*.py" | xargs pychecker


.PHONY: run grun mrun line size

run:
ifeq ($(ARCH_NAME), sim)
	$(TARGET)
else ifeq ($(ARCH_NAME), x86)
	qemu-system-x86_64 -cpu core2duo -cdrom $(TARGET).iso -nographic -curses -clock hpet
#	qemu-system-x86_64 -cpu qemu64 -cdrom $(TARGET).iso -nographic -curses
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
	qemu-system-x86_64 -cdrom $(TARGET).iso
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


line:
	@$(PYTHON) $(TOP_DIR)/scripts/misc/line.py

size:
	$(SIZE) $(TARGET)

.PHONY: doxygen clean buildclean doxygenclean docclean

doxygen: doxygenclean
	@$(DOXYGEN) Doxyfile

clean: doxygenclean buildclean docclean
	@for file in $(CLEANFILES); do $(FIND) . -name $$file -delete || exit 1; done
	@$(RM) $(TARGET) $(TARGET).elf $(OBJS) $(DEPS) $(DMPFILE) $(MAP)
	@$(RM) $(ROMFILE) $(BIN) irun* $(TARGET)-flat.vmdk $(TARGET).vmdk
	@$(RM) -r INCA_libs testconfig

buildclean:
	@$(RM) -r $(BUILD_DIR)

doxygenclean:
	@$(RM) -r html latex

docclean:
	@cd $(DOC_DIR); make clean; cd ..

# dependencies
ifneq ($(DEPS),)
-include $(DEPS)
endif

# assembler dependencies
ifneq ($(ASM_DEPS),)
-include $(ASM_DEPS)
endif
