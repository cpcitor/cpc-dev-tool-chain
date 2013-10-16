SHELL=/bin/bash

DSKNAME=target.dsk
EXENAME=sdccproj
-include cdtc_project.conf
#EXENAME:=$(shell date +%Hh%Mm%S )

SRCS := $(wildcard *.c)
SRSS := $(wildcard *.s)

RELS=$(patsubst %.c,%.rel,$(SRCS)) $(patsubst %.s,%.rel,$(SRSS))

IHXS=$(EXENAME).ihx
BINS=$(patsubst %.ihx,%.bin,$(IHXS))

TARGETS=$(DSKNAME) $(BINS) $(OPTS)

all: $(TARGETS)

test:
	@echo EXENAME=$(EXENAME)
	@echo SRCS=$(SRCS)
	@echo SRSS=$(SRSS)
	@echo RELS=$(RELS)
	@echo IHXS=$(IHXS)
	@echo BINS=$(BINS)

$(EXENAME).ihx: $(RELS)

########################################################################
# Conjure up compiler
########################################################################

# pseudo-target sdcc is used to say "I need sdcc compiled in PATH."
.PHONY: sdcc
sdcc: $(CDTC_ROOT)/tool/sdcc/build_config.inc

$(CDTC_ROOT)/tool/sdcc/build_config.inc:
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" build_config.inc ; )

########################################################################
# Compile
########################################################################

# FIXME change code loc project must choose it
%.rel: %.c Makefile sdcc
	( SDCCARGS="" ; \
	if grep '^#include .cpcrslib.h.' $< ; then echo "Uses cpcrslib: $<" ; SDCCARGS="$${SDCCARGS} -I$(CDTC_ROOT)/tool/cpcrslib/cpcrslib_SDCC.installtree/include" ; fi ; \
	. "$(CDTC_ROOT)"/tool/sdcc/build_config.inc ; set -xv ; sdcc -mz80 $${SDCCARGS} -c $< ; )

%.rel: %.s Makefile sdcc
	( . $(CDTC_ROOT)/tool/sdcc/build_config.inc ; set -xv ; sdasz80 -l -o -s $@ $< ; )

$(EXENAME).ihx: $(RELS) Makefile sdcc
	( SDCCARGS="" ; \
	if grep -H '^#include .cpcrslib.h.' $(SRCS) ; then echo "This executable depends on cpcrslib: $@" ; SDCCARGS="$${SDCCARGS} -l$(CDTC_ROOT)/tool/cpcrslib/cpcrslib_SDCC.installtree/lib/cpcrslib.lib" ; fi ; \
	 . $(CDTC_ROOT)/tool/sdcc/build_config.inc ; sdcc -mz80 --no-std-crt0 --code-loc 32768 $${SDCCARGS} $(filter %.rel,$^) -o "$@" ; )

# For aggressive optimization add :
# --max-allocs-per-node 100000000
# --fomit-frame-pointer
# --all-callee-saves

########################################################################
# Conjure up hex2bin
########################################################################

# pseudo-target hex2bin is used to say "I need hex2bin compiled in PATH."
.PHONY: hex2bin
hex2bin: $(CDTC_ROOT)/tool/hex2bin/build_config.inc

$(CDTC_ROOT)/tool/hex2bin/build_config.inc:
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" build_config.inc ; )

########################################################################
# Use hex2bin
########################################################################

%.bin: %.ihx hex2bin
	( . $(CDTC_ROOT)/tool/hex2bin/build_config.inc ; hex2bin -p 00 "$<" ; )

########################################################################
# Conjure up tool to insert file in dsk image
########################################################################

# pseudo-target sdcc is used to say "I need sdcc compiled in PATH."
.PHONY: idsk
idsk: $(CDTC_ROOT)/tool/idsk/build_config.inc

$(CDTC_ROOT)/tool/idsk/build_config.inc:
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" ; )

########################################################################
# Insert file in dsk image
########################################################################

# Create a new DSK file with all binaries.
$(DSKNAME): $(BINS) idsk Makefile
#	./iDSK $@ -n -i $< -t 1 -e 6000 -c 6000 -i a.bas -t 0 -l
#	./iDSK $@ -n -i $< -t 1 -e 6000 -c 6000 -l
# WARNING : addresses are in hex without prefix, no warning on overflow
	( set -exv ; source $(CDTC_ROOT)/tool/idsk/build_config.inc ; iDSK $@.tmp -n $$( for IN in $(filter %.bin,$^) ; do echo -i $$IN ; done ; ) -e 8000 -c 8000 -t 1 && mv -vf $@.tmp $@ ; )
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
	-rm -f *.lk *.noi *.rel *.asm *.ihx *.lst *.map *.sym $(TARGETS)

distclean: clean


########################################################################


HDRS := $(wildcard *.h)
SRCS := $(wildcard *.c)
GENHRDS := $(SRCS:.c=.h)

indent:
	indent -fca -fc1 -bbb -bad -bap -sob -bli0 -ce -i8 $(HDRS) $(SRCS)
	@echo "Modifs: "
	( for a in $(HDRS) $(SRCS) ; do echo -n $$a ;  diff -u $$a~ $$a && { echo " inchangé" ; mv -f $$a~ $$a ; } ; done ; )

# Headers générés automatiquement
%.h: %.c Makefile
	@echo Header pour $<: ont changé $? ...
#	@mv 2>/dev/null $@ $@.bak && echo || echo '(était absent)'
	( rm -f $@.tmp ; set -e ; grep -q may_be_overwritten $@ || { touch $@ ; exit ; } && { echo "/* This is a generated header. If you modify it, remove this line and the next to prevent overwriting. */" ; echo "/* may_be_overwritten */" ; echo "#ifndef _$(*F)_H_" ; echo "#define _$(*F)_H_" ; cproto -ec $< -s -v $(INCLUDE) ; echo "#endif /* _$(*F)_H_ */" ; } >$@.tmp ; mv -f $@.tmp $@ ; )
#	@[ -e $@.bak ] && diff -us $@.bak $@ && rm $@.bak || /bin/true

headers: $(GENHRDS)
	@echo $(GENHRDS)

dep: $(DEPS)
	@echo $(DEPS)
