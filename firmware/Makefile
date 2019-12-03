ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

HOST_PLAT ?= macos
TOOLS_DIR ?= $(ROOT_DIR)/tools
OUTPUT_DIR ?= $(ROOT_DIR)/output
OUTPUT_BIN ?= $(OUTPUT_DIR)/stm32f3disco.bin
PROFILE ?= debug

ifeq ($(HOST_PLAT), x86_64)
TARGET_PLAT := arm-eabi
CROSS_GCC_NAME := gcc-linaro-7.4.1-2019.02-x86_64_$(TARGET_PLAT).tar.xz
CROSS_GCC_URL := https://releases.linaro.org/components/toolchain/binaries/latest-7/$(TARGET_PLAT)/$(CROSS_GCC_NAME)
CROSS_GCC_TAR := $(TOOLS_DIR)/$(CROSS_GCC_NAME)
else ifeq ($(HOST_PLAT), macos)
TARGET_PLAT := arm-none-eabi
CROSS_GCC_NAME := gcc-$(TARGET_PLAT)-9-2019-q4-major-mac.tar.bz2
CROSS_GCC_URL := https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/RC2.1/$(CROSS_GCC_NAME)
CROSS_GCC_TAR := $(TOOLS_DIR)/$(CROSS_GCC_NAME)
endif

CROSS_GCC_DIR := $(TOOLS_DIR)/$(TARGET_PLAT)
CROSS_GCC := $(CROSS_GCC_DIR)/bin/$(TARGET_PLAT)-gcc
CROSS_GDB := $(CROSS_GCC_DIR)/bin/$(TARGET_PLAT)-gdb

OPENOCD_DIR := $(TOOLS_DIR)/openocd
OPENOCD_VER := v0.10.0
OPENOCD_CFG := $(OPENOCD_DIR)/tcl/board/stm32f3discovery.cfg
OPENOCD := $(OPENOCD_DIR)/src/openocd

INCLUDE := $(ROOT_DIR)/include
SRC_DIR := $(ROOT_DIR)/src

ST_INCLUDE := $(INCLUDE)/st
STM32_USB_LIB_DIR := $(ST_INCLUDE)/STM32_USB_Device_Library
BSP_LIB_DIR := $(ST_INCLUDE)/BSP
STM32_USB_INCLUDE := $(STM32_USB_LIB_DIR)/Core/Inc
STM32_USB_HID_INCLUDE := $(STM32_USB_LIB_DIR)/Class/HID/Inc
CMSIS_STM32F3_INCLUDE := $(ST_INCLUDE)/CMSIS/Device/ST/STM32F3xx/Include
CMSIS_INCLUDE := $(ST_INCLUDE)/CMSIS/Include
HAL_INCLUDE := $(ST_INCLUDE)/STM32F3xx_HAL_Driver/Inc
BSP_INCLUDE := \
	$(BSP_LIB_DIR) \
	$(BSP_LIB_DIR)/Components/Common \
	$(BSP_LIB_DIR)/Components/l3gd20 \
	$(BSP_LIB_DIR)/Components/lsm303dlhc

ST_SRC_DIR := $(SRC_DIR)/st
STM32_USB_DIR := $(ST_SRC_DIR)/STM32_USB_Device_Library
CMSIS_SRC_DIR := $(ST_SRC_DIR)/CMSIS
HAL_SRC_DIR := $(ST_SRC_DIR)/STM32F3xx_HAL_Driver
BSP_SRC_DIR := $(ST_SRC_DIR)/BSP

ST_SOURCES := \
	$(SRC_DIR)/selftest.c \
	$(SRC_DIR)/stm32f3xx_it.c \
	$(SRC_DIR)/system_stm32f3xx.c \
	$(SRC_DIR)/usbd_conf.c \
	$(SRC_DIR)/usbd_desc.c
STM32_USB_SOURCES := $(wildcard $(STM32_USB_DIR)/*.c)
STM32_USB_HID_SOURCES := $(STM32_USB_LIB_DIR)/Class/HID/Src/usbd_hid.c
HAL_SOURCES := \
	$(wildcard $(HAL_SRC_DIR)/stm32f3xx_hal_*.c) \
	$(HAL_SRC_DIR)/stm32f3xx_hal.c
BSP_SOURCES := \
	$(BSP_LIB_DIR)/Components/l3gd20/l3gd20.c \
	$(BSP_LIB_DIR)/Components/lsm303dlhc/lsm303dlhc.c \
	$(BSP_SRC_DIR)/stm32f3_discovery.c \
	$(BSP_SRC_DIR)/stm32f3_discovery_accelerometer.c \
	$(BSP_SRC_DIR)/stm32f3_discovery_gyroscope.c

SOURCES := \
	$(SRC_DIR)/startup/stm32f303xc.s \
	$(SRC_DIR)/main.c

GCC_OPTS := \
	-o $(OUTPUT_BIN) \
	-mcpu=cortex-m4 -march=armv7e-m \
	-mfloat-abi=hard -mfpu=fpv4-sp-d16 \
	-ffreestanding -flto \
	--specs=nosys.specs -lnosys \
	-fno-asynchronous-unwind-tables \
	-T $(SRC_DIR)/startup/stm32f303xc.ld
GCC_INCLUDES := \
	-I$(INCLUDE) \
	-I$(ST_INCLUDE) \
	-I$(STM32_USB_INCLUDE) \
	-I$(STM32_USB_HID_INCLUDE) \
	-I$(CMSIS_INCLUDE) \
	-I$(CMSIS_STM32F3_INCLUDE) \
	-I$(HAL_INCLUDE) \
	$(addprefix -I, $(BSP_INCLUDE))
GCC_SOURCES := \
	$(STM32_USB_SOURCES) \
	$(STM32_USB_HID_SOURCES) \
	$(HAL_SOURCES) \
	$(BSP_SOURCES) \
	$(ST_SOURCES) \
	$(SOURCES)

ifeq ($(PROFILE),debug)
GCC_OPTS += -g
endif

$(OUTPUT_DIR):
	mkdir -pv $@

$(TOOLS_DIR):
	mkdir -pv $@

toolchain: $(TOOLS_DIR)
	wget $(CROSS_GCC_URL) -P $(TOOLS_DIR)
	mkdir -pv $(CROSS_GCC_DIR)
	tar -xf $(CROSS_GCC_TAR) -C $(CROSS_GCC_DIR) --strip 1
	
openocd:
	if [ ! -d "$(OPENOCD_DIR)" ]; then \
		git clone \
			--branch $(OPENOCD_VER) \
			https://git.code.sf.net/p/openocd/code \
			$(OPENOCD_DIR); \
	fi
	cd $(OPENOCD_DIR) && ./bootstrap && ./configure && $(MAKE)

binary: $(OUTPUT_DIR)
	$(CROSS_GCC) $(GCC_OPTS) $(GCC_INCLUDES) $(GCC_SOURCES)

run_openocd:
	$(OPENOCD) --file $(OPENOCD_CFG) --search $(OPENOCD_DIR)/tcl

load:
	$(CROSS_GDB) \
		--eval-command="target remote :3333" \
		--eval-command="monitor reset" \
		--eval-command="target remote :3333" \
		--eval-command="load" \
		$(OUTPUT_BIN)

clean:
	rm -rf $(OUTPUT_DIR)

clean_tools:
	rm -rf $(TOOLS_DIR)

clean_all: clean clean_tools