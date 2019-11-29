ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

OUTPUT_DIR ?= $(ROOT_DIR)/output

$(OUTPUT_DIR):
	mkdir -pv $@

buildsystem: $(OUTPUT_DIR)
	cd $(OUTPUT_DIR) && cmake ..

build: buildsystem
	cd $(OUTPUT_DIR) && make


