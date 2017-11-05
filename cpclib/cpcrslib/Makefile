SHELL=/bin/bash

VARIABLES_AT_MAKEFILE_START := $(.VARIABLES)

TARGETLIBS=cpcrslib.lib cpcwyzlib
export CDTC_ROOT:=$(shell cd ../../ ; pwd ; )

.PHONY: cpcrslib examples all

default: cpcrslib
	@echo "Notice: in $$PWD default target builds lib only. Consider issuing: 'cd $$PWD ; make examples'"

all: cpcrslib examples



########################################################################
# Variables to adjust at each new integrated project
########################################################################

PRODUCT_NAME=cpcrslib_SDCC

URL_RELEASE=https://github.com/cpcitor/cpcrslib/archive/master.zip
# EXTRACT_DIR_NAME must match the name of extracted directory.
EXTRACT_DIR_NAME=cpcrslib-master
EXTRACT_DIR_NAME?=$(PRODUCT_NAME)

cpcrslib: cpcrslib_SDCC.installtree/.installed $(EXTRACT_DIR_NAME)/examples/.configured

examples: cpcrslib $(EXTRACT_DIR_NAME)/examples/.built

# If the and of the URL does not make a suitable file name, set ARCHIVE_NAME to a suitable file name.
# Example: URL ends in /download -> define ARCHIVE_NAME to something suitable
# Example: URL ends in foo.tar.gz -> do not define ARCHIVE_NAME
ARCHIVE_NAME=$(PRODUCT_NAME)_latest.zip


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
	unzip $<
# 	@echo "************************************************************************"
# 	@echo "**************** Cleaning conflicting files (generated files with info local to the machine): $^"
# 	@echo "************************************************************************"
# 	find cpcrslib_SDCC -type f -not -iname "*.s" -not -iname "*.c" -not -iname "*.h" -not -iname "*.bat" -not -iname LICENSE -not -iname "*.txt" -print0 | xargs -0 rm -v
	touch $@
	@echo "************************************************************************"
	@echo "**************** Source extracted to: $(@D)"
	@echo "************************************************************************"

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

#BUILD_TARGET_FILE?=$(EXTRACT_DIR_NAME)/.built
#$(BUILD_TARGET_FILE): $(EXTRACT_DIR_NAME)/.patched

cpcrslib_SDCC.buildtree/.built: $(EXTRACT_DIR_NAME)/.patched
	@echo "************************************************************************"
	@echo "**************** Building in: $(@D) target $(@F)"
	@echo "************************************************************************"
	( set -e ; \
	mkdir -p $(@D) ; \
	( export LC_ALL="C" ; $(MAKE) --print-directory -C $(@D) -f ../cpcrslib_SDCC_SDCC.Makefile $(TARGET) CPCRSLIB_SRCTREE=../$(<D) ; ) ; \
	touch "$@" ; \
	)
	@echo "************************************************************************"
	@echo "**************** Build success in: $(@D) target $(@F)"
	@echo "************************************************************************"

cpcrslib_SDCC.installtree/.installed: cpcrslib_SDCC.buildtree/.built
	@echo "************************************************************************"
	@echo "**************** Installing in: $(@D) target $(@F)"
	@echo "************************************************************************"
	( set -e ; mkdir -p $(@D) ; cd $(@D) ; \
	mkdir -p include ; cp ../$(EXTRACT_DIR_NAME)/src/cpcrslib/*.h include/ ;  \
	mkdir -p lib ; cp ../cpcrslib_SDCC.buildtree/*.lib lib/ ; \
	cd - ; touch "$@" \
	)
	@echo "************************************************************************"
	@echo "**************** Install success in: $(@D) target $(@F)"
	@echo "************************************************************************"

########################################################################

$(EXTRACT_DIR_NAME)/examples/.configured: $(EXTRACT_DIR_NAME)/.unpacked Makefile
	@echo "************************************************************************"
	@echo "**************** Configuring samples in: $(@D)"
	@echo "************************************************************************"
	( set -e ; \
	cd $(@D) ; \
	for EXAMPLE in */ ; \
	do \
	echo "************************************************************************" ; \
	echo "**************** Configuring sample in: $${EXAMPLE}" ; \
	echo "************************************************************************" ; \
	( \
		cd "$${EXAMPLE}" ; \
		grep -q -e "--fomit-frame-pointer" make.bat && echo "CFLAGS=--fomit-frame-pointer" ; \
		echo PROJNAME=$$( echo "$${EXAMPLE}" | tr -d -c 'A-Za-z0-9_' | grep -o '^........' ; ) ; \
		echo CODELOC=0x4000 ; \
		rm -f crt0* putchar* ; \
	) >"$${EXAMPLE}"/cdtc_project.conf ; \
	$(CDTC_ROOT)/cdtc-project-setup.sh "$${EXAMPLE}" ; \
	done ; \
	)
	touch $@
	@echo "************************************************************************"
	@echo "**************** Configured samples in: $(@D)"
	@echo "************************************************************************"
	@echo "**************** You can go in any sample directory (path above) and type make."
	@echo "************************************************************************"

$(EXTRACT_DIR_NAME)/examples/.built: $(EXTRACT_DIR_NAME)/examples/.configured cpcrslib_SDCC.buildtree/.built Makefile
	@echo "************************************************************************"
	@echo "**************** Building samples in: $(@D)"
	@echo "************************************************************************"
	( set -e ; \
	cd $(@D) ; \
	for EXAMPLE in */ ; \
	do \
	echo "************************************************************************" ; \
	echo "**************** Building sample in: $${EXAMPLE}" ; \
	echo "************************************************************************" ; \
	( \
		cd "$${EXAMPLE}" ; \
		$(MAKE) dsk ; \
	) ; \
	done ; \
	)
	touch $@
	@echo "************************************************************************"
	@echo "**************** Built samples in: $(@D)"
	@echo "************************************************************************"
	@echo "**************** You can go in any sample directory (path above) and type make."
	@echo "************************************************************************"

########################################################################

clean:
	-( set -e ; cd cpcrslib_SDCC.buildtree ; $(MAKE) clean ; )

mrproper: #clean
	-rm -rf $(EXTRACT_DIR_NAME) cpcrslib_SDCC.buildtree cpcrslib_SDCC.installtree ._cpcrslib_SDCC *~

distclean: mrproper
	-rm -f $(ARCHIVE_NAME)

########################################################################
# Debug the makefile
########################################################################
$(foreach v,                                        \
  $(filter-out $(VARIABLES_AT_MAKEFILE_START) VARIABLES_AT_MAKEFILE_START,$(.VARIABLES)), \
  $(info $(v) = $($(v))))
