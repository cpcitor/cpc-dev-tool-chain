SHELL=/bin/bash

VARIABLES_AT_MAKEFILE_START := $(.VARIABLES)

TARGETS=build_config.inc

all: $(TARGETS)

########################################################################
# Variables to adjust at each new integrated project
########################################################################

PRODUCT_NAME=z88dk
URL_RELEASE=http://sourceforge.net/projects/z88dk/files/z88dk/1.10/z88dk-1.10.1.tgz/download
#URL_LATESTRELEASE=http://sourceforge.net/projects/z88dk/files/latest/download?source=files
#URL_NIGHTLY=http://nightly.z88dk.org/z88dk-latest.tgz

# If the and of the URL does not make a suitable file name, set ARCHIVE_NAME to a suitable file name.
# Example : URL ends in /download -> define ARCHIVE_NAME to something suitable
# Example : URL ends in foo.tar.gz -> do not define ARCHIVE_NAME
ARCHIVE_NAME=$(PRODUCT_NAME)_latest.tgz

# You must set EXTRACT_DIR_NAME to match archive content.
EXTRACT_DIR_NAME=$(PRODUCT_NAME)

# If only one generated executable file, set BUILD_TARGET_FILE to path to it.
# It several generated executable files, do not set BUILD_TARGET_FILE.
# BUILD_TARGET_FILE=$(EXTRACT_DIR_NAME)/...

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

# CVS sometimes broken for weeks. cvs up -D 2013-09-18
#$(EXTRACT_DIR_NAME)/.unpacked:
#	( set -e ; cvs -z3 -d:pserver:anonymous@z88dk.cvs.sourceforge.net:/cvsroot/z88dk co -D 2013-09-18 -P z88dk ; touch $@ ; )

PATCHNAME=$(EXTRACT_DIR_NAME)_build_patch.patch

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
	./z88dk_prerequisites_check.sh
	(set -e ; ( cd "$(@D)" ; LC_ALL=C ./build.sh ; ) ; touch $@ ; )
	@echo "************************************************************************"
	@echo "**************** Configured and built in: $(@D)"
	@echo "************************************************************************"

build_config.inc: $(BUILD_TARGET_FILE) Makefile
	(set -eu ; \
	{ \
	cd $(EXTRACT_DIR_NAME)/ ; eval $$( egrep '(Z80_OZFILES|ZCCCFG)' build.sh ) ; \
	echo "# with bash do SOURCE this file to get a working z88dk config." ; \
	echo "export Z80_OZFILES=\"$${Z80_OZFILES}\"" ; \
	echo "export ZCCCFG=\"$${ZCCCFG}\"" ; \
	echo "export PATH=\"\$${PATH}:$$PWD/bin\"" ; \
	} >$@ ; )

clean:
	-( cd $(EXTRACT_DIR_NAME) && $(MAKE) clean  ;)
	-rm -f $(BUILD_TARGET_FILE)

mrproper:
	-rm -f $(BUILD_TARGET_FILE)
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
