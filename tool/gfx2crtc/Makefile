SHELL=/bin/bash

VARIABLES_AT_MAKEFILE_START := $(.VARIABLES)

TARGETS=build_config.inc

.PHONY: all

all: $(TARGETS)

########################################################################
# Variables to adjust at each new integrated project
########################################################################

PRODUCT_NAME=gfx2crtc

# You must set EXTRACT_DIR_NAME to match archive content.
EXTRACT_DIR_NAME=$(PRODUCT_NAME)

# If only one generated executable file, set BUILD_TARGET_FILE to path to it.
# It several generated executable files, do not set BUILD_TARGET_FILE.
BUILD_DIR=$(EXTRACT_DIR_NAME)
BUILD_TARGET_FILE=$(EXTRACT_DIR_NAME)/cap32

ROM_PROVIDER_LOCATION=../rom
ROM_LOCATION=$(ROM_PROVIDER_LOCATION)/roms

.PHONY: all roms

rom: $(ROM_LOCATION)/.extracted

$(ROM_LOCATION)/.extracted:
	$(MAKE) -C $(ROM_PROVIDER_LOCATION)

$(EXTRACT_DIR_NAME)/.unpacked:
	@echo "************************************************************************"
	@echo "**************** Extracting source from: $^"
	@echo "************************************************************************"
	git submodule update --init $(EXTRACT_DIR_NAME)
	touch $@
	@echo "************************************************************************"
	@echo "**************** Source extracted to: $(@D)"
	@echo "************************************************************************"

BUILD_TARGET_FILE?=$(EXTRACT_DIR_NAME)/gfx2crtc

$(BUILD_TARGET_FILE): $(EXTRACT_DIR_NAME)/.unpacked
	@echo "************************************************************************"
	@echo "**************** Configuring and build in: $^"
	@echo "************************************************************************"
	$(MAKE) -C "$(BUILD_DIR)" --print-directory
	touch "$@"
	@echo "************************************************************************"
	@echo "**************** Configured and built in: $(@D)"
	@echo "************************************************************************"

build_config.inc: $(BUILD_TARGET_FILE) Makefile
	(set -eu ; \
	{ \
	echo "# with bash do \"source\" this file." ; \
	cd "$(<D)" ; \
	echo "export PATH=\"\$${PATH}:$$PWD\"" ; \
	} >$@ ; )

run: build_config.inc roms
	( . build_config.inc ; cap32_once )

clean:
	-$(MAKE) -C "$(BUILD_DIR)" clean
	-rm -f $(BUILD_TARGET_FILE)

mrproper:
	-rm -f $(BUILD_TARGET_FILE)
	-rm -f $(TARGETS)
	-rm -rf $(EXTRACT_DIR_NAME) ._$(EXTRACT_DIR_NAME) *~
	-rm -f $(PRODUCT_NAME)

distclean: mrproper
	-rm -f $(ARCHIVE_NAME)

########################################################################
# Debug the makefile
########################################################################
$(foreach v,                                        \
  $(filter-out $(VARIABLES_AT_MAKEFILE_START) VARIABLES_AT_MAKEFILE_START,$(.VARIABLES)), \
  $(info $(v) = $($(v))))
