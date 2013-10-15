SHELL=/bin/bash

DSKNAME=target.dsk

SRCS := $(wildcard *.c)

BINS=$(patsubst %.c,%.bin,$(SRCS))
OPTS=$(patsubst %.c,%.opt,$(SRCS))

TARGETS=$(DSKNAME) $(BINS) $(OPTS)

all: $(TARGETS)

test:
	@echo SRCS=$(SRCS)
	@echo BINS=$(BINS)
	@echo OPTS=$(OPTS)

########################################################################

../tool/z88dk/build_config.inc:
	( export LC_ALL=C ; make -C "$(@D)" build_config.inc ; )

#%: $(SRCS) ../tool/z88dk/build_config.inc
#	( . ../tool/z88dk/build_config.inc ; zcc +cpc -lcpcfs -lmz -create-app -o $@ $< ; )

# create-app adds a header but not if we name the file $@.bin
%.bin: $(SRCS) ../tool/z88dk/build_config.inc
	( set -o pipefail ; source ../tool/z88dk/build_config.inc ; zcc +cpc -lndos -create-app $< -o $@.cpc 2>&1 | sed 's/^sccz80:"\([^"]*\)" L:\([0-9]*\)/\1:\2:/' && mv -vf $@.cpc $@ )

# z88dk compiler output does not match traditional compiler output.
# expected:
#	myfile.c:19:
# observed:
#	sccz80:"myfile.c" L:19
# To help development environments, we adjust.
# Benefit : from the IDE you see where warnings and errors
# are, you can click to go on line to fix, etc.
# sed 's/^sccz80:"\([^"]*\)" L:\([0-9]*\)/\1:\2/'

%.opt: $(SRCS) ../tool/z88dk/build_config.inc
	( . ../tool/z88dk/build_config.inc ; zcc +cpc -lndos -a $< -o $@ ; )

#%.asm: $(SRCS) ../tool/z88dk/build_config.inc
#	( . ../tool/z88dk/build_config.inc ; zcc -O0 +cpc -lndos -a $< -o $@ ; )

########################################################################
# Conjure up tool to insert file in dsk image
########################################################################

# pseudo-target sdcc is used to say "I need sdcc compiled in PATH."
.PHONY: idsk
idsk: ../tool/idsk/build_config.inc

../tool/idsk/build_config.inc:
	( export LC_ALL=C ; make -C "$(@D)" ; )

########################################################################
# Insert file in dsk image
########################################################################

# Create a new DSK file with all binaries.
$(DSKNAME): $(BINS) idsk Makefile
#	./iDSK $@ -n -i $< -t 1 -e 6000 -c 6000 -i a.bas -t 0 -l
#	./iDSK $@ -n -i $< -t 1 -e 6000 -c 6000 -l
	( set -e ; source ../tool/idsk/build_config.inc ; iDSK $@.tmp -n $$( for IN in $^ ; do [[ $$IN =~ .bin ]] && echo -i $$IN ; done ; ) && mv -vf $@.tmp $@ ; )
# Arguments -e 6000 -c 6000 -t 1 are redundant when -create-app app is used : both create a AMSDOS header.
#	( ./iDSK $@.tmp -n $$( for IN in $^ ; do [[ $$IN =~ .bin ]] && echo -i $$IN ; done ; ) -e 6000 -c 6000 -t 1 && mv -vf $@.tmp $@ ; )
	@echo
	@echo "************************************************************************"
	@echo "************************************************************************"
	@echo "**************** Current directory is: $(PWD) "
	@echo "**************** Image ready: in $@ "
	@echo "************************************************************************"
	@echo "**************** Fire up your favorite emulator and run from it: $(BINS)"
	@echo "************************************************************************"
	@echo "************************************************************************"

########################################################################

clean:
	-rm -f *.bin *.cpc *.app zcc_opt.def $(TARGETS) *.dsk *.ihx *.lnk *.lst *.map *.opt *.sym *.o *.asm *~

distclean: clean

########################################################################


HDRS := $(wildcard *.h)
SRCS := $(wildcard *.c)
GENHRDS := $(SRCS:.c=.h)

indent:
	indent -fca -fc1 -bbb -bad -bap -sob -br -ce $(HDRS) $(SRCS)
	@echo "Modifs: "
	( for a in $(HDRS) $(SRCS) ; do echo -n $$a ;  diff -u $$a~ $$a && { echo " inchangé" ; mv -f $$a~ $$a ; } ; done ; )

# Headers générés automatiquement
%.h: %.c Makefile
	@echo -n Header pour $<: ont changé $? ...
#	@mv 2>/dev/null $@ $@.bak && echo || echo '(était absent)'
	( rm -f $@.tmp ; set -e ; grep -q may_be_overwritten $@ || { touch $@ ; exit ; } && { echo "/* This is a generated header. If you modify it, remove this line and the next to prevent overwriting. */" ; echo "/* may_be_overwritten */" ; echo "#ifndef _$(*F)_H_" ; echo "#define _$(*F)_H_" ; cproto -ec $< -s -v $(INCLUDE) ; echo "#endif /* _$(*F)_H_ */" ; } >$@.tmp ; mv -f $@.tmp $@ ; )
#	@[ -e $@.bak ] && diff -us $@.bak $@ && rm $@.bak || /bin/true

headers: $(GENHRDS)
	@echo $(GENHRDS)

dep: $(DEPS)
	@echo $(DEPS)
