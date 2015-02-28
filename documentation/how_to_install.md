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

#### Generic instructions

Any system capable of running these should be able to run `cpc-dev-tool-chain`:

* `bash`, a command line interpreter also known as "shell".
* `make`,  a tool to automate build steps.
* `wget` used on first run to automatically get CPC-specific software from the Internet.
* `patch`, a tool to adjust iDSK source code (and possibly others in the future).
* `gcc`, the native compiler, to compile CPC compiler and tools.
* `bison`, `flex` and `libboost-dev` are needed to compile sdcc.
* `libncurses-devel` provides `termcap.h` needed by cpcxfs.

#### Debian Linux and derivatives (e.g. Ubuntu)

For the widespread tools, use your distribution's package manager, for example on Debian/Ubuntu/derivatives:

    APT="sudo apt-get --no-install-recommends --assume-yes install"
    ${APT:?} wget make patch gcc               # common tools
    ${APT:?} gzip libmodern-perl-perl          # for z88dk
    ${APT:?} bzip2                             # for hex2bin and sdcc
    ${APT:?} g++ bison flex libboost-dev       # for sdcc

#### Windows

Windows can run the needed software using a compatibility layer named cygwin.  Cygwin is mainly a DLL emulating features that Windows does not offer, some precompiled software and an installer.

Install cygwin and when the installer asks you what packages you want, be sure to than the following packages names are selected:

* `git` (unless you use github or any other way to get the framework)
* `make`
* `patch`
* `gcc`, `gcc-core`, `gcc-g++`
* `wget`
* `unzip`
* `bison`, `flex`, `libboost-devel`
* `libncurses-devel`, `ioperm`

You can install all in one go using a command like this.

    \path\to\setup-x86 -q -P git,make,patch,gcc,gcc-core,gcc-g++,wget,bison,flex,libboost-devel,unrar,libncurses-devel,ioperm

Windows command line is not very programmer-friendly.  Fortunately, a drag-n-drop trick allows to get everything working quickly. See [step-by-step instructions for Windows](quick_cygwin_install/README.md).

### After system preparation

Once your computer has the prerequisites, you only have to get a copy of `cpc-dev-tool-chain` and a project to compile.

As a sample project, a "Hello World" is provided as well as examples from cpcrslib.

* get a copy of `cpc-dev-tool-chain`, for example:

        git clone https://github.com/cpcitor/cpc-dev-tool-chain.git

* open a command line, go to the top directory, optionally run make to get a list of targets
* currently you have these targets:

        make hello_world_using_z88dk
		make hello_world_using_sdcc
		make cpcrslib

What they do:

* (first time only), get from the internet and compile needed tools (currently sdcc or z88dk, hex2bin, cpcxfs or iDSK, cpcrslib, 2cdt, playtzx, as needed)
* compile the project from source to executable

Output produced:

* disk (DSK) or tape image (CDT, VOC) to run on Amstrad CPC platform (or emulator). Try e.g. `make dsk`, `make voc`.
* a message tells you where the image is.

Suggested next step: [CDTC with custom project](CDTC_with_custom_project.md)

[Back to main documentation](../README.md)
