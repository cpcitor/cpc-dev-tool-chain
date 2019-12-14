[Back to main documentation](../README.md)

### cpc-dev-tool-chain tries to ease installation and use

cpc-dev-tool-chain tries to follow the best practices common with current operating systems.

#### Installation style

* **No manual** file copy.
* **No reboot**/logout/login.
* **No global** system modification.
* Just get a few common programs, run the toolchain, it will fetch CPC-specific software from the internet and compile it for your platform while you have a break.

You can even have several copies with different versions of tools and settings if you want, they won't interfere.

The toolchain is intended to be so easy to get running that you may even consider getting it **just for having it install all tools needed**. Once it has created the "hello world" disk image, you know the tools are there and work.  From that on, you are free to use the tools as you wish!

#### Use style

* No manual file copy ever.
* It's meant to separate cleanly project-specific areas and toolchain-specific areas. You can use the same toolchain instance to build several projects.
* **Open**: it's all editable text files. Like standing on the shoulder of giants, you can benefit from what it offers without constraint: modify/adjust whatever you want and **use it as a basis for your projects.**

### Do I need to prepare my system ?

Maybe.  If you're a developer, your system may already have everything that's needed.

* The only third-party dependencies are software that is widespread and easily obtainable on any system.
* Software that is specific to CPC is automatically fetched and compiled from the Internet.

#### Debian Linux and derivatives (e.g. Ubuntu)

For the widespread tools, use your distribution's package manager, for example on Debian/Ubuntu/derivatives, you can copy-paste this:

	{
    APT="sudo apt-get --no-install-recommends --assume-yes install"
    ${APT:?} git ca-certificates                    		# to fetch the rest
    ${APT:?} wget make patch gcc                    		# common tools
    ${APT:?} g++ texinfo bison flex libboost-dev zlib1g-dev # for sdcc compiler
    ${APT:?} bzip2                                  		# for hex2bin and sdcc
    ${APT:?} unzip                                  		# for addhead
    ${APT:?} libncurses-dev                         		# for cpcxfs
    ${APT:?} libsdl1.2-dev pkgconf libfreetype6-dev 		# for caprice32 emulator (optional)
	}

#### Generic instructions

Look at the Debian/Ubuntu instructions above, find and install the software and move to next step.

#### Windows

**For the impatient: [quick step-by-step instructions for Windows](quick_cygwin_install/README.md).**

Windows can run the needed software using a compatibility layer named cygwin.  Cygwin is mainly a DLL emulating features that Windows does not offer, some precompiled software and an installer.

Install cygwin and when the installer asks you what packages you want, be sure to than the following packages names are selected:

* `git` (unless you use github or any other way to get the framework)
* `make`
* `patch`
* `gcc`, `gcc-core`, `gcc-g++`
* `wget`
* `bison`, `flex`, `libboost-devel`
* `unzip`
* `libncurses-devel`, `ioperm`
* `texinfo`
* `zlib-devel`

You can install all in one go using a command like this.

    \path\to\setup-x86 -q -P git,make,patch,gcc,gcc-core,gcc-g++,wget,bison,flex,libboost-devel,unzip,libncurses-devel,ioperm,texinfo,zlib-devel

Windows command line is not very programmer-friendly.  Fortunately, a drag-n-drop trick allows to get everything working quickly. See [step-by-step instructions for Windows](quick_cygwin_install/README.md).

### After system preparation

Once your computer has the prerequisites, you only have to get a copy of `cpc-dev-tool-chain` and a project to compile.

#### Get cpc-dev-tool-chain itself

* get a copy of `cpc-dev-tool-chain`, for example:

        git clone https://github.com/cpcitor/cpc-dev-tool-chain
		cd cpc-dev-tool-chain

#### Build a project

Building any project (sample or yours) is enough to trigger fetch-from-the-internet and compile of needed tools (cross-compiler, conversion tools, etc).  This is done once and kept for future use.

As a sample project, a "Hello World" is provided as well as examples from cpcrslib.

* open a command line, go to the top directory, optionally run make to get a list of targets
* currently you have these targets:

		make hello_world_using_sdcc
		make hello_world_using_sdcc-all
		make cpcrslib
		make cpcrslib-all

* `make hello_world_using_sdcc-all` builds `hello_world_using_sdcc/hellosdc.dsk`
* `make cpcrslib-all` builds a number of `DSK` files in `cpclib/cpcrslib/cpcrslib-master/examples/*/*.dsk`

When compiling, a message tells you where the image is (path, name).

You can run those images on an emulator or transfer to a real CPC.

You can also build `cdt` files, then `wav` `voc` or `au` audio files that you can play onto a real CPC (either via a good old tape, or via an audio cable from your PC to your CPC.  Just cd into the project directory and issue `make dsk`, `make wav`, etc.

Suggested next step: [CDTC with custom project](CDTC_with_custom_project.md)

[Back to main documentation](../README.md)
