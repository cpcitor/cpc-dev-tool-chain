TARGETS=build_config.inc

all: $(TARGETS)

########################################################################

# Tested release
URL_RELEASE=http://sourceforge.net/projects/z88dk/files/z88dk/1.10/z88dk-1.10.1.tgz/download

# Untested
URL_LATESTRELEASE=http://sourceforge.net/projects/z88dk/files/latest/download?source=files
URL_NIGHTLY=http://nightly.z88dk.org/z88dk-latest.tgz


URL_DOWNLOAD=$(URL_RELEASE)

z88dk-latest.tgz:
	( set -e ; wget -S $(URL_RELEASE) -O $@.tmp ; mv -vf $@.tmp $@ ; )

z88dk/.unpacked: z88dk-latest.tgz
	( set -e ; tar zxvf "$<" ; touch $@ ; )

# CVS sometimes broken for weeks. cvs up -D 2013-09-18
#z88dk/.unpacked:
#	( set -e ; cvs -z3 -d:pserver:anonymous@z88dk.cvs.sourceforge.net:/cvsroot/z88dk co -D 2013-09-18 -P z88dk ; touch $@ ; )

# nothing to patch at the moment
z88dk/.patched: z88dk/.unpacked
	( set -e ; touch $@ ; )

z88dk/.built: z88dk/.patched
	./z88dk_prerequisites_check.sh
	(set -e ; ( cd "$(@D)" ; LC_ALL=C ./build.sh ; ) ; touch $@ ; )

build_config.inc: z88dk/.built Makefile
	(set -e ; \
	{ \
	cd z88dk/ ; eval $$( egrep '(Z80_OZFILES|ZCCCFG)' build.sh ) ; \
	echo "# with bash do SOURCE this file to get a working z88dk config." ; \
	echo "export Z80_OZFILES=\"$${Z80_OZFILES}\"" ; \
	echo "export ZCCCFG=\"$${ZCCCFG}\"" ; \
	echo "export PATH=\"\$${PATH}:$$PWD/bin\"" ; \
	} >$@ ; )

clean:
	-( cd z88dk && make clean ; rm -f z88dk/.built ; )

mrproper: clean
	-rm -f build_config.inc
#-( make -C z88dk clean ; rm z88dk/.built ; )

distclean: mrproper
	-rm -rf z88dk
