TARGETS=build_config.inc

all: $(TARGETS)

########################################################################

z88dk/.unpacked:
	( set -e ; if [ -e z88dk_clean_checkout_20111118.tar.gz ] ; then tar zxvf z88dk_clean_checkout_20111118.tar.gz ; else cvs -z3 -d:pserver:anonymous@z88dk.cvs.sourceforge.net:/cvsroot/z88dk co -P z88dk ; fi ; touch $@ ; )

z88dk/.patched: z88dk/.unpacked
	( set -e ; touch $@ ; )

z88dk/.built: z88dk/.patched
	(set -e ; ( cd $$(dirname "$@") ; LC_ALL=C ./build.sh ; ) ; touch $@ ; )

build_config.inc: z88dk/.built Makefile
	(set -e ; \
	{ \
	cd z88dk/ ; eval $$( egrep '(Z80_OZFILES|ZCCCFG)' build.sh ) ; \
	echo "# with bash do SOURCE this file to get a working z88dk config." ; \
	echo "export Z80_OZFILES=\"$${Z80_OZFILES}\"" ; \
	echo "export ZCCCFG=\"$${ZCCCFG}\"" ; \
	echo "export PATH=\"\$${PATH}:$$PWD/bin\"" ; \
	} >$@ ; )

#iDSK: z88dk/iDSK/src/iDSK
#	cp -avf $< $@

clean:
	-( cd z88dk && make clean ; rm -f z88dk/.built ; )

mrproper: clean
	-rm -f build_config.inc
#-( make -C z88dk clean ; rm z88dk/.built ; )

distclean: mrproper
	-rm -rf z88dk
