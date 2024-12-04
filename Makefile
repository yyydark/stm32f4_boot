CONFIG_BOOTLOADER := y

MAKEFLAGS += -rR

PROJECT_NAME := bulid

PWD := $(shell pwd)

ifeq ($(CONFIG_BOOTLOADER), y)
TARGET ?= boot
else
TARGET ?= app
endif

INTERFACE_CFG := jlink.cfg
TARGET_CFG := stm32f4x.cfg


BUILD_BASE := output

ifeq ($(CONFIG_BOOTLOADER), y)
BUILD := $(BUILD_BASE)/$(PROJECT_NAME)_boot
else
BUILD := $(BUILD_BASE)/$(PROJECT_NAME)
endif

DEBUG ?=
V ?=

CROSS_COMPILE ?= tools/toolchain/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-
KCONF := scripts/kconf
ECHO := echo
CP := cp
MKDIR := mkdir -p
PYTHON := python3
OS = $(uname -s)

ifeq ($(V),)
QUITE := @
endif

PLATFORM := stm32f4
MCU := -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
LDSCRIPT := platform/stm32f407vetx_flash.ld


LIBS := -lm

P_DEF :=
P_DEF += STM32F40_41xxx\
		 USE_STDPERIPH_DRIVER\
		 HSE_VALUE=8000000

ifeq ($(DEBUG), y)
P_DEF += DEBUG
endif

s_inc-y = boot \
		  boot/override \
		  platform/cmsis/core \
		  platform/cmsis/device \
		  platform/driver/inc \
		  driver/led \
		  driver/delay

s_dir-y = boot \
		  boot/override \
		  platform/cmsis/core \
		  platform/cmsis/device \
		  platform/driver/src \
		  driver/led \
		  driver/delay

ifeq ($(DEBUG), y)

s_src-y += debug

endif

s_src-y += $(wildcard $(addsuffix /*.c, $(s_dir-y)))
s_src-y += platform/cmsis/device/startup_stm32f40_41xxx.s

P_INC := $(sort $(s_inc-y))
P_SRC := $(sort $(s_src-y))

A_FLAGS = $(MCU) -g
C_FLAGS = $(MCU) -g
C_FLAGS += -std=gnu11
C_FLAGS += -ffunction-sections -fdata-sections -fno-builtin -fno-strict-aliasing
C_FLAGS += -Wall -Werror
ifeq ($(DEBUG), y)
C_FLAGS += -Wno-unused-variable -Wno-comment -Wno-unused-function -Wno-unused-value
endif
C_FLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

ifeq ($(DEBUG), y)
C_FLAGS += -O0
else
C_FLAGS += -Os
endif

L_FLAGS = $(MCU)
L_FLAGS += -T$(LDSCRIPT)
L_FLAGS += -static -specs=nano.specs
L_FLAGS += -Wl,-cref
L_FLAGS += -Wl,-Map=$(BUILD)/$(TARGET).map
L_FLAGS += -Wl,--gc-sections
L_FLAGS += -Wl,--start-group
L_FLAGS += -Wl,$(LIBS)
L_FLAGS += -Wl,--end-group

D_FLAGS := 
D_FLAGS += -f
D_FLAGS += interface/$(INTERFACE_CFG)
D_FLAGS += -f
D_FLAGS += target/$(TARGET_CFG)
D_FLAGS += -c
D_FLAGS += "program $(BUILD)/$(TARGET).elf verify reset exit"


B_LIB := $(addprefix $(BUILD)/, $(P_LIB))
B_INC := $(addprefix -I, $(P_INC))
B_DEF := $(addprefix -D, $(P_DEF))
B_OBJ := $(P_SRC)
B_OBJ := $(B_OBJ:%.c=%.o)
B_OBJ := $(B_OBJ:%.s=%.o)
B_OBJ := $(addprefix $(BUILD)/, $(B_OBJ))
B_DEP := $(B_OBJ:%.o=%.d)

LINKERFILE := $(LDSCRIPT)

FORCE_RELY := Makefile

.PHONY: FORCE _all all test clean distclean $(BUILD)/$(TARGET) download

all: $(BUILD)/$(TARGET)

-include $(B_DEP)

$(BUILD)/%.o: %.s $(FORCE_RELY)
	$(QUITE)$(ECHO) "  AS       $@"
	$(QUITE)$(MKDIR) $(dir $@)
	$(QUITE)$(CROSS_COMPILE)as -c $(A_FLAGS) $< -o $@

$(BUILD)/%.o: %.c $(FORCE_RELY)
	$(QUITE)$(ECHO) "  CC       $@"
	$(QUITE)$(MKDIR) $(dir $@)
	$(QUITE)$(CROSS_COMPILE)gcc -c $(C_FLAGS) $(B_INC) $(B_DEF) $< -o $@

$(BUILD)/$(TARGET): $(B_OBJ) $(LINKERFILE) $(FORCE_RELY)
	$(QUITE)$(MKDIR) $(dir $@)
	$(QUITE)$(ECHO) "   LD       $@.elf"
	$(QUITE)$(CROSS_COMPILE)gcc $(L_FLAGS) $(B_OBJ) -o $@.elf
	$(QUITE)$(ECHO) "   OBJ      $@.hex"
	$(QUITE)$(CROSS_COMPILE)objcopy -O ihex $@.elf $@.hex
	$(QUITE)$(ECHO) "   OBJ      $@.bin"
	$(QUITE)$(CROSS_COMPILE)objcopy -O binary -S $@.elf $@.bin
	$(QUITE)$(CROSS_COMPILE)size $@.elf
	$(QUITE)$(ECHO) "  BUILD FINISH"

clean:
	$(QUITE)rm -rf $(BUILD)
	$(QUITE)$(ECHO) "clean up"

distclean:
	$(QUITE)rm -rf $(BUILD_BASE)
	$(QUITE)$(ECHO) "distclean up"

test:
	$(QUITE)$(ECHO) $(BUILD) $(BUILD_BASE)

download:
	$(QUITE)openocd $(D_FLAGS)

