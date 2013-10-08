SHELL=/bin/bash

DSKNAME=target.dsk

SRCS := $(wildcard *.c)

IHXS=$(patsubst %.c,%.ihx,$(SRCS))
BINS=$(patsubst %.c,%.bin,$(SRCS))

TARGETS=$(DSKNAME) $(BINS) $(OPTS)

all: $(TARGETS)

test:
	@echo SRCS=$(SRCS)
	@echo IHXS=$(IHXS)
	@echo BINS=$(BINS)

########################################################################
# Conjure up compiler
########################################################################

# pseudo-target sdcc is used to say "I need sdcc compiled in PATH."
.PHONY: sdcc
sdcc: ../tool/sdcc/build_config.inc

../tool/sdcc/build_config.inc:
	( export LC_ALL=C ; make -C "$$(dirname "$@")" build_config.inc ; )

########################################################################
# Compile
########################################################################

# FIXME change code loc project must choose it
%.ihx: %.c Makefile sdcc
	( . ../tool/sdcc/build_config.inc ; sdcc -mz80 --no-std-crt0 --code-loc 32768 $< ; )

########################################################################
# Conjure up hex2bin
########################################################################

# pseudo-target hex2bin is used to say "I need hex2bin compiled in PATH."
.PHONY: hex2bin
hex2bin: ../tool/hex2bin/build_config.inc

../tool/hex2bin/build_config.inc:
	( export LC_ALL=C ; make -C "$$(dirname "$@")" build_config.inc ; )

########################################################################
# Use hex2bin
########################################################################

%.bin: %.ihx hex2bin
	( . ../tool/hex2bin/build_config.inc ; hex2bin -p 00 $< ; )

########################################################################
# Conjure up tool to insert file in dsk image
########################################################################

../tool/iDSK/iDSK:
	( export LC_ALL=C ; make -C "$$(dirname "$@")" iDSK ; )

iDSK: ../tool/iDSK/iDSK
	cp -avf $< $@

########################################################################
# Insert file in dsk image
########################################################################

# Create a new DSK file with all binaries.
$(DSKNAME): $(BINS) iDSK Makefile
#	./iDSK $@ -n -i $< -t 1 -e 6000 -c 6000 -i a.bas -t 0 -l
#	./iDSK $@ -n -i $< -t 1 -e 6000 -c 6000 -l
# WARNING : addresses are in hex without prefix, no warning on overflow
	( ./iDSK $@.tmp -n $$( for IN in $^ ; do [[ $$IN =~ .bin ]] && echo -i $$IN ; done ; ) -e 8000 -c 8000 -t 1 && mv -vf $@.tmp $@ ; )
echo "************************************************************************"
echo "**************** Image ready in $@ "
echo "************************************************************************"
echo "**************** Fire up your favorite emulator and run from it: $(BINS)"
echo "************************************************************************"

########################################################################

clean:
	-rm -f *.lk *.noi *.rel *.asm *.ihx *.lst *.map *.sym $(TARGETS)

distclean: clean
	-rm -f iDSK

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
