SHELL=/bin/bash

VARIABLES_AT_MAKEFILE_START := $(.VARIABLES)

TARGETS=build_config.inc

all: $(TARGETS)

########################################################################
# Variables to adjust at each new integrated project
########################################################################

PRODUCT_NAME=caprice32
URL_GIT=https://github.com/cpcitor/caprice32

# You must set EXTRACT_DIR_NAME to match archive content.
EXTRACT_DIR_NAME=caprice32

# If only one generated executable file, set BUILD_TARGET_FILE to path to it.
# It several generated executable files, do not set BUILD_TARGET_FILE.
BUILD_TARGET_FILE=$(EXTRACT_DIR_NAME)/cap32

URL_DOWNLOAD=$(URL_GIT)

ROM_PROVIDER_LOCATION=../rom
ROM_LOCATION=$(ROM_PROVIDER_LOCATION)/roms

.PHONY: all roms

rom: $(ROM_LOCATION)/.extracted

$(ROM_LOCATION)/.extracted:
	$(MAKE) -C $(ROM_PROVIDER_LOCATION)

$(EXTRACT_DIR_NAME)/.unpacked: $(ARCHIVE_NAME)
	@echo "************************************************************************"
	@echo "**************** Extracting source from: $^"
	@echo "************************************************************************"
	git submodule update --init caprice32
	touch $@
	@echo "************************************************************************"
	@echo "**************** Source extracted to: $(@D)"
	@echo "************************************************************************"

LOCAL_ADJUSTSCRIPT=$(EXTRACT_DIR_NAME)_local_adjust.sh

cap32_local.cfg: $(LOCAL_ADJUSTSCRIPT) $(EXTRACT_DIR_NAME)/.unpacked
	@echo "************************************************************************"
	@echo "**************** Local_Adjusting source in: $^"
	@echo "************************************************************************"
	./$(LOCAL_ADJUSTSCRIPT) $(EXTRACT_DIR_NAME)
	touch $@
	@echo "************************************************************************"
	@echo "**************** Source local_adjusted in: $(@D)"
	@echo "************************************************************************"

BUILD_TARGET_FILE?=$(EXTRACT_DIR_NAME)/.built

$(BUILD_TARGET_FILE): cap32_local.cfg
	@echo "************************************************************************"
	@echo "**************** Configuring and build in: $^"
	@echo "************************************************************************"
	( set -e ; cd $(EXTRACT_DIR_NAME) ; LC_ALL=C $(MAKE) --print-directory ; )
	@echo "************************************************************************"
	@echo "**************** Configured and built in: $(@D)"
	@echo "************************************************************************"

build_config.inc: $(BUILD_TARGET_FILE) Makefile
	(set -eu ; \
	{ \
	echo "# with bash do \"source\" this file." ; \
	echo "export PATH=\"\$${PATH}:$$PWD/bin\"" ; \
	} >$@ ; )

run: build_config.inc roms
	( . build_config.inc ; cap32_once )

clean:
	-( set -e ; cd $(EXTRACT_DIR_NAME) ; $(MAKE) clean ; )
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
