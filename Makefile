#
#  Mcube Kernel
# 
#  Hiroyuki Chishiro
#

.SUFFIXES:
.SUFFIXES: .c .o .S .asm

TOP_DIR = .

include $(TOP_DIR)/Rules.make

SRCS += \
 $(TOP_DIR)/kernel/main.c \
 $(TOP_DIR)/kernel/printk.c \
 $(TOP_DIR)/kernel/thread.c \
 $(TOP_DIR)/kernel/task.c \
 $(TOP_DIR)/kernel/sched.c \
 $(TOP_DIR)/kernel/job.c \
 $(TOP_DIR)/kernel/irq.c \
 $(TOP_DIR)/kernel/state.c \
 $(TOP_DIR)/kernel/queue.c \
 $(TOP_DIR)/lib/ctype.c \
 $(TOP_DIR)/lib/math.c \
 $(TOP_DIR)/lib/stdlib.c \
 $(TOP_DIR)/lib/string.c \
 $(TOP_DIR)/lib/buffer.c \
 $(TOP_DIR)/lib/syscall.c \
 $(TOP_DIR)/lib/mm.c \
 $(TOP_DIR)/user/user.c \

BFLAGS = -nostdlib

.PHONY: all configure testconfig defaultconfig testpython
all: $(TARGET)


$(TARGET): $(ASM_OBJS) $(OBJS) $(BOOT_TARGET) $(LOADER_TARGET)
	$(LD) -o $@ $(ASM_OBJS) $(OBJS) $(LDFLAGS)
	$(SIZE) $@
ifeq ($(ARCH_NAME), x86)
	$(OBJCOPY) -O binary $(TARGET) $(BIN)
#	$(TOP_DIR)/scripts/misc/vmdk.py $(BIN) $(TARGET)-flat.vmdk $(TARGET).vmdk
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

$(BUILD_DIR)/%.o: %.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CC) $(CFLAGS) -o $@ -c -MMD -MP -MF $(@:%.o=%.d) $<

# except x86
$(BUILD_DIR)/%.o: %.S
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
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

testpython:
#	@$(FIND) $(PYTHON_DIR) -name "*.py" | xargs pep8 --ignore=E111
#	@$(FIND) $(PYTHON_DIR) -name "*.py" | xargs pychecker
	@$(FIND) $(PYTHON_DIR) -name "*.py" | xargs pyflakes
	@$(FIND) $(PYTHON_DIR) -name "*.py" | xargs pylint


.PHONY: run mrun line size
run:
ifeq ($(ARCH_NAME), sim)
	$(TARGET)
else ifeq ($(ARCH_NAME), x86)
# graphic
#	qemu-system-x86_64 -cdrom $(TARGET).iso
# nographic
	qemu-system-x86_64 -cdrom $(TARGET).iso -nographic -curses
else ifeq ($(ARCH_NAME), arm)
# for UART011
	qemu-system-aarch64 -m 128 -M raspi3 -nographic -kernel $(TARGET)
#	qemu-system-aarch64 -m 128 -M raspi3 -serial mon:stdio -nographic -kernel $(TARGET)
# for MINI UART
#	qemu-system-aarch64 -m 128 -M raspi3 -serial null -serial mon:stdio -nographic -kernel $(TARGET)
else ifeq ($(ARCH_NAME), axis)
	$(RUN_AXIS)
endif

mrun:
	$(RUN_AXIS) "+define+MONITOR_ALL"	

line:
	@$(PYTHON) $(TOP_DIR)/scripts/misc/line.py

size:
	$(SIZE) $(TARGET)

.PHONY: doxy clean buildclean doxyclean docclean

doxy: doxyclean
	@$(DOXYGEN) Doxyfile

clean: doxyclean buildclean docclean
	@for file in $(CLEANFILES); do $(FIND) . -name $$file -delete || exit 1; done
	@$(RM) $(TARGET) $(TARGET).elf $(OBJS) $(DEPS) $(DMPFILE) $(MAP) $(ROMFILE) $(BIN) irun* $(TARGET)-flat.vmdk $(TARGET).vmdk
	@$(RM) -r INCA_libs testconfig

buildclean:
	@$(RM) -r $(BUILD_DIR)

doxyclean:
	@$(RM) -r html latex

docclean:
	@cd $(DOC_DIR); make clean; cd ..

# dependencies
ifneq ($(DEPS),)
-include $(DEPS)
endif

# dependencies
ifneq ($(ASM_DEPS),)
-include $(ASM_DEPS)
endif
