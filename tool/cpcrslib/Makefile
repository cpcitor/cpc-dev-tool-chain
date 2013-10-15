TARGETLIBS=cpcrslib.lib cpcwyzlib.lib

all: cpcrslib_SDCC.installtree/.installed cpcrslib_SDCC/.patched

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

cpcrslib_SDCC/.patched: cpcrslib_SDCC/.unpacked
	@echo "************************************************************************"
	@echo "**************** Configuring samples in : $(@D)"
	@echo "************************************************************************"
	( set -e ; \
	cd $(@D) ; \
	cd examples ; \
	for EXAMPLE in */ ; \
	do \
	$(CDTC_ROOT)/new-sdcc-project.sh "$${EXAMPLE}" ; \
	( \
		echo EXENAME=$$( echo "$${EXAMPLE}" | tr -d -c 'A-Za-z0-9_'| grep -o '^........' ; ) ; \
		echo CDTC_ROOT=$(CDTC_ROOT) ; \
	) >"$${EXAMPLE}"/cdtc_project.conf ; \
	done ; \
	)
	touch $@
	@echo "************************************************************************"
	@echo "**************** Configured samples in : $(@D)"
	@echo "************************************************************************"
	@echo "**************** You can go in any sample directory (path above) and type make."
	@echo "************************************************************************"

cpcrslib_SDCC.buildtree/.built: cpcrslib_SDCC/.unpacked
	@echo "************************************************************************"
	@echo "**************** Building in : $(@D) target $(@F)"
	@echo "************************************************************************"
	( set -e ; \
	mkdir -p $(@D) ; \
	LC_ALL=C make --print-directory -f ../cpcrslib_SDCC_SDCC.Makefile -C $(@D) $(TARGET) CPCRSLIB_SRCTREE=../$(<D)/SDCC ; \
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

clean:
	-( set -e ; cd cpcrslib_SDCC.buildtree ; make clean ; )

mrproper: #clean
	-rm -rf cpcrslib_SDCC cpcrslib_SDCC.buildtree cpcrslib_SDCC.installtree ._cpcrslib_SDCC *~

distclean: mrproper
	-rm -f cpcrslib_SDCC_11.03.2012.rar
