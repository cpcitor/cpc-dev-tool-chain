TARGETS=build_config.inc

all: $(TARGETS)

########################################################################

iDSK.0.13-src.tgz:
	wget -S http://sid.cpc.free.fr/iDSK.0.13-src.tgz -O $@.tmp && mv $@.tmp $@
	@echo "************************************************************************"
	@echo "**************** Source archive was downloaded to : $(@)"
	@echo "************************************************************************"

iDSK.0.13/.unpacked: iDSK.0.13-src.tgz
	@echo "************************************************************************"
	@echo "**************** Extracting source from : $^"
	@echo "************************************************************************"
	( set -e ; tar zxvf $< ; touch $@ ; )
	@echo "************************************************************************"
	@echo "**************** Source extracted to : $(@D)"
	@echo "************************************************************************"

iDSK.0.13/.patched: iDSK.0.13_build_patch.patch iDSK.0.13/.unpacked
	@echo "************************************************************************"
	@echo "**************** Patching source in : $^"
	@echo "************************************************************************"
	( set -e ; patch -p0 < $< ; touch $@ ; )
	@echo "************************************************************************"
	@echo "**************** Source patched in : $(@D)"
	@echo "************************************************************************"

iDSK.0.13/iDSK/src/iDSK: iDSK.0.13/.patched
	@echo "************************************************************************"
	@echo "**************** Configuring and build in : $^"
	@echo "************************************************************************"
	( set -e ; cd iDSK.0.13/iDSK ; ./configure ; LC_ALL=C $(MAKE) --print-directory ; )
	@echo "************************************************************************"
	@echo "**************** Configured and built in : $(@D)"
	@echo "************************************************************************"

build_config.inc: iDSK.0.13/iDSK/src/iDSK
	(set -eu ; \
	{ \
	echo "# with bash do \"source\" this file." ; \
	cd "$(<D)" ; \
	echo "export PATH=\"\$${PATH}:$$PWD\"" ; \
	} >$@ ; )

iDSK: iDSK.0.13/iDSK/src/iDSK
	cp -avf $< $@

clean:
	-( set -e ; cd iDSK.0.13/iDSK ; make clean ; )

mrproper: clean
	-rm -rf iDSK.0.13 ._iDSK.0.13 *~

distclean: mrproper
	-rm -f iDSK iDSK.0.13-src.tgz
