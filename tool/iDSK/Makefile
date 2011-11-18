TARGETS=iDSK

all: $(TARGETS)

########################################################################

iDSK.0.13-src.tgz:
	wget -S http://sid.cpc.free.fr/iDSK.0.13-src.tgz -O $@.tmp && mv $@.tmp $@

iDSK.0.13/.unpacked: iDSK.0.13-src.tgz
	( set -e ; tar zxvf $< ; touch $@ ; )

iDSK.0.13/.patched: iDSK.0.13_build_patch.patch iDSK.0.13/.unpacked
	( set -e ; patch -p0 < $< ; touch $@ ; )

iDSK.0.13/iDSK/src/iDSK: iDSK.0.13/.patched
	(set -e ; cd iDSK.0.13/iDSK ; ./configure ; LC_ALL=C make --print-directory ; )

iDSK: iDSK.0.13/iDSK/src/iDSK
	cp -avf $< $@

clean:
	-( set -e ; cd iDSK.0.13/iDSK ; make clean ; )

mrproper: clean
	-rm -rf iDSK.0.13 ._iDSK.0.13 *~

distclean: mrproper
	-rm -f iDSK iDSK.0.13-src.tgz
