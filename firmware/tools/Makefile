ifeq ($(UNAME_S), Linux)
TARGET_GCC_NAME := gcc-linaro-7.4.1-2019.02-x86_64_$(TARGET_PLAT).tar.xz
TARGET_GCC_URL := https://releases.linaro.org/components/toolchain/binaries/latest-7/$(TARGET_PLAT)/$(TARGET_GCC_NAME)
TARGET_GCC_TAR := $(TOOLS_DIR)/$(TARGET_GCC_NAME)
else ifeq ($(UNAME_S), Darwin)
TARGET_GCC_NAME := gcc-$(TARGET_PLAT)-9-2019-q4-major-mac.tar.bz2
TARGET_GCC_URL := https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/RC2.1/$(TARGET_GCC_NAME)
TARGET_GCC_TAR := $(TOOLS_DIR)/$(TARGET_GCC_NAME)
endif

OPENOCD_URL := https://git.code.sf.net/p/openocd/code
OPENOCD_VER := v0.10.0
OPENOCD_CFG := $(OPENOCD_DIR)/tcl/board/stm32f3discovery.cfg
OPENOCD := $(OPENOCD_DIR)/src/openocd

$(TOOLS_DIR):
	mkdir -pv $@

$(OPENOCD_DIR)/Makefile:
	git clone --branch $(OPENOCD_VER) $(OPENOCD_URL) $(OPENOCD_DIR)

build/openocd: $(OPENOCD_DIR)/Makefile
	cd $(OPENOCD_DIR) && ./bootstrap && ./configure --enable-jlink && $(MAKE)

build/toolchain:
	wget $(TARGET_GCC_URL) -P $(TOOLS_DIR)
	mkdir -pv $(TARGET_GCC_DIR)
	tar -xf $(TARGET_GCC_TAR) -C $(TARGET_GCC_DIR) --strip 1

run/openocd:
	$(OPENOCD) --file $(OPENOCD_CFG) --search $(OPENOCD_DIR)/tcl

run/gdb:
	$(TARGET_GDB) \
		--eval-command="target extended-remote :$(OPENOCD_PORT)" \
		$(OUTPUT_BIN)

clean/openocd:
	rm -rf $(OPENOCD_DIR)

clean/toolchain:
	rm -rf $(TARGET_GCC_DIR)
