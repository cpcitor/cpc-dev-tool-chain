SHELL=/bin/bash

VARIABLES_AT_MAKEFILE_START := $(.VARIABLES)

-include cdtc_project.conf
#PROJNAME:=$(shell date +%Hh%Mm%S )
PROJNAME?=sdccproj
LDFLAGS?=
CODELOC?=0x4000
DSKNAME?=$(PROJNAME).dsk
CDTNAME?=$(PROJNAME).cdt
VOCNAME?=$(PROJNAME).voc

SRCS := $(wildcard *.c src/*.c)
SRSS := $(wildcard *.s src/*.s)

RELS=$(patsubst %.c,%.rel,$(SRCS)) $(patsubst %.s,%.rel,$(SRSS))

IHXS=$(PROJNAME).ihx
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
voc: $(VOCNAME)

lib: $(PROJNAME).lib
ihx: $(PROJNAME).ihx

########################################################################
# Conjure up cpc-specific putchar
########################################################################

CDTC_ENV_FOR_CPC_PUTCHAR=$(CDTC_ROOT)/cpclib/cdtc_stdio/cdtc_stdio.lib $(CDTC_ROOT)/cpclib/cdtc_stdio/putchar_cpc.rel

$(CDTC_ENV_FOR_CPC_PUTCHAR):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" lib putchar_cpc.rel ; )

########################################################################
# Conjure up cpcrslib
########################################################################

CDTC_ENV_FOR_CPCRSLIB=$(CDTC_ROOT)/cpclib/cpcrslib/cpcrslib_SDCC.installtree/.installed

$(CDTC_ENV_FOR_CPCRSLIB):
	( export LC_ALL=C ; $(MAKE) -C "$(dir $(@D))" ; )

########################################################################
# Conjure up cfw
########################################################################

CDTC_ENV_FOR_CFW=$(CDTC_ROOT)/cpclib/c_on_cpcfirmware/cfw.lib

$(CDTC_ENV_FOR_CFW):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" lib ; )

########################################################################
# Conjure up compiler
########################################################################

CDTC_ENV_FOR_SDCC=$(CDTC_ROOT)/tool/sdcc/build_config.inc

$(CDTC_ENV_FOR_SDCC):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" build_config.inc ; )

########################################################################
# Compile
########################################################################

# FIXME change code loc project must choose it
%.rel: %.c Makefile $(CDTC_ENV_FOR_SDCC) cdtc_project.conf
	( SDCCARGS="" ; \
	if grep -E '^#include .cpc(rs|wyz)lib.h.' $< ; then echo "Uses cpcrslib and/or cpcwyzlib: $<" ; $(MAKE) $(CDTC_ENV_FOR_CPCRSLIB) ; SDCCARGS="$${SDCCARGS} -I$(CDTC_ROOT)/cpclib/cpcrslib/cpcrslib_SDCC.installtree/include" ; fi ; \
	if grep -E '^#include .amsgraph.h.' $< ; then echo "Uses cfw: $<" ; $(MAKE) $(CDTC_ENV_FOR_CFW) ; SDCCARGS="$${SDCCARGS} -I$(CDTC_ROOT)/cpclib/c_on_cpcfirmware" ; fi ; \
	. "$(CDTC_ROOT)"/tool/sdcc/build_config.inc ; set -xv ; sdcc -mz80 $${SDCCARGS} $(CFLAGS) -c $< ; )

%.rel: %.s Makefile $(CDTC_ENV_FOR_SDCC) cdtc_project.conf
	( . $(CDTC_ENV_FOR_SDCC) ; set -xv ; sdasz80 -l -o -s $@ $< ; )

# If the project does "#include <stdio.h>" we link our putchar implementation. In theory someone might include stdio and prefer his own putchar implementation. If this happens to you, please tell, or even better offer a patch.

# "--data-loc 0" ensures data area is computed by linker.
$(PROJNAME).ihx: $(RELS) Makefile $(CDTC_ENV_FOR_SDCC) cdtc_project.conf
	( set -xv ; SDCCARGS="--code-loc $$(printf 0x%x $(CODELOC)) --data-loc 0" ; \
	if [[ -n "$(SRCS)" ]] ; then \
	if grep -H '^#include .stdio.h.' $(SRCS) ; then echo "This executable depends on stdio(putchar): $@" ; $(MAKE) $(CDTC_ENV_FOR_CPC_PUTCHAR) ; SDCCARGS="$${SDCCARGS} $(CDTC_ROOT)/cpclib/cdtc_stdio/putchar_cpc.rel" ; fi ; \
	if grep -H '^#include .cpcrslib.h.' $(SRCS) ; then echo "This executable depends on cpcrslib: $@" ; $(MAKE) $(CDTC_ENV_FOR_CPCRSLIB) ; SDCCARGS="$${SDCCARGS} -l$(CDTC_ROOT)/cpclib/cpcrslib/cpcrslib_SDCC.installtree/lib/cpcrslib.lib" ; fi ; \
	if grep -H '^#include .cpcwyzlib.h.' $(SRCS) ; then echo "This executable depends on cpcwyzlib: $@" ; $(MAKE) $(CDTC_ENV_FOR_CPCRSLIB) ; SDCCARGS="$${SDCCARGS} -l$(CDTC_ROOT)/cpclib/cpcrslib/cpcrslib_SDCC.installtree/lib/cpcwyzlib.lib" ; fi ; \
	if grep -H '^#include .amsgraph.h.' $(SRCS) ; then echo "This executable depends on cfw: $@" ; $(MAKE) $(CDTC_ENV_FOR_CFW) ; SDCCARGS="$${SDCCARGS} -l$(abspath $(CDTC_ENV_FOR_CFW))" ; fi ; \
	fi ; \
	. $(CDTC_ENV_FOR_SDCC) ; sdcc -mz80 --no-std-crt0 -Wl-u $(LDFLAGS) $(filter crt0.rel,$^) $(filter %.rel,$(filter-out crt0.rel,$^)) $${SDCCARGS} -o "$@" ; )

$(PROJNAME).lib: $(RELS) Makefile $(CDTC_ENV_FOR_SDCC) cdtc_project.conf
	 ( . $(CDTC_ENV_FOR_SDCC) ; set -euxv ; sdar rc "$@" $(filter %.rel,$^) ; )

# For aggressive optimization add :
# --max-allocs-per-node 100000000
# --fomit-frame-pointer
# --all-callee-saves

########################################################################
# Conjure up hex2bin
########################################################################

CDTC_ENV_FOR_HEX2BIN=$(CDTC_ROOT)/tool/hex2bin/build_config.inc

$(CDTC_ENV_FOR_HEX2BIN):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" build_config.inc ; )

########################################################################
# Use hex2bin
########################################################################

%.bin.log %.bin: %.ihx $(CDTC_ENV_FOR_HEX2BIN)
	( . $(CDTC_ENV_FOR_HEX2BIN) ; hex2bin -e "bin" -p 00 "$*.ihx" | tee "$*.bin.log" ; )

########################################################################
# Conjure up tool to insert file in dsk image
########################################################################

CDTC_ENV_FOR_IDSK=$(CDTC_ROOT)/tool/idsk/build_config.inc

$(CDTC_ENV_FOR_IDSK):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" ; )

########################################################################
# Insert file in dsk image
########################################################################

# Create a new DSK file with all binaries.
# FIXME supports only one binary.
$(DSKNAME): $(BINS) $(CDTC_ENV_FOR_IDSK) Makefile
#	./iDSK $@ -n -i $< -t 1 -e 6000 -c 6000 -i a.bas -t 0 -l
#	./iDSK $@ -n -i $< -t 1 -e 6000 -c 6000 -l
# WARNING : addresses are in hex without prefix, no warning on overflow
	( set -exv ; \
	LOADADDR=$$( sed -n 's/^Lowest address  = 0000\([0-9]*\).*$$/\1/p' <$(<).log ) ; \
	RUNADDR=$$( sed -n 's/^ *0000\([0-9A-F]*\) *cpc_run_address  *.*$$/\1/p' <$(<:.bin=.map) ) ; \
	if [[ -z "$$RUNADDR" ]] ; then \
	RUNADDR=$$( sed -n 's/^ *0000\([0-9A-F]*\) *init  *.*$$/\1/p' <$(<:.bin=.map) ) ; \
	fi ; \
	if [[ -z "$$RUNADDR" ]] ; then \
	RUNADDR=$$( sed -n 's/^ *0000\([0-9A-F]*\) *_main  *.*$$/\1/p' <$(<:.bin=.map) ) ; \
	fi ; \
	if [[ -z "$$RUNADDR" ]] ; then \
	echo "Cannot figure out run address. Aborting." ; exit 1 ; \
	fi ; \
	source $(CDTC_ENV_FOR_IDSK) ; \
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

CDTC_ENV_FOR_2CDT=$(CDTC_ROOT)/tool/2cdt/build_config.inc

$(CDTC_ENV_FOR_2CDT):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" ; )

########################################################################
# Insert file in CDT tape image
########################################################################

# FIXME DRY LOADADDR
# FIXME support only one bin
$(CDTNAME): $(BINS) $(CDTC_ENV_FOR_2CDT) Makefile
	( set -exv ; \
	LOADADDR=$$( sed -n 's/^Lowest address  = 0000\([0-9]*\).*$$/\1/p' <$(<).log ) ; \
	RUNADDR=$$( sed -n 's/^ *0000\([0-9A-F]*\) *cpc_run_address  *.*$$/\1/p' <$(<:.bin=.map) ) ; \
	if [[ -z "$$RUNADDR" ]] ; then \
	RUNADDR=$$( sed -n 's/^ *0000\([0-9A-F]*\) *_main  *.*$$/\1/p' <$(<:.bin=.map) ) ; \
	fi ; \
	if [[ -z "$$RUNADDR" ]] ; then \
	echo "Cannot figure out run address. Aborting." ; exit 1 ; \
	fi ; \
	source $(CDTC_ENV_FOR_2CDT) ; \
	2cdt -n -X 0x$${RUNADDR} -L 0x$${LOADADDR} -r $(PROJNAME) $< $@ ; \
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
# Conjure up tool to convert CDT to voc
########################################################################

CDTC_ENV_FOR_PLAYTZX=$(CDTC_ROOT)/tool/playtzx/build_config.inc

$(CDTC_ENV_FOR_PLAYTZX):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" ; )

########################################################################
# Insert file in CDT tape image
########################################################################

$(VOCNAME): $(CDTNAME)
	( . $(CDTC_ENV_FOR_PLAYTZX) ; playtzx -voc $< $@ ; )

########################################################################

clean:
	-rm -f *.lk *.noi *.rel *.asm *.ihx *.lst *.map *.sym *.rst *.bin.log *.lib $(TARGETS)
	-rm -f src/*.lk src/*.noi src/*.rel src/*.asm src/*.ihx src/*.lst src/*.map src/*.sym src/*.rst src/*.bin.log

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
  $(sort $(filter-out $(VARIABLES_AT_MAKEFILE_START) VARIABLES_AT_MAKEFILE_START,$(.VARIABLES))), \
  $(info $(v) = $($(v))))
