SHELL=/bin/bash

VARIABLES_AT_MAKEFILE_START := $(.VARIABLES)

-include cdtc_project.conf
#EXENAME:=$(shell date +%Hh%Mm%S )
EXENAME?=sdccproj
LDFLAGS?=
CODELOC?=0x4000
DSKNAME?=$(EXENAME).dsk
CDTNAME?=$(EXENAME).cdt

SRCS := $(wildcard *.c)
SRSS := $(wildcard *.s)

RELS=$(patsubst %.c,%.rel,$(SRCS)) $(patsubst %.s,%.rel,$(SRSS))

IHXS=$(EXENAME).ihx
BINS=$(patsubst %.ihx,%.bin,$(IHXS))

TARGETS=$(DSKNAME) $(BINS) $(OPTS)

########################################################################
# Common targets
########################################################################

default: $(BINS)

all: $(TARGETS)

bin: $(BINS)
dsk: $(DSKNAME)
cdt: $(CDTNAME)

ihx: $(EXENAME).ihx

########################################################################
# Conjure up compiler
########################################################################

SDCC_ACCESS=$(CDTC_ROOT)/tool/sdcc/build_config.inc

$(SDCC_ACCESS):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" build_config.inc ; )

########################################################################
# Compile
########################################################################

# FIXME change code loc project must choose it
%.rel: %.c Makefile $(SDCC_ACCESS)
	( SDCCARGS="" ; \
	if grep -E '^#include .cpc(rs|wyz)lib.h.' $< ; then echo "Uses cpcrslib and/or cpcwyzlib: $<" ; SDCCARGS="$${SDCCARGS} -I$(CDTC_ROOT)/tool/cpcrslib/cpcrslib_SDCC.installtree/include" ; fi ; \
	. "$(CDTC_ROOT)"/tool/sdcc/build_config.inc ; set -xv ; sdcc -mz80 $${SDCCARGS} -c $< ; )

%.rel: %.s Makefile $(SDCC_ACCESS)
	( . $(SDCC_ACCESS) ; set -xv ; sdasz80 -l -o -s $@ $< ; )

# "--data-loc 0" ensures data area is computed by linker.
$(EXENAME).ihx: $(RELS) Makefile $(SDCC_ACCESS)
	( set -xv ; SDCCARGS="--code-loc $$(printf 0x%x $(CODELOC)) --data-loc 0" ; \
	if grep -H '^#include .cpcrslib.h.' $(SRCS) ; then echo "This executable depends on cpcrslib: $@" ; SDCCARGS="$${SDCCARGS} -l$(CDTC_ROOT)/tool/cpcrslib/cpcrslib_SDCC.installtree/lib/cpcrslib.lib" ; fi ; \
	if grep -H '^#include .cpcwyzlib.h.' $(SRCS) ; then echo "This executable depends on cpcwyzlib: $@" ; SDCCARGS="$${SDCCARGS} -l$(CDTC_ROOT)/tool/cpcrslib/cpcrslib_SDCC.installtree/lib/cpcwyzlib.lib" ; fi ; \
	 . $(SDCC_ACCESS) ; sdcc -mz80 --no-std-crt0 -Wl-u $(filter %.rel,$^) $${SDCCARGS} $(LDFLAGS) -o "$@" ; )

# For aggressive optimization add :
# --max-allocs-per-node 100000000
# --fomit-frame-pointer
# --all-callee-saves

########################################################################
# Conjure up hex2bin
########################################################################

HEX2BIN_ACCESS=$(CDTC_ROOT)/tool/hex2bin/build_config.inc

$(HEX2BIN_ACCESS):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" build_config.inc ; )

########################################################################
# Use hex2bin
########################################################################

%.bin.log %.bin: %.ihx $(HEX2BIN_ACCESS)
	( . $(HEX2BIN_ACCESS) ; hex2bin -e "bin" -p 00 "$*.ihx" | tee "$*.bin.log" ; )

########################################################################
# Conjure up tool to insert file in dsk image
########################################################################

IDSK_ACCESS=$(CDTC_ROOT)/tool/idsk/build_config.inc

$(IDSK_ACCESS):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" ; )

########################################################################
# Insert file in dsk image
########################################################################

# Create a new DSK file with all binaries.
# FIXME supports only one binary.
$(DSKNAME): $(BINS) $(IDSK_ACCESS) Makefile
#	./iDSK $@ -n -i $< -t 1 -e 6000 -c 6000 -i a.bas -t 0 -l
#	./iDSK $@ -n -i $< -t 1 -e 6000 -c 6000 -l
# WARNING : addresses are in hex without prefix, no warning on overflow
	( set -exv ; \
	LOADADDR=$$( sed -n 's/^Lowest address  = 0000\([0-9]*\).*$$/\1/p' <$(<).log ) ; \
	RUNADDR=$$( sed -n 's/^ *0000\([0-9A-F]*\) *cpc_run_address  *.*$$/\1/p' <$(<:.bin=.map) ) ; \
	if [[ -z "$$RUNADDR" ]] ; then \
	RUNADDR=$$( sed -n 's/^ *0000\([0-9A-F]*\) *_main  *.*$$/\1/p' <$(<:.bin=.map) ) ; \
	fi ; \
	if [[ -z "$$RUNADDR" ]] ; then \
	echo "Cannot figure out run address. Aborting." ; exit 1 ; \
	fi ; \
	source $(IDSK_ACCESS) ; \
	iDSK $@.tmp -n $(patsubst %,-i %, $(filter %.bin,$^)) -e $${RUNADDR} -c $${LOADADDR} -t 1 && mv -vf $@.tmp $@ ; \
	)
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
# Conjure up tool to insert file in CDT tape image
########################################################################

# pseudo-target 2cdt is used to say "I need 2cdt compiled in PATH."
2CDT_ACCESS=$(CDTC_ROOT)/tool/2cdt/build_config.inc

$(2CDT_ACCESS):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" ; )

########################################################################
# Insert file in CDT tape image
########################################################################

# FIXME DRY LOADADDR
# FIXME support only one bin
$(CDTNAME): $(BINS) 2cdt Makefile
	( set -exv ; \
	LOADADDR=$$( sed -n 's/^Lowest address  = 0000\([0-9]*\).*$$/\1/p' <$(<).log ) ; \
	RUNADDR=$$( sed -n 's/^ *0000\([0-9A-F]*\) *cpc_run_address  *.*$$/\1/p' <$(<:.bin=.map) ) ; \
	if [[ -z "$$RUNADDR" ]] ; then \
	RUNADDR=$$( sed -n 's/^ *0000\([0-9A-F]*\) *_main  *.*$$/\1/p' <$(<:.bin=.map) ) ; \
	fi ; \
	if [[ -z "$$RUNADDR" ]] ; then \
	echo "Cannot figure out run address. Aborting." ; exit 1 ; \
	fi ; \
	source $(2CDT_ACCESS) ; \
	2cdt -n -X 0x$${RUNADDR} -L 0x$${LOADADDR} -r $(EXENAME) $< $@ ; \
	)
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

########################################################################
# Debug the makefile
########################################################################
$(foreach v,                                        \
  $(filter-out $(VARIABLES_AT_MAKEFILE_START) VARIABLES_AT_MAKEFILE_START,$(.VARIABLES)), \
  $(info $(v) = $($(v))))
