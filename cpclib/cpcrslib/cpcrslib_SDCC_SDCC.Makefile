# This is a machine-readable version of [cpcrslibSDCC - cpcrslib - How to use cpcrslib in SDCC. - CPC libraries for z88dk and SDCC - Google Project Hosting](http://code.google.com/p/cpcrslib/wiki/cpcrslibSDCC)

# It is intended to be used from SDCC subdirectory extracted from cpcrslib_SDCC_11.03.2012.rar

VPATH = $(CPCRSLIB_SRCTREE)

TARGETS=cpcrslib.lib cpcwyzlib.lib

.PHONY : all
all: $(TARGETS)

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

%.rel: %.s sdcc
	( . "$(CDTC_ROOT)"/tool/sdcc/build_config.inc ; set -xv ; sdasz80 -o $@ $< ; )

SRSS=cpcrslib.s GphStr.s Sprites.s Keyboard.s UnExoOpt.s Uncrunch.s GphStrStd.s TileMap.s Wyz.s
RELS=$(patsubst %.s,%.rel,$(SRSS))

cpcrslib.lib: $(filter-out Wyz.rel,$(RELS)) sdcc
	( . "$(CDTC_ROOT)"/tool/sdcc/build_config.inc ; set -xv ; sdar rc $@ $(filter %.rel,$^) ; )

cpcwyzlib.lib: $(filter Wyz.rel,$(RELS)) sdcc
	( . "$(CDTC_ROOT)"/tool/sdcc/build_config.inc ; set -xv ; sdar rc $@ $(filter %.rel,$^) ; )

clean:
	-rm -f $(RELS)

mrproper:
	-rm -f $(RELS) ($TARGETS)
