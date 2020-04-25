PROFILE ?= debug
STRIP_BIN ?= 1
OPENOCD_PORT ?= 3333

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
TARGET_PLAT := arm-eabi
endif
ifeq ($(UNAME_S),Darwin)
TARGET_PLAT := arm-none-eabi
endif

ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
TOOLS_DIR := $(ROOT_DIR)/tools
OUTPUT_DIR := $(ROOT_DIR)/output
INCLUDE_DIR := $(ROOT_DIR)/include
SRC_DIR := $(ROOT_DIR)/src
OPENOCD_DIR := $(TOOLS_DIR)/openocd
TARGET_GCC_DIR := $(TOOLS_DIR)/$(TARGET_PLAT)

TARGET_PREFIX := $(TARGET_GCC_DIR)/bin/$(TARGET_PLAT)
TARGET_GCC := $(TARGET_PREFIX)-gcc
TARGET_GDB := $(TARGET_PREFIX)-gdb
TARGET_OBJCOPY := $(TARGET_PREFIX)-objcopy
TARGET_STRIP := $(TARGET_PREFIX)-strip

OUTPUT_BIN := $(OUTPUT_DIR)/stm32f3disco.bin
OUTPUT_BIN_SYMS := $(OUTPUT_DIR)/stm32f3disco.debug

GCC_OPTS := \
	-o $(OUTPUT_BIN) \
	-Os \
	-mcpu=cortex-m4 -march=armv7e-m \
	-mfloat-abi=hard -mfpu=fpv4-sp-d16 \
	-ffreestanding \
	--specs=nosys.specs -lnosys \
	-fno-asynchronous-unwind-tables \
	-T $(SRC_DIR)/startup/stm32f303xc.ld

ifeq ($(PROFILE),debug)
GCC_OPTS += -g
endif

ST_INCLUDE_DIR := $(INCLUDE_DIR)/st
STM32_USB_LIB_DIR := $(ST_INCLUDE_DIR)/STM32_USB_Device_Library
BSP_LIB_DIR := $(ST_INCLUDE_DIR)/BSP
STM32_USB_INCLUDE := $(STM32_USB_LIB_DIR)/Core/Inc
STM32_USB_HID_INCLUDE := $(STM32_USB_LIB_DIR)/Class/HID/Inc
CMSIS_STM32F3_INCLUDE := $(ST_INCLUDE_DIR)/CMSIS/Device/ST/STM32F3xx/Include
CMSIS_INCLUDE := $(ST_INCLUDE_DIR)/CMSIS/Include
HAL_INCLUDE := $(ST_INCLUDE_DIR)/STM32F3xx_HAL_Driver/Inc
BSP_INCLUDE := \
	$(BSP_LIB_DIR) \
	$(BSP_LIB_DIR)/Components/Common \
	$(BSP_LIB_DIR)/Components/l3gd20 \
	$(BSP_LIB_DIR)/Components/lsm303dlhc
GCC_INCLUDES := \
	-I$(INCLUDE_DIR) \
	-I$(ST_INCLUDE_DIR) \
	-I$(STM32_USB_INCLUDE) \
	-I$(STM32_USB_HID_INCLUDE) \
	-I$(CMSIS_INCLUDE) \
	-I$(CMSIS_STM32F3_INCLUDE) \
	-I$(HAL_INCLUDE) \
	$(addprefix -I, $(BSP_INCLUDE))

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
GCC_SOURCES := \
	$(STM32_USB_SOURCES) \
	$(STM32_USB_HID_SOURCES) \
	$(HAL_SOURCES) \
	$(BSP_SOURCES) \
	$(ST_SOURCES) \
	$(SOURCES)

export UNAME_S TARGET_PLAT 
export ROOT_DIR TOOLS_DIR
export OPENOCD_PORT OPENOCD_DIR
export TARGET_GCC_DIR TARGET_GDB
export OUTPUT_BIN

$(OUTPUT_DIR):
	mkdir -pv $@

$(TOOLS_DIR):
	mkdir -pv $@

binary: $(OUTPUT_DIR)
	$(TARGET_GCC) $(GCC_OPTS) $(GCC_INCLUDES) $(GCC_SOURCES)
ifeq ($(STRIP_BIN),1)
	$(TARGET_OBJCOPY) --only-keep-debug $(OUTPUT_BIN) $(OUTPUT_BIN_SYMS) 
	$(TARGET_STRIP) --strip-debug --strip-unneeded $(OUTPUT_BIN)
	cd $(OUTPUT_DIR) && $(TARGET_OBJCOPY) \
		--add-gnu-debuglink=$(notdir $(OUTPUT_BIN_SYMS)) \
		$(OUTPUT_BIN)
endif

flash:
	cd $(TOOLS_DIR) && $(MAKE) $@

build/%: $(TOOLS_DIR)
	cd $(TOOLS_DIR) && $(MAKE) $@

run/%: $(TOOLS_DIR)
	cd $(TOOLS_DIR) && $(MAKE) $@

clean:
	rm -rf $(OUTPUT_DIR)

clean/%: 
ifeq ($(notdir $*), "tools")
	rm -rf $(TOOLS_DIR)/*
else
	cd $(TOOLS_DIR) && $(MAKE) $@
endif	
	