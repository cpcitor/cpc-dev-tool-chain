# How this is organized.

displayhelp:
	@echo "Choose target among the following:"
	@echo
	@sed -n 's/^\([a-z0-9_-]*\):.*$$/make \1/p' Makefile

.PHONY: hello_world_using_z88dk hello_world_using_sdcc hello_world_using_sdcc-all cpcrslib cpcrslib-all sdcc-all test-custom-project

hello_world_using_z88dk: hello_world_using_z88dk/Makefile
	LC_ALL=C $(MAKE) -C $@

hello_world_using_sdcc: hello_world_using_sdcc/Makefile
	LC_ALL=C $(MAKE) -C $@

hello_world_using_sdcc-all: hello_world_using_sdcc/Makefile
	LC_ALL=C $(MAKE) -C $(<D) all

cpcrslib: cpclib/cpcrslib/Makefile
	LC_ALL=C $(MAKE) -C $(<D)

cpcrslib-all: cpclib/cpcrslib/Makefile
	LC_ALL=C $(MAKE) -C $(<D) all

#distclean:
#	( find . -type d -print -exec bash -c "cd '{}' ; make clean ; make mrproper ; make distclean" \; ; )

sdcc-all: test-custom-project hello_world_using_sdcc-all cpcrslib-all

test-custom-project:
	( maintainer-tools/test_custom_project.sh ; )
