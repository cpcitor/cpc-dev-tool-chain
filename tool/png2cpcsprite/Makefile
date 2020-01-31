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

