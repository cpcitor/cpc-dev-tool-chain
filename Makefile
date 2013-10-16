# How this is organized.

# toolchain : brings 3rd party libraries and tools.
# others : projects

# Makefile in project knows its dependencies.
#all: findyway/findyway.dsk

displayhelp:
	@echo "Choose target:"
	@sed -n 's/^\([a-z0-9_-]*\):.*$$/make \1/p' Makefile

.PHONY: hello_world_using_z88dk hello_world_using_sdcc

hello_world_using_z88dk: hello_world_using_z88dk/Makefile
	LC_ALL=C $(MAKE) -C $@

hello_world_using_sdcc: hello_world_using_sdcc/Makefile
	LC_ALL=C $(MAKE) -C $@

cpcrslib: tool/cpcrslib/Makefile
	LC_ALL=C $(MAKE) -C $(<D)

#distclean:
#	( find . -type d -print -exec bash -c "cd '{}' ; make clean ; make mrproper ; make distclean" \; ; )
