[[_TOC_]]

cpc-dev-tool-chain
==================

----------------------------------------------------------------

## Quick Summary

### What is this ?

This is a portable toolchain for C/ASM development targetting the Amstrad CPC platform.

### What does it do ?

* It takes your project's C and/or assembly source code and use your modern computer to automatically produces a **ready-to-run disk or tape image** for an Amstrad CPC or emulator.
* You can edit source and get another image, at will.
* Eventually it will allow you to edit your graphics/music/whatever in your preferred editor, save, get another image, run.

### C? But Amstrad did not provide a C-level API!

Sure. But we made one.

To be complete, there are options:

* Use CFWI, our provided **thin** C-level API around CPC **firmware**.  Imagine most elements from amstrad CPC Basic available with the **ease of use of** C.  See [coverage statistics](https://cdn.rawgit.com/cpcitor/cpc-dev-tool-chain/master/cpclib/cfwi/coverage.html).
* If you're after 2D sprite routines consider using a cousin project, [cpctelera](https://github.com/lronaldo/cpctelera "lronaldo/cpctelera: Astonishingly fast Amstrad CPC game engine for C developers").
* From C call **your assembly routines**.  Assembly can call firmware or hardware directly.  As efficient as assembly can be!  Wolfenstein 3D (running on 16bit processor 80286) then Doom were fully written in C except one line drawing routine in assembly!

You can mix-and-match options, like using firmware at initialization because it's easy, then switch to own routines or even pure ASM once hardware is initialized.

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

## More topics

* [Why this tool ?](documentation/why_this_tool.md) more about the situation that `cpc-dev-tool-chain` is meant to solve.
* [How to install](documentation/how_to_install.md)
  * Just get a copy in a local directory. `cpc-dev-tool-chain` only needs a bunch of common tools (definitely available and often already installed in most Linux PCs).
  * It then installs automatically on-demand the needed third party tools (like Z80 compiler).
  * No global change or administrator permission needed.
* [CDTC with custom project](documentation/CDTC_with_custom_project.md): Quickstart tutorial: how do I work with my own project ?

### Tested platforms

* Debian 7.1 Wheezy 64-bits
* Ubuntu 12.10 64-bits
* Ubuntu 13.10 64-bits
* Ubuntu 14.04 64-bits
* Ubuntu 16.04 64-bits
* Reported to work on Windows 7: [How to install cpc-dev-tool-chain on Windows 7 - YouTube](https://www.youtube.com/watch?v=FQ4ToK9ry5U)

If you encounter any trouble (even minor) on your platform, please report and I'll be happy to help.

Until you report, trouble remains.
Once you report, trouble is known, can be fixed for everyone, all will benefit.


### Credits

* Thanks to ronaldo / Cheesetea (ronaldo@cheesetea.com) for testing on cygwin, reporting issues and offering fixes.
* Thanks to artaburu, upstream author of cpcrslib, for cooperation making it easier to use.
