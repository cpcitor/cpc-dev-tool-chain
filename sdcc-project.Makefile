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

RELSS=$(patsubst %.s,%.rel,$(SRSS))
RELSC=$(patsubst %.c,%.rel,$(SRCS))
RELS=$(RELSS) $(RELSC)

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
	@echo "$(CDTC_ROOT)"
	@echo "Absolute path: $$( cd $(CDTC_ROOT) ; pwd )"
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

# /usr/include/freetype2/ft2build.h
# /usr/include/SDL/SDL.h
TOOLS=git:git wget:wget make:make patch:patch gcc:gcc bzip2:bzip2 unzip:unzip g++:g++ makeinfo:texinfo bison:bison flex:flex /usr/include/boost/version.hpp:libboost-all-dev sdl-config:libsdl1.2-dev pkgconf:pkgconf /usr/lib/x86_64-linux-gnu/pkgconfig/freetype2.pc:libfreetype6-dev ncurses5-config:libncurses-dev
.build_dependencies_checked:
	MISSING="" ; for TOOLNP in $(TOOLS) ; do IFS=: read FNAME PNAME <<< "$$TOOLNP" ; echo -ne "Checking for $$FNAME \011of $$PNAME...  \0011" ; { which $$FNAME || stat --format="%n" $$FNAME ; } || { MISSING="$$MISSING $$TOOLNP" ; echo "not found" ; } ; done ; if [[ -n "$$MISSING" ]] ; then echo ; echo ; echo "################################" ; echo "Some tools are missing: " ; echo "$$MISSING " | sed 's/:[^:]* / /g' ; echo ; echo "Suggested action:" ; echo ; echo -n "sudo apt-get install " ; echo " $$MISSING " | sed 's/ [^:]*:/ /g' ; echo ; echo "Or the equivalent for your environment (yum, cygwin, etc)." ; echo ; echo "*** If you have installed things manually and believe the build can go on without all tools, you can override this check with the command line below: ***" ; echo "More information on https://github.com/cpcitor/cpc-dev-tool-chain/blob/master/documentation/how_to_install.md#user-content-do-i-need-to-prepare-my-system-" ; echo "touch $$PWD/$@" ; echo ; echo "################################" ; else echo "######## All tools found. ########" ; touch $@ ; fi

bin: .build_dependencies_checked $(BINS)
dsk: .build_dependencies_checked $(DSKNAME)
cdt: .build_dependencies_checked $(CDTNAME)
voc: .build_dependencies_checked $(VOCNAME)
au: .build_dependencies_checked $(AUNAME)

lib: .build_dependencies_checked $(PROJNAME).lib
ihx: .build_dependencies_checked $(PROJNAME).ihx

.PHONY: default all bin dsk cdt voc au lib ihx run

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
# Generating any %.rel from a %.c needs to first compile all the %.s because %.c might depend on any of the generated symbol exported from ASM.
%.rel: %.c Makefile $(CDTC_ENV_FOR_SDCC) cdtc_project.conf $(RELSS)
	( SDCC_CFLAGS="$(CFLAGS_PROJECT_SDCC) $(CFLAGS_PROJECT_ALLPLATFORMS)" ; \
	if grep -E '^#include .cpc(rs|wyz)lib.h.' $< ; then echo "Uses cpcrslib and/or cpcwyzlib: $<" ; $(MAKE) $(CDTC_ENV_FOR_CPCRSLIB) ; SDCC_CFLAGS="$${SDCC_CFLAGS} -I$(CDTC_ROOT)/cpclib/cpcrslib/cpcrslib_SDCC.installtree/include" ; fi ; \
	if grep -E '^#include .cfwi/.*\.h.' $< ; then echo "Uses cfwi: $<" ; $(MAKE) $(CDTC_ENV_FOR_CFWI) ; SDCC_CFLAGS="$${SDCC_CFLAGS} -I$(abspath $(CDTC_ROOT)/cpclib/cfwi/include/)" ; fi ; \
	. "$(CDTC_ROOT)"/tool/sdcc/build_config.inc ; set -xv ; $(SDCC) -mz80 --allow-unsafe-read $${SDCC_CFLAGS} $(CFLAGS) -c $< -o $@ ; )

%.generated_from_asm_exported_symbols.h %.rel: %.s Makefile $(CDTC_ENV_FOR_SDCC) cdtc_project.conf
	( . $(CDTC_ENV_FOR_SDCC) ; \
	set -eu ; \
	RELFILE="$(patsubst %.s,%.rel,$<)" ; \
	OUTFILE="$(patsubst %.s,%.generated_from_asm_exported_symbols.h,$<)" ; \
	if time sdasz80 -w -l -o -s "$$RELFILE" $< \
	&& { \
	echo "#include <stdint.h>" ; \
	echo ; \
	echo "// This file is generated from assembler's relocatable output:" ; \
	echo "// $$RELFILE" ; \
	echo ; \
	echo "// As you may know, in assembly source code you can:" ; \
	echo "// (1) define named constants," ; \
	echo "// (2) even when defined in reference to other constants," ; \
	echo "// (3) use them as parameters to macro calls," ; \
	echo "// (4) export them to C as... ahem, void pointers, not true constants." ; \
	echo ; \
	echo "// This file generated by cpc-dev-tool-chain allows you, " ; \
	echo "// *without any cost on the target*, to get" ; \
	echo "// C-level #define constants *guaranteed to be in sync with ASM*, " ; \
	echo "// will full compile-time optimization and tricks available." ; \
	echo ; \
	echo "// Basically, ASM side do this:" ; \
	echo "// my_variable == ( 42 >> 2 ) / 3" ; \
	echo "// my_other_variable == ( my_variable + 6128 ) / 464" ; \
	echo "// And in you C source file do this:" ; \
	echo "// #include \"$$OUTFILE\"" ; \
	echo "// Your C code can then do:" ; \
	echo "// uint8_t table[ASMCONST_my_variable][ASMCONST_my_other_variable];" ; \
	echo ; \
	echo "// Notice, we only generate #define here because they consume no target memory compiler would allocate " ; \
	echo "// Notice, typed variables would have consumed target memory, would not have been good." ; \
	echo ; \
	echo "// DO NOT EDIT this file. Instead, edit upstream source of truth: " ; \
	echo "// $<" ; \
	echo ; \
	sed -n 's|^S \([^\. ][^ ]*\) Def\([0-9A-F][0-9A-F][0-9A-F][0-9A-F]\)$$|\1 \2|p' <"$${RELFILE}" \
	| while read LABEL ADDRESS ; do grep -q "^[[:space:]]*$$LABEL[[:space:]]*==" "$<" && echo "#define ASMCONST_$${LABEL} 0x$${ADDRESS}" ; done ; true ; \
	} >"$${OUTFILE}.tmp" \
	&& grep -q "^#define ASMCONST_" "$${OUTFILE}.tmp" ; \
	then mv -f "$${OUTFILE}.tmp" "$${OUTFILE}" ; else rm -f "$${OUTFILE}.tmp" ; fi \
	)

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
	-rm -f *.lk *.noi *.rel *.asm *.ihx *.lst *.map *.sym *.rst *.bin.log *.lib *.tmp *.binamsdos *.binamsdos.log $(TARGETS)
	-rm -f */*.lk */*.noi */*.rel */*.asm */*.ihx */*.lst */*.map */*.sym */*.rst */*.bin.log */*.tmp
	-rm -f */*/*.lk */*/*.noi */*/*.rel */*/*.asm */*/*.ihx */*/*.lst */*/*.map */*/*.sym */*/*.rst */*/*.bin.log */*/*.tmp
	-rm -f *~ */*~ */*/*~
	-rm -f *.generated_from_asm_exported_symbols.h */*.generated_from_asm_exported_symbols.h
distclean: clean

########################################################################
# Run emulator
########################################################################

CDTC_ENV_FOR_CAPRICE32=$(CDTC_ROOT)/tool/caprice32/build_config.inc

$(CDTC_ENV_FOR_CAPRICE32):
	( export LC_ALL=C ; $(MAKE) -C "$(@D)" build_config.inc ; )

run: $(DSKNAME) $(CDTC_ENV_FOR_CAPRICE32)
	( . $(CDTC_ENV_FOR_CAPRICE32) ; cap32_once $(DSKNAME) -a 'run"$(PROJNAME)' ; )


########################################################################


HDRS := $(wildcard *.h platform_sdcc/*.h)
GENHRDS := $(SRCS:.c=.h)

indent:
	indent -fca -fc1 -bbb -bad -bap -sob -ce -i8 -bli0 -bls -blf $(HDRS) $(SRCS)
	@echo "Modifs: "
	( for a in $(HDRS) $(SRCS) ; do echo -n $$a ;  diff -u $$a~ $$a && { echo " inchangé" ; mv -f $$a~ $$a ; } ; done ; exit 0 ; )

astyle:
	astyle --mode=c --lineend=linux --indent=spaces=8 --style=ansi --add-brackets --indent-switches --indent-classes --indent-preprocessor --convert-tabs --break-blocks --pad-oper --pad-paren-in --pad-header --unpad-paren --align-pointer=name $(HDRS) $(SRCS)
	@echo "Modifs: "
	( for a in $(HDRS) $(SRCS) ; do echo -n $$a ;  diff -u $$a~ $$a && { echo " inchangé" ; mv -f $$a~ $$a ; } ; done ; exit 0 ; )

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
$(info ######################################################################## )
$(info Some variables about the project )
$(info ######################################################################## )
$(foreach v,                                        \
  $(sort $(filter-out $(VARIABLES_AT_MAKEFILE_START) VARIABLES_AT_MAKEFILE_START,$(.VARIABLES))), \
  $(info $(v) = $($(v))))

cppcheck:
	( shopt -s nullglob ; cppcheck --force -UDEBUG -I . -I cpc-dev-tool-chain/cpclib/cfwi/include -I cpc-dev-tool-chain/tool/sdcc/sdcc-*.installtree/share/sdcc/include/ -I platform_sdcc/ --quiet --enable=all --platform=unspecified --std=c89 *.c *.h platform_sdcc/*.c platform_sdcc/*.h ; )
