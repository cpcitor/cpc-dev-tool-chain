# What is this ?

This is currently an embryo of an integrated toolchain for the Amstrad CPC platform (or emulator).

## What does it do ?

* It compiles C source code into a ready-to-run disk image.

## How to use ?

* Have a Linux-like environment with dependencies (see below).
* Get a copy (very small download).
* run "make" in the directory.
* propagate dsk image to cpc/emulator and run.

## Can you give more details ?

It takes as input:

* a Linux-like environment (Windows users might install cygwin) with some dependencies (see below)
* C source code (currently, just a "Hello World")

It does for its own use:

* if needed (normally one time only), get from the internet and compile needed tools (currently z88dk and iDSK)

It produces as output:

* disk image to run on Amstrad CPC platform (or emulator)


## Needed software

Dependencies are quite ordinary for a Linux system, so they should be easy to get through your distribution's standard packaged.
Software that is specific to CPC is fetched from the Internet.

make

    sudo apt-get install make

dependencied to get and compile z88dk: cvs, gcc

    sudo apt-get install cvs gcc

## Tested platforms

* Ubuntu 11.10 (or was it 11.04 ?)
* Ubuntu 12.10
* should work on most Linux-like platforms, provided you can install the dependencies
