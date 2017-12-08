SHELL=/bin/bash

where-am-i = $(lastword $(MAKEFILE_LIST))
THIS_MAKEFILE := $(call where-am-i)

VARIABLES_AT_MAKEFILE_START := $(.VARIABLES)

SDCC = time sdcc

# optional include because inner projects don't have a cdtc_local_machine.conf
-include cdtc_local_machine.conf
-include local.Makefile
-include cdtc_project.conf
#PROJNAME:=$(shell date +%Hh%Mm%S )
PROJNAME?=sdccproj
LDFLAGS?=
CODELOC?=0x4000
DSKNAME?=$(PROJNAME).dsk
CDTNAME?=$(PROJNAME).cdt
VOCNAME?=$(PROJNAME).voc
AUNAME?=$(PROJNAME).au

# https://stackoverflow.com/questions/40558385/gnu-make-wildcard-no-longer-gives-sorted-output-is-there-any-control-switch
SRCS := $(sort $(wildcard *.c src/*.c platform_sdcc/*.c))
SRSS := $(sort $(wildcard *.s src/*.s platform_sdcc/*.s))

RELS=$(patsubst %.c,%.rel,$(SRCS)) $(patsubst %.s,%.rel,$(SRSS))

IHXS=$(PROJNAME).ihx
BINS=$(patsubst %.ihx,%.bin,$(IHXS))
BINAMSDOSS=$(patsubst %.bin,%.binamsdos,$(BINS))

TARGETS=$(DSKNAME) $(BINS) $(OPTS)

########################################################################
# Common targets
########################################################################

default:
	@echo "########################################################################"
	@echo "GOOD! This project will use your cpc-dev-tool-chain copy there:"
	@echo "$$PWD"
	@echo "########################################################################"
	@echo "This message won't appear if you define a \`local.Makefile\` starting with your preferred default rule."
	@echo ""
	@echo "Choose target among the following:"
	@echo ""
	@sed -n 's/^\([a-z0-9_-]*\):.*$$/make \1/p' $(THIS_MAKEFILE)
	@echo
	@echo "########################################################################"
	@echo "Exiting without performing any build. Happy hacking!"
	@echo "########################################################################"
	@exit 0


all: $(TARGETS)

bin: $(BINS)
dsk: $(DSKNAME)
cdt: $(CDTNAME)
voc: $(VOCNAME)
au: $(AUNAME)

lib: $(PROJNAME).lib
ihx: $(PROJNAME).ihx

.PHONY: default all bin dsk cdt voc au lib ihx

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
# Conjure up cfwi
########################################################################

CDTC_ENV_FOR_CFWI=$(CDTC_ROOT)/cpclib/cfwi/cfwi.lib

.PHONY: $(CDTC_ENV_FOR_CFWI)
$(CDTC_ENV_FOR_CFWI):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" ; )

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
	( SDCC_CFLAGS="$(CFLAGS_PROJECT_SDCC) $(CFLAGS_PROJECT_ALLPLATFORMS)" ; \
	if grep -E '^#include .cpc(rs|wyz)lib.h.' $< ; then echo "Uses cpcrslib and/or cpcwyzlib: $<" ; $(MAKE) $(CDTC_ENV_FOR_CPCRSLIB) ; SDCC_CFLAGS="$${SDCC_CFLAGS} -I$(CDTC_ROOT)/cpclib/cpcrslib/cpcrslib_SDCC.installtree/include" ; fi ; \
	if grep -E '^#include .cfwi/.*\.h.' $< ; then echo "Uses cfwi: $<" ; $(MAKE) $(CDTC_ENV_FOR_CFWI) ; SDCC_CFLAGS="$${SDCC_CFLAGS} -I$(abspath $(CDTC_ROOT)/cpclib/cfwi/include/)" ; fi ; \
	. "$(CDTC_ROOT)"/tool/sdcc/build_config.inc ; set -xv ; $(SDCC) -mz80 --allow-unsafe-read --std-sdcc99 $${SDCC_CFLAGS} $(CFLAGS) -c $< -o $@ ; )

%.rel: %.s Makefile $(CDTC_ENV_FOR_SDCC) cdtc_project.conf
	( . $(CDTC_ENV_FOR_SDCC) ; set -xv ; time sdasz80 -l -o -s $@ $< ; )

# If the project does "#include <stdio.h>" we link our putchar implementation. In theory someone might include stdio and prefer his own putchar implementation. If this happens to you, please tell, or even better offer a patch.

# "--data-loc 0" ensures data area is computed by linker.
$(PROJNAME).ihx: $(RELS) Makefile $(CDTC_ENV_FOR_SDCC) cdtc_project.conf
	( set -xv ; SDCC_LDFLAGS="--code-loc $$(printf 0x%x $(CODELOC)) --data-loc 0" ; \
	if [[ -n "$(SRCS)" ]] ; then \
	if grep -H '^#include .stdio.h.' $(SRCS) ; then echo "This executable depends on stdio(putchar): $@" ; $(MAKE) $(CDTC_ENV_FOR_CPC_PUTCHAR) ; SDCC_LDFLAGS="$${SDCC_LDFLAGS} $(CDTC_ROOT)/cpclib/cdtc_stdio/putchar_cpc.rel" ; fi ; \
	if grep -H '^#include .cpcrslib.h.' $(SRCS) ; then echo "This executable depends on cpcrslib: $@" ; $(MAKE) $(CDTC_ENV_FOR_CPCRSLIB) ; SDCC_LDFLAGS="$${SDCC_LDFLAGS} -l$(CDTC_ROOT)/cpclib/cpcrslib/cpcrslib_SDCC.installtree/lib/cpcrslib.lib" ; fi ; \
	if grep -H '^#include .cpcwyzlib.h.' $(SRCS) ; then echo "This executable depends on cpcwyzlib: $@" ; $(MAKE) $(CDTC_ENV_FOR_CPCRSLIB) ; SDCC_LDFLAGS="$${SDCC_LDFLAGS} -l$(CDTC_ROOT)/cpclib/cpcrslib/cpcrslib_SDCC.installtree/lib/cpcwyzlib.lib" ; fi ; \
	if grep -H '^#include .cfwi/.*\.h.' $(SRCS) ; then echo "This executable depends on cfwi: $@" ; $(MAKE) $(CDTC_ENV_FOR_CFWI) ; SDCC_LDFLAGS="$${SDCC_LDFLAGS} -l$(abspath $(CDTC_ENV_FOR_CFWI))" ; fi ; \
	fi ; \
	. $(CDTC_ENV_FOR_SDCC) ; $(SDCC) -mz80 --no-std-crt0 -Wl-u $(LDFLAGS) $(LDLIBS) $(filter crt0.rel,$^) $(filter %.rel,$(filter-out crt0.rel,$^)) $${SDCC_LDFLAGS} -o "$@" ; )

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
# Conjure up iDSK ( tool to insert file in dsk image )
########################################################################

CDTC_ENV_FOR_IDSK=$(CDTC_ROOT)/tool/idsk/build_config.inc

$(CDTC_ENV_FOR_IDSK):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" ; )

########################################################################
# Conjure up addhead
########################################################################

CDTC_ENV_FOR_ADDHEAD=$(CDTC_ROOT)/tool/addhead/build_config.inc

$(CDTC_ENV_FOR_ADDHEAD):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" build_config.inc ; )

########################################################################
# Use addhead
########################################################################

%.binamsdos.log %.binamsdos: %.bin $(CDTC_ENV_FOR_ADDHEAD)
	( set -exv ; \
	LOADADDR=$$( sed -n 's/^Lowest address  = 0000\([0-9]*\).*$$/\1/p' <$(<).log ) ; \
	RUNADDR=$$( sed -n 's/^ *0000\([0-9A-F]*\) *cpc_run_address  *.*$$/\1/p' <$*.map ) ; \
	if [[ -z "$$RUNADDR" ]] ; then \
	RUNADDR=$$( sed -n 's/^ *0000\([0-9A-F]*\) *init  *.*$$/\1/p' <$*.map ) ; \
	fi ; \
	if [[ -z "$$RUNADDR" ]] ; then \
	RUNADDR=$$( sed -n 's/^ *0000\([0-9A-F]*\) *_main  *.*$$/\1/p' <$*.map ) ; \
	fi ; \
	if [[ -z "$$RUNADDR" ]] ; then \
	echo "Cannot figure out run address. Aborting." ; exit 1 ; \
	fi ; \
	. $(CDTC_ENV_FOR_ADDHEAD) ; \
	addhead -a -t "binary" "$*.bin" "$*.binamsdos" -x '&'$${RUNADDR} -s '&'$${LOADADDR} | tee "$*.binamsdos.log" ; \
	)

%.cpcascii: %.txt
	( recode ../CRLF <$^ >$@.tmp && mv -vf $@.tmp $@ ; )

%.cpcascii: %.bas.txt
	( recode ../CRLF <$^ >$@.tmp && mv -vf $@.tmp $@ ; )

# This rule would require a CPC BASIC file in binary form without a header. Not common these days.
%.basamsdos.log %.basamsdos: %.bas $(CDTC_ENV_FOR_ADDHEAD)
	( set -exv ; \
	. $(CDTC_ENV_FOR_ADDHEAD) ; \
	addhead -a -t "basic" "$*.bas" "$*.basamsdos" | tee "$*.basamsdos.log" ; \
	)

########################################################################
# Conjure up cpcxfs ( tool to insert file in dsk image )
########################################################################

CDTC_ENV_FOR_CPCXFS=$(CDTC_ROOT)/tool/cpcxfs/build_config.inc

$(CDTC_ENV_FOR_CPCXFS):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" ; )

ifdef PREFER_IDSK_OVER_CPCXFS

########################################################################
# Insert file in dsk image using iDSK
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

else

########################################################################
# Insert file in dsk image using cpcxfs
########################################################################

# Create a new DSK file with all binaries.
$(DSKNAME): $(BINAMSDOSS) $(CDTC_ENV_FOR_CPCXFS) Makefile
	( set -exv ; \
	source $(CDTC_ENV_FOR_CPCXFS) ; \
	cpcxfs -f -nd $@.tmp -b $(patsubst %,-p %, $(filter %.binamsdos,$^)) \
	&& mv -vf $@.tmp $@ ; \
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

endif

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
# Conjure up tool to convert .cdt to .voc or .au
########################################################################

CDTC_ENV_FOR_PLAYTZX=$(CDTC_ROOT)/tool/playtzx/build_config.inc

$(CDTC_ENV_FOR_PLAYTZX):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" ; )

########################################################################
# Insert file in CDT tape image
########################################################################

%.voc: %.cdt $(CDTC_ENV_FOR_PLAYTZX)
	( . $(CDTC_ENV_FOR_PLAYTZX) ; playtzx -voc $< $@ ; )

%.au: %.cdt $(CDTC_ENV_FOR_PLAYTZX)
	( . $(CDTC_ENV_FOR_PLAYTZX) ; playtzx -au $< $@ ; )

########################################################################

clean:
	-rm -f *.lk *.noi *.rel *.asm *.ihx *.lst *.map *.sym *.rst *.bin.log *.lib *.tmp $(TARGETS)
	-rm -f src/*.lk src/*.noi src/*.rel src/*.asm src/*.ihx src/*.lst src/*.map src/*.sym src/*.rst src/*.bin.log src/*.tmp
	-rm -f *~ */*~ */*/*~
distclean: clean


########################################################################


HDRS := $(wildcard *.h platform_sdcc/*.h)
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
