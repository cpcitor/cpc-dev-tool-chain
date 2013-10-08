SHELL=/bin/bash

TARGETS=build_config.inc

all: $(TARGETS)

########################################################################

# Tested release
URL_RELEASE=http://sourceforge.net/projects/sdcc/files/sdcc/3.3.0/sdcc-src-3.3.0.tar.bz2/download
#http://dl.sourceforge.net/project/sdcc/sdcc/3.3.0/sdcc-src-3.3.0.tar.bz2

URL_DOWNLOAD=$(URL_RELEASE)

sdcc.downloadlog.txt:
	wget -S $(URL_RELEASE) -o $@.tmp -O $@.downloadeddata
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
	[[ -f sdcc_srctree.ref ]] && $(MAKE) -f Makefile.havesourcetree $(MAKECMDGOALS)
	rm -rf *.ref

%: sdcc_srctree.ref
	$(MAKE) -f Makefile.havesourcetree $(MAKECMDGOALS)
