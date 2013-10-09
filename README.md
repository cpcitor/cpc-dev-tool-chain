cpc-dev-tool-chain
==================

----------------------------------------------------------------
## Summary for the impatient

### What is this ?

This is a portable toolchain for C/ASM development targetting the Amstrad CPC platform.

### What does it do ?

* It takes C and/or assembly source code and use your modern computer to automatically produces a **ready-to-run disk image** for an Amstrad CPC or emulator.
* You can edit source and get another image, at will.
* Eventually it will allow you to edit your graphics/music/whatever in your preferred editor, save, get another image, run.

### Why would I want to use that ?

* If you know to program in C and want to try that for the CPC without first learning all details of all tools.
* Or if you're an advanced hacker who like an open starting point to build upon (in that case you'll feel at home with the `Makefile`-based internals).

### Why would I *not* want to use that ?

* If you don't want to know that virtually all Integrated Development
  Environments are indeed graphical front-ends for a toolchain.
* If anything looking like a command line or an open-source style
  environment makes you sick.  In that case, it might be interesting
  for you to read Wikipedia's article about
  [Hacker (programmer subculture)](http://en.wikipedia.org/wiki/Hacker_%28programmer_subculture%29)
  and ponder whether some kind of structure or common tools helps
  every member of a community.

----------------------------------------------------------------

## Details

### Why this tool ?

#### Programming in C for the CPC is not very easy and documented

* There are **two** main **compilers** with different way of doing things.
* **Several tools** have to be installed (compiler, binary converter, disk image manager).
* There are several **output formats** (regular file on disk, cpm replacement boot sector, ROM, tape, cartridge for CPC+).
* Manualy setups are possible following several tutorials
 * often **specific to a development platform** (Windows only or Linux only)
 * asking to **manually install** in specific directories,
 * asking to **change system settings**
* To add to the confusion, there are **differing assembler syntaxes**.
* Sometimes people want to help by sharing a few binary files they made, typically for Windows only, but that won't benefit from progress in tools like sdcc.
* When programming or integrating resources (music, graphics), most people **manually re-run** various tools, which is **error-prone**.
* **It's not obvious how to use all those tools.**

#### A toolchain provides an end-to-end solution

* When programming or integrating resources (music, graphics), a toolchain knows what tool to run when, with parameters. **Clean builds, no more forgotten step.**
* The development cycle is **simplified**:
 * edit (even graphics, music, and other resources etc)
 * just build (nothing to think about)
 * run.

#### cpc-dev-tool-chain tries to ease installation and use

cpc-dev-tool-chain tries to follow the best practices common with current operating systems.

##### Installation style

* **No manual** file copy.
* **No reboot**/logout/login.
* **No global** system modification.
* Just get a few common programs, run the toolchain, it will fetch CPC-specific software from the internet and compile it for your platform while you have a break.

You can even have several copies with different versions of tools and settings if you want, they won't interfere.

The toolchain is intended to be so easy to get running that you may even consider getting it **just for having it install all tools needed**. Once it has created the "hello world" disk image, you know the tools are there and work.  From that on, you are free to use the tools as you wish!

#### Use style

* No manual file copy ever.
* It's meant to separate cleanly project-specific areas and toolchain-specific areas. Eventually you'll be able to use same toolchain instance to build several projects.
* **Open**: it's all editable text files. Like standing on the shoulder of giants, you can benefit from what it offers without constraint: modify/adjust whatever you want and **use it as a basis for your projects.**

### Can you give more details ?

Prerequisites:

* a Linux-like environment (Windows users will want to install cygwin) with a compiled and widespread tools. This may change in the future (e.g. mingw32, msys, gnuwin32).
* C source code (a "Hello World" is provided).

How to run:

* open a command line, go to the top directory, run make
* currently you have two targets :

        make hello_world_using_z88dk
		make hello_world_using_sdcc

What it does:

* (first time only), get from the internet and compile needed tools (currently z88dk, iDSK, sdcc, hex2bin, as needed)
* compile your project from source to executable

Output produced:

* disk image to run on Amstrad CPC platform (or emulator)
* a message tells you where the image is.

### Details about needed software

The only dependencies are software that is widespread and easily obtainable on any system.

If you are on Windows, install cygwin and when the installer asks you what packages you want, be sure to than the following packages names are selected:

* `make`
* `patch`
* `gcc`
* `wget`
* `bison`, `flex`, `libboost-dev`

`make` is a tool to automate build steps. `patch` is to adjust iDSK source code (and possibly others in the future). `gcc` is the native compiler, to compile CPC compiler and tools. `wget` is used on first run to automatically get CPC-specific software from the Internet. `bison`, `flex` and `libboost-dev` are needed to compile sdcc.

If you are on Linux, use your distribution's package manager, for example on Debian/Ubuntu/derivatives :

	sudo apt-get install make patch gcc wget bison flex

Software that is specific to CPC is automatically fetched and compiled from the Internet.

### Tested platforms

* Ubuntu 11.10 (or was it 11.04 ?)
* Ubuntu 12.10

Test and report on your platform, I'll be happy to help.

### Possible evolutions

These would be possible only with your help:

* integration with major IDEs (any IDE knowing about makefiles and gcc-style output already works)
* rules to automatically convert resources files into formats suitable for inclusions in projects (e.g. PNG image)
* run emulator automatically ?
* cleanly separate portable C and platform-compiler-output-specific parts, to ease not getting trapped in a particular toolset
* offer multi-platform build : run your portable C part as an actual native app (makes sense only if most app logic is in portable C)
