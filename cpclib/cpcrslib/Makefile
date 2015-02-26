SHELL=/bin/bash

TARGETLIBS=cpcrslib.lib cpcwyzlib.lib
export CDTC_ROOT:=$(shell cd ../../ ; pwd ; )

.PHONY: cpcrslib examples all

default: cpcrslib
	@echo "Notice: in $$PWD default target builds lib only. Consider issuing: 'make examples'"

all: cpcrslib examples

cpcrslib: cpcrslib_SDCC.installtree/.installed cpcrslib_SDCC/examples/.configured

examples: cpcrslib cpcrslib_SDCC/examples/.built


########################################################################

PRODUCT_NAME=cpcrslib_SDCC
URL_RELEASE=https://cpcrslib.googlecode.com/files/cpcrslib%20SDCC%2012.08.2013.zip

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
	@echo "**************** Source archive was downloaded to : $(@)"
	@echo "************************************************************************"

cpcrslib_SDCC/.unpacked: $(ARCHIVE_NAME)
	@echo "************************************************************************"
	@echo "**************** Extracting source from : $^"
	@echo "************************************************************************"
	( set -e ; mkdir -p $(@D) ; cd $(@D) ; unzip ../$< ; cd - ; touch $@ ; )
	@echo "************************************************************************"
	@echo "**************** Source extracted to : $(@D)"
	@echo "************************************************************************"

cpcrslib_SDCC.buildtree/.built: cpcrslib_SDCC/.unpacked
	@echo "************************************************************************"
	@echo "**************** Building in : $(@D) target $(@F)"
	@echo "************************************************************************"
	( set -e ; \
	mkdir -p $(@D) ; \
	LC_ALL=C $(MAKE) --print-directory -f ../cpcrslib_SDCC_SDCC.Makefile -C $(@D) $(TARGET) CPCRSLIB_SRCTREE=../$(<D)/SDCC ; \
	touch "$@" ; \
	)
	@echo "************************************************************************"
	@echo "**************** Build success in : $(@D) target $(@F)"
	@echo "************************************************************************"

cpcrslib_SDCC.installtree/.installed: cpcrslib_SDCC.buildtree/.built
	@echo "************************************************************************"
	@echo "**************** Installing in : $(@D) target $(@F)"
	@echo "************************************************************************"
	( set -e ; mkdir -p $(@D) ; cd $(@D) ; \
	mkdir -p include ; cp ../cpcrslib_SDCC/SDCC/*.h include/ ;  \
	mkdir -p lib ; cp ../cpcrslib_SDCC.buildtree/*.lib lib/ ; \
	cd - ; touch "$@" \
	)
	@echo "************************************************************************"
	@echo "**************** Install success in : $(@D) target $(@F)"
	@echo "************************************************************************"

########################################################################

cpcrslib_SDCC/examples/.configured: cpcrslib_SDCC/.unpacked Makefile
	@echo "************************************************************************"
	@echo "**************** Configuring samples in : $(@D)"
	@echo "************************************************************************"
	( set -e ; \
	cd $(@D) ; \
	for EXAMPLE in */ ; \
	do \
	echo "************************************************************************" ; \
	echo "**************** Configuring sample in : $${EXAMPLE}" ; \
	echo "************************************************************************" ; \
	( \
		cd "$${EXAMPLE}" ; \
		grep -q -e "--fomit-frame-pointer" make.bat && echo "CFLAGS=--fomit-frame-pointer" ; \
		echo PROJNAME=$$( echo "$${EXAMPLE}" | tr -d -c 'A-Za-z0-9_' | grep -o '^........' ; ) ; \
		echo CODELOC=0x4000 ; \
		rm -f crt0* putchar* ; \
	) >"$${EXAMPLE}"/cdtc_project.conf ; \
	$(CDTC_ROOT)/new-sdcc-project.sh "$${EXAMPLE}" ; \
	done ; \
	)
	touch $@
	@echo "************************************************************************"
	@echo "**************** Configured samples in : $(@D)"
	@echo "************************************************************************"
	@echo "**************** You can go in any sample directory (path above) and type make."
	@echo "************************************************************************"

cpcrslib_SDCC/examples/.built: cpcrslib_SDCC/examples/.configured cpcrslib_SDCC.buildtree/.built Makefile
	@echo "************************************************************************"
	@echo "**************** Building samples in : $(@D)"
	@echo "************************************************************************"
	( set -e ; \
	cd $(@D) ; \
	for EXAMPLE in */ ; \
	do \
	echo "************************************************************************" ; \
	echo "**************** Building sample in : $${EXAMPLE}" ; \
	echo "************************************************************************" ; \
	( \
		cd "$${EXAMPLE}" ; \
		$(MAKE) dsk ; \
	) ; \
	done ; \
	)
	touch $@
	@echo "************************************************************************"
	@echo "**************** Built samples in : $(@D)"
	@echo "************************************************************************"
	@echo "**************** You can go in any sample directory (path above) and type make."
	@echo "************************************************************************"

########################################################################

clean:
	-( set -e ; cd cpcrslib_SDCC.buildtree ; $(MAKE) clean ; )

mrproper: #clean
	-rm -rf cpcrslib_SDCC cpcrslib_SDCC.buildtree cpcrslib_SDCC.installtree ._cpcrslib_SDCC *~

distclean: mrproper
	-rm -f $(ARCHIVE_NAME)
