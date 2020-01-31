CFLAGS=-g -Wall -Wextra $(shell pkg-config --cflags libpng)
LDFLAGS=$(shell pkg-config --libs libpng)
CC=gcc

SOURCES=$(wildcard *.c)

png2cpcsprite: $(SOURCES) Makefile
	$(CC) $(CFLAGS) $(SOURCES) -o $@ $(LDFLAGS)

#OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

#png2sprite: $(OBJECTS)
#	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	-rm -f $(OBJECTS)

indent:
	clang-format -i *.c

astyle: $(wildcard *.c */*.c *.h */*.h)
	astyle --mode=c --lineend=linux --indent=spaces=8 --style=ansi --add-brackets --indent-switches --indent-classes --indent-preprocessor --convert-tabs --break-blocks --pad-oper --pad-paren-in --pad-header --unpad-paren --align-pointer=name $^

