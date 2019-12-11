SHELL=/bin/bash

VARIABLES_AT_MAKEFILE_START := $(.VARIABLES)

TARGETS=build_config.inc

all: $(TARGETS)

########################################################################

# Tested release
URL_RELEASE=http://sourceforge.net/projects/sdcc/files/sdcc/3.9.0/sdcc-src-3.9.0.tar.bz2/download

URL_DOWNLOAD=$(URL_RELEASE)

sdcc.downloadlog.txt:
	set -o pipefail ; wget -c -S $(URL_RELEASE) -O $@.downloadeddata 2>&1 | tee $@.tmp
	mv -vf $@.tmp $@

sdcc_tarball.ref: sdcc.downloadlog.txt
	grep -i -o 'sdcc-[^:/]*.tar.bz2' $< | uniq >$@.tmp
	mv -vf $@.tmp $@
	mv -vf $<.downloadeddata $$(cat $@)
	@echo "************************************************************************"
	@echo "**************** Source archive was downloaded to : $$(cat $@)"
	@echo "************************************************************************"

sdcc_srctree.ref: sdcc_tarball.ref
	@echo "************************************************************************"
	@echo "**************** Extracting source from : $$(cat $<)"
	@echo "************************************************************************"
	( set -o pipefail ; tar jxvf "$$( cat $< )" | grep -o '^[^/]*' | uniq >$@.tmp ; )
	if [[ $$( cat $@.tmp | wc -l ) != 1 ]] ; then echo "Error figuring out name of extracted directory. Found:" ; cat $@.tmp ; exit 1 ; fi
	mv -vf $@.tmp $@
	@echo "************************************************************************"
	@echo "**************** Source extracted to : $$(cat $@)"
	@echo "************************************************************************"

distclean:
	if [[ -f sdcc_srctree.ref ]] ; then $(MAKE) -f Makefile.havesourcetree $(MAKECMDGOALS) ; fi
	rm -rf *.ref

default test build_config.inc clean mrproper install: sdcc_srctree.ref
	$(MAKE) -f Makefile.havesourcetree $(MAKECMDGOALS)

########################################################################
# Debug the makefile
########################################################################
$(foreach v,                                        \
  $(sort $(filter-out $(VARIABLES_AT_MAKEFILE_START) VARIABLES_AT_MAKEFILE_START,$(.VARIABLES))), \
  $(info $(v) = $($(v))))
