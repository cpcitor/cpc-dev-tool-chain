TARGETS=cpcrslib_SDCC.buildtree/cpcrslib.lib cpcrslib_SDCC.buildtree/cpcwyzlib.lib

all: $(TARGETS)

########################################################################

cpcrslib_SDCC_11.03.2012.rar:
	wget -S http://cpcrslib.googlecode.com/files/cpcrslib%20SDCC%2011.03.2012.rar -O $@.tmp && mv $@.tmp $@
	@echo "************************************************************************"
	@echo "**************** Source archive was downloaded to : $(@)"
	@echo "************************************************************************"

cpcrslib_SDCC/.unpacked: cpcrslib_SDCC_11.03.2012.rar
	@echo "************************************************************************"
	@echo "**************** Extracting source from : $^"
	@echo "************************************************************************"
	( set -e ; mkdir -p $(@D) ; cd $(@D) ; unrar x ../$< ; cd - ; touch $@ ; )
	@echo "************************************************************************"
	@echo "**************** Source extracted to : $(@D)"
	@echo "************************************************************************"

#cpcrslib_SDCC/.patched: cpcrslib_SDCC_build_patch.patch cpcrslib_SDCC/.unpacked
#	( set -e ; patch -p0 < $< ; touch $@ ; )

cpcrslib_SDCC.buildtree/%: cpcrslib_SDCC/.unpacked
	@echo "************************************************************************"
	@echo "**************** Building in : $(@D) target $(@F)"
	@echo "************************************************************************"
	( set -e ; mkdir -p $(@D) ; LC_ALL=C make --print-directory -f ../cpcrslib_SDCC_SDCC.Makefile -C $(@D) $(@F) CPCRSLIB_SRCTREE=../$(<D)/SDCC ; )
	@echo "************************************************************************"
	@echo "**************** Build success in : $(@D) target $(@F)"
	@echo "************************************************************************"

clean:
	-( set -e ; cd cpcrslib_SDCC.buildtree ; make clean ; )

mrproper: #clean
	-rm -rf cpcrslib_SDCC cpcrslib_SDCC.buildtree ._cpcrslib_SDCC *~

distclean: mrproper
	-rm -f cpcrslib_SDCC_11.03.2012.rar
