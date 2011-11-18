# How this is organized.

# toolchain : brings 3rd party libraries and tools.
# others : projects

# Makefile in project knows its dependencies.
#all: findyway/findyway.dsk



all: hello_world_using_z88dk/Makefile
	LC_ALL=C make -C hello_world_using_z88dk

#distclean:
#	( find . -type d -print -exec bash -c "cd '{}' ; make clean ; make mrproper ; make distclean" \; ; )
