SHELL=/bin/bash

VARIABLES_AT_MAKEFILE_START := $(.VARIABLES)

TARGETS=build_config.inc

.PHONY: all

all: $(TARGETS)

########################################################################
# Variables to adjust at each new integrated project
########################################################################

PRODUCT_NAME=playtzx
URL_RELEASE=http://www.worldofspectrum.org/pub/sinclair/tools/unix/playtzx-0.12c.tar.gz

# If the end of the URL does not make a suitable file name, set ARCHIVE_NAME to a suitable file name.
# Example : URL ends in /download -> define ARCHIVE_NAME to something suitable
# Example : URL ends in foo.tar.gz -> do not define ARCHIVE_NAME
#ARCHIVE_NAME=$(PRODUCT_NAME)_latest.tgz

# You must set EXTRACT_DIR_NAME to match archive content.
EXTRACT_DIR_NAME=playtzx-0.12c

# If only one generated executable file, set BUILD_TARGET_FILE to path to it.
# It several generated executable files, do not set BUILD_TARGET_FILE.
BUILD_DIR=$(EXTRACT_DIR_NAME)
BUILD_TARGET_FILE=$(BUILD_DIR)/${PRODUCT_NAME}

URL_DOWNLOAD=$(URL_RELEASE)

ARCHIVE_NAME?=$(notdir $(URL_DOWNLOAD))

$(ARCHIVE_NAME):
	wget -S $(URL_RELEASE) -O $@.tmp
	mv -vf $@.tmp $@
	@echo "************************************************************************"
	@echo "**************** Source archive was downloaded to: $(@)"
	@echo "************************************************************************"

$(EXTRACT_DIR_NAME)/.unpacked: $(ARCHIVE_NAME)
	@echo "************************************************************************"
	@echo "**************** Extracting source from: $^"
	@echo "************************************************************************"
	tar zxvf $<
	touch $@
	@echo "************************************************************************"
	@echo "**************** Source extracted to: $(@D)"
	@echo "************************************************************************"

PATCHNAME=$(EXTRACT_DIR_NAME)_build_patch.patch

.PHONY: patch

patch: $(EXTRACT_DIR_NAME)/.patched

ifeq ($(wildcard $(PATCHNAME)),)
$(EXTRACT_DIR_NAME)/.patched: $(EXTRACT_DIR_NAME)/.unpacked
	@echo "************************************************************************"
	@echo "**************** No patch to apply to: $^"
	@echo "************************************************************************"
	touch $@
	@echo "************************************************************************"
	@echo "**************** Source patched in: $(@D)"
	@echo "************************************************************************"
else
$(EXTRACT_DIR_NAME)/.patched: $(PATCHNAME) $(EXTRACT_DIR_NAME)/.unpacked
	@echo "************************************************************************"
	@echo "**************** Patching source in: $^"
	@echo "************************************************************************"
	patch -p0 < $<
	touch $@
	@echo "************************************************************************"
	@echo "**************** Source patched in: $(@D)"
	@echo "************************************************************************"
endif

BUILD_TARGET_FILE?=$(EXTRACT_DIR_NAME)/.built

$(BUILD_TARGET_FILE): $(EXTRACT_DIR_NAME)/.patched
	@echo "************************************************************************"
	@echo "**************** Configuring and build in: $^"
	@echo "************************************************************************"
	( cd $(BUILD_DIR) ; ./configure ; )
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

#$(PRODUCT_NAME): $(BUILD_TARGET_FILE)
#	cp -avf $< $@

.PHONY: install

install: $(BUILD_TARGET_FILE)
	@ if [[ -z "$PREFIX_BIN" ]] ; then echo >&2 "PREFIX_BIN not set. Aborting." ; exit 1 ; fi
	@echo "************************************************************************"
	@echo "**************** Installing: $^"
	@echo "************************************************************************"
	install -s -D -v --target-directory=$(PREFIX_BIN) $(BUILD_TARGET_FILE)
	@echo "************************************************************************"
	@echo "**************** Configured and built in: $(@D)"
	@echo "************************************************************************"


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
