SHELL=/bin/bash

TARGETS=build_config.inc

all: $(TARGETS)

########################################################################

# Tested release
URL_RELEASE=http://sourceforge.net/projects/hex2bin/files/hex2bin/Hex2bin-1.0.10.tar.bz2/download

URL_DOWNLOAD=$(URL_RELEASE)

hex2bin.downloadlog.txt:
	wget -S $(URL_RELEASE) -o $@.tmp -O $@.downloadeddata
	mv -vf $@.tmp $@

hex2bin_tarball.ref: hex2bin.downloadlog.txt
	grep -i -o 'hex2bin-[^:/]*.tar.bz2' $< | uniq >$@.tmp
	mv -vf $@.tmp $@
	mv -vf $<.downloadeddata $$(cat $@) || echo "Assuming downloaded data is already in place."
	@echo "************************************************************************"
	@echo "**************** Source archive was downloaded to : $$(cat $@)"
	@echo "************************************************************************"

hex2bin_srctree.ref: hex2bin_tarball.ref
	@echo "************************************************************************"
	@echo "**************** Extracting source from : $$(cat $<)"
	@echo "************************************************************************"
	( set -o pipefail ; tar jxvf "$$( cat $< )" | grep -o '^[^/]*' | uniq >$@.tmp ; )
	if [[ $$( cat $@.tmp | wc -l ) != 1 ]] ; then echo "Error figuring out name of extracted directory. Found:" ; cat $@.tmp ; exit 1 ; fi
	mv -vf "$$(cat $@.tmp)/hex2bin" "$$(cat $@.tmp)/hex2bin.i386" || echo "Could not move away i386-only precompiled binaries. Hope that's ok."
	mv -vf "$$(cat $@.tmp)/mot2bin" "$$(cat $@.tmp)/mot2bin.i386" || echo "Could not move away i386-only precompiled binaries. Hope that's ok."
	mv -vf $@.tmp $@
	@echo "************************************************************************"
	@echo "**************** Source extracted to : $$(cat $@)"
	@echo "************************************************************************"

distclean:
	[[ -f hex2bin_srctree.ref ]] && $(MAKE) -f Makefile.havesourcetree $(MAKECMDGOALS)
	rm -rf *.ref

%: hex2bin_srctree.ref
	$(MAKE) -f Makefile.havesourcetree $(MAKECMDGOALS)
