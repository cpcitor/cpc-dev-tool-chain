# This is a machine-readable version of [cpcrslibSDCC - cpcrslib - How to use cpcrslib in SDCC. - CPC libraries for z88dk and SDCC - Google Project Hosting](http://code.google.com/p/cpcrslib/wiki/cpcrslibSDCC)

# It is intended to be used from SDCC subdirectory extracted from cpcrslib_SDCC_11.03.2012.rar

VPATH = $(CPCRSLIB_SRCTREE)

TARGETS=cpcrslib.lib cpcwyzlib.lib

all: $(TARGETS)

%.rel: %.s
	sdasz80 -o $@ $<

SRSS=cpcrslib.s GphStr.s Sprites.s Keyboard.s UnExoOpt.s Uncrunch.s GphStrStd.s TileMap.s Wyz.s
RELS=$(patsubst %.s,%.rel,$(SRSS))

%.lib: %.

cpcrslib.lib: $(filter-out Wyz.rel,$(RELS))
	sdar rc $@ $^

cpcwyzlib.lib: $(filter Wyz.rel,$(RELS))
	sdar rc $@ $^

clean:
	-rm -f $(RELS)

mrproper:
	-rm -f $(RELS) ($TARGETS)
