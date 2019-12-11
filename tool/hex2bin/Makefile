SHELL=/bin/bash

VARIABLES_AT_MAKEFILE_START := $(.VARIABLES)

TARGETS=build_config.inc

.PHONY: all

all: $(TARGETS)

########################################################################
# Variables to adjust at each new integrated project
########################################################################

# Tested release
PRODUCT_NAME=hex2bin
URL_RELEASE=http://sourceforge.net/projects/hex2bin/files/hex2bin/Hex2bin-1.0.10.tar.bz2/download

URL_DOWNLOAD=$(URL_RELEASE)

$(PRODUCT_NAME).downloadlog.txt:
	set -o pipefail ; wget -c -S $(URL_RELEASE) -O $@.downloadeddata -o $@.tmp
	mv -vf $@.tmp $@

$(PRODUCT_NAME)_tarball.ref: $(PRODUCT_NAME).downloadlog.txt
	grep -i -o '$(PRODUCT_NAME)-[^:/]*.tar.bz2' $< | uniq >$@.tmp
	mv -vf $@.tmp $@
	mv -vf $<.downloadeddata $$(cat $@) || echo "Assuming downloaded data is already in place."
	@echo "************************************************************************"
	@echo "**************** Source archive was downloaded to : $$(cat $@)"
	@echo "************************************************************************"

$(PRODUCT_NAME)_srctree.ref: $(PRODUCT_NAME)_tarball.ref
	@echo "************************************************************************"
	@echo "**************** Extracting source from : $$(cat $<)"
	@echo "************************************************************************"
	( set -o pipefail ; tar jxvf "$$( cat $< )" | grep -o '^[^/]*' | uniq >$@.tmp ; )
	if [[ $$( cat $@.tmp | wc -l ) != 1 ]] ; then echo "Error figuring out name of extracted directory. Found:" ; cat $@.tmp ; exit 1 ; fi
	mv -vf "$$(cat $@.tmp)/hex2bin" "$$(cat $@.tmp)/hex2bin.x86-elf-linux" || echo "Could not move away Linux-x86-only precompiled binaries. Hope that's ok."
	mv -vf "$$(cat $@.tmp)/mot2bin" "$$(cat $@.tmp)/mot2bin.x86-elf-linux" || echo "Could not move away Linux-x86-only precompiled binaries. Hope that's ok."
	mv -vf $@.tmp $@
	@echo "************************************************************************"
	@echo "**************** Source extracted to : $$(cat $@)"
	@echo "************************************************************************"

distclean:
	if [[ -f $(PRODUCT_NAME)_srctree.ref ]] ; then $(MAKE) -f Makefile.havesourcetree $(MAKECMDGOALS) ; fi
	rm -rf *.ref

default test build_config.inc clean mrproper install: $(PRODUCT_NAME)_srctree.ref
	$(MAKE) -f Makefile.havesourcetree $(MAKECMDGOALS)

########################################################################
# Debug the makefile
########################################################################
$(foreach v,                                        \
  $(sort $(filter-out $(VARIABLES_AT_MAKEFILE_START) VARIABLES_AT_MAKEFILE_START,$(.VARIABLES))), \
  $(info $(v) = $($(v))))
