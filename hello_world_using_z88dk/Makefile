SHELL=/bin/bash

SRCS := $(wildcard *.c)

TARGETS=a.dsk a.cpc a.opt

all: $(TARGETS)

test:
	@echo SRCS=$(SRCS)

../tool/z88dk/build_config.inc:
	( export LC_ALL=C ; make -C "$$(dirname "$@")" build_config.inc ; )

#%: $(SRCS) ../tool/z88dk/build_config.inc
#	( . ../tool/z88dk/build_config.inc ; zcc +cpc -lcpcfs -lmz -create-app -o $@ $< ; )

%.cpc: $(SRCS) ../tool/z88dk/build_config.inc
	( . ../tool/z88dk/build_config.inc ; zcc +cpc -lndos -create-app $< ; )

%.opt: $(SRCS) ../tool/z88dk/build_config.inc
	( . ../tool/z88dk/build_config.inc ; zcc +cpc -lndos -a $< -o $@ ; )

#%.asm: $(SRCS) ../tool/z88dk/build_config.inc
#	( . ../tool/z88dk/build_config.inc ; zcc -O0 +cpc -lndos -a $< -o $@ ; )

../tool/iDSK/iDSK:
	( export LC_ALL=C ; make -C "$$(dirname "$@")" iDSK ; )

iDSK: ../tool/iDSK/iDSK
	cp -avf $< $@

#a.bin: a.cpc
#	cp -avf $< $@

a.dsk: a.cpc iDSK Makefile
#	./iDSK $@ -n -i $< -t 1 -e 6000 -c 6000 -i a.bas -t 0 -l
	./iDSK $@ -n -i $< -t 1 -e 6000 -c 6000 -l

########################################################################

clean:
	-rm -f *.bin *.cpc *.app zcc_opt.def $(TARGETS) *.dsk *.ihx *.lnk *.lst *.map *.opt *.sym *.o *.asm *~

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
