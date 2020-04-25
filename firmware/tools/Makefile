ifeq ($(HOST_PLAT), x86_64)
CROSS_GCC_NAME := gcc-linaro-7.4.1-2019.02-x86_64_$(TARGET_PLAT).tar.xz
CROSS_GCC_URL := https://releases.linaro.org/components/toolchain/binaries/latest-7/$(TARGET_PLAT)/$(CROSS_GCC_NAME)
CROSS_GCC_TAR := $(TOOLS_DIR)/$(CROSS_GCC_NAME)
else ifeq ($(HOST_PLAT), macos)
CROSS_GCC_NAME := gcc-$(TARGET_PLAT)-9-2019-q4-major-mac.tar.bz2
CROSS_GCC_URL := https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/RC2.1/$(CROSS_GCC_NAME)
CROSS_GCC_TAR := $(TOOLS_DIR)/$(CROSS_GCC_NAME)
endif

OPENOCD_VER := v0.10.0
OPENOCD_CFG := $(OPENOCD_DIR)/tcl/board/stm32f3discovery.cfg
OPENOCD := $(OPENOCD_DIR)/src/openocd

$(TOOLS_DIR):
	mkdir -pv $@

$(OPENOCD_DIR)/Makefile:
	git clone \
			--branch $(OPENOCD_VER) \
			https://git.code.sf.net/p/openocd/code \
			$(OPENOCD_DIR); \

build/openocd: $(OPENOCD_DIR)/Makefile
	cd $(OPENOCD_DIR) && ./bootstrap && ./configure --enable-jlink && $(MAKE)

build/toolchain:
	wget $(CROSS_GCC_URL) -P $(TOOLS_DIR)
	mkdir -pv $(CROSS_GCC_DIR)
	tar -xf $(CROSS_GCC_TAR) -C $(CROSS_GCC_DIR) --strip 1

run/openocd:
	$(OPENOCD) --file $(OPENOCD_CFG) --search $(OPENOCD_DIR)/tcl

run/gdb:
	$(CROSS_GDB) \
		--eval-command="target remote :$(OPENOCD_PORT)"

clean/openocd:
	rm -rf $(OPENOCD_DIR)

clean/toolchain:
	rm -rf $(CROSS_GCC_DIR)
