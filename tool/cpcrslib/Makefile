TARGETS=cpcrslib_SDCC.buildtree/cpcrslib.lib cpcrslib_SDCC.buildtree/cpcwyzlib.lib

all: $(TARGETS)

########################################################################

cpcrslib_SDCC_11.03.2012.rar:
	wget -S http://cpcrslib.googlecode.com/files/cpcrslib%20SDCC%2011.03.2012.rar -O $@.tmp && mv $@.tmp $@

cpcrslib_SDCC/.unpacked: cpcrslib_SDCC_11.03.2012.rar
	( set -e ; mkdir $$(dirname "$@") ; cd $$(dirname "$@") ; unrar x ../$< ; cd - ; touch $@ ; )

#cpcrslib_SDCC/.patched: cpcrslib_SDCC_build_patch.patch cpcrslib_SDCC/.unpacked
#	( set -e ; patch -p0 < $< ; touch $@ ; )

cpcrslib_SDCC.buildtree/%: cpcrslib_SDCC/.unpacked
	( set -e ; LC_ALL=C make --print-directory -C $(@D) $(@F) CPCRSLIB_SRCTREE=../$(<D)/SDCC ; )

clean:
	-( set -e ; cd cpcrslib_SDCC.buildtree ; make clean ; )

mrproper: #clean
	-rm -rf cpcrslib_SDCC ._cpcrslib_SDCC *~

distclean: mrproper
	-rm -f cpcrslib_SDCC_11.03.2012.rar
