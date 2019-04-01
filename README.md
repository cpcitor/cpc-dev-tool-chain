# cpc-dev-tool-chain

By [cpcitor](https://github.com/cpcitor)

Build status | Linux result
---- | ----
Full build: SDCC cross-compiler + binutils + tape/disk image tools + tape/disk images + test projects | [![Build Status](https://travis-ci.org/cpcitor/cpc-dev-tool-chain.svg?branch=master)](https://travis-ci.org/cpcitor/cpc-dev-tool-chain)
----------------------------------------------------------------

## Quick Summary

### What is this?

This is a portable toolchain for C/ASM development targeting the Amstrad CPC platform.

### What does it do?

* It takes your project's C and/or assembly source code and use your modern computer to automatically produces a **ready-to-run disk or tape image** for an Amstrad CPC or emulator.
* You can edit source and get another image, at will.
* Eventually it will allow you to edit your graphics/music/whatever in your preferred editor, save, get another image, run.

### I want screenshots!

Okay, but remember: a toolchain is **abstract**, a time saver, a guide.  You can make I/O libraries or whatever with it, that does not show on a screenshot.

#### Third-party productions using cpcrslib

![Super Retro Robot Rampage by Vortex](https://cpcrulez.fr/img/6/srrr2.png)
![Kotorans Shadow by Kronoscide](http://www.cpc-power.com/extra_lire_fichier.php?extra=cpcold&fiche=11423&slot=2&part=A&type=.png)

* Raúl Simarro has written [cpcrslib](http://www.cpcwiki.eu/index.php/CPCRSlib).
* cpc-dev-tool-chain makes it easy to use cpcrslib: just `#include "cpcrslib.h"`!
* A number of productions in first editions of [#CPCRetroDev Game Creation Contests](http://cpcretrodev.byterealms.com/en/ "#CPCRetroDev Game Creation Contest | Amstrad CPC game development contest") did just that! Some examples:
  * Super Retro Robot Rampage by Vortex,
  * The Legend Of Cpcerdo by CPCR2,
  * Cavebola by Close To Metal,
  * Kotorans Shadow by Kronoscide,
  * Orcs Dungeon by Fracture Games,
  * VoidHawk by Goodies,
  * Chipotle Warfare by Cosmic Warpigs,
  * Pixel Thief by Zrilon.

#### Production leveraging the CPC firmware style

A project that targets mostly CFWI, the C-level firmware API : [color-flood-for-amstrad-cpc](https://github.com/cpcitor/color-flood-for-amstrad-cpc "cpcitor/color-flood-for-amstrad-cpc: A puzzle game for 2 to 4 player. Written in portable C as an experiment: can a program with non-trivial algorithmic logic be compiled and run on an 8-bit platform? Answer: yes.")

![Playing Color Flood](https://raw.githubusercontent.com/cpcitor/color-flood-for-amstrad-cpc/master/doc/images/dump_2018y12m03d_19h24m32s_174836371.png)
![Playing Color Flood](https://raw.githubusercontent.com/cpcitor/color-flood-for-amstrad-cpc/master/doc/images/dump_2018y12m03d_19h34m30s_522981601.png)

### Okay, nice screenshots.  How can I benefit from this?

Several paths are possible:

* Have you programmed in CPC BASIC already?  This yields a specific style.  Ever dreamt you could write easy, modern C and have an API to provide CPC primitives?  Now you can!  Use CFWI, our provided **thin** C-level API around CPC **firmware**.  See [coverage statistics](https://cdn.rawgit.com/cpcitor/cpc-dev-tool-chain/master/cpclib/cfwi/coverage.html).  Then move on to [How to install](documentation/how_to_install.md).
* If you're after 2D sprite routines consider using a cousin project, [cpctelera](https://github.com/lronaldo/cpctelera "lronaldo/cpctelera: Astonishingly fast Amstrad CPC game engine for C developers").
* Write most code in easy C, easily call **your assembly routines** for specific routines.  Assembly can call firmware or hardware directly.  As efficient as assembly can be!  Remember, on 16bit processor 80286, Wolfenstein 3D then on i386 Doom, were fully written in C except one line drawing routine in assembly!

You can mix-and-match options, like using firmware at initialization because it's easy, then switch to own routines or even pure ASM once hardware is initialized.

----------------------------------------------------------------

## More topics

* [Why this tool?](documentation/why_this_tool.md) more about the situation that `cpc-dev-tool-chain` is meant to solve.
* [How to install](documentation/how_to_install.md)
  * Get a copy in a local directory (e.g. `git clone`). `cpc-dev-tool-chain` only needs a bunch of common tools (definitely available and often already installed in most Linux PCs).  Works on Windows and Mac, too.
  * It then installs automatically on-demand the needed third party tools (like Z80 compiler).
  * No global change or administrator permission needed.
* [CDTC with custom project](documentation/CDTC_with_custom_project.md): Quickstart tutorial: how do I benefit from this in my own project?

### Tested platforms

* Debian 7.1 Wheezy 64-bits
* Ubuntu 12.10 64-bits
* Ubuntu 13.10 64-bits
* Ubuntu 14.04 64-bits
* Ubuntu 16.04 64-bits
* Ubuntu 18.04 64-bits
* Debian 9.7.0
* Reported to work on Windows 7: [How to install cpc-dev-tool-chain on Windows 7 - YouTube](https://www.youtube.com/watch?v=FQ4ToK9ry5U)

If you encounter any trouble (even minor) on your platform, please report and I'll be happy to help.

Until you report, trouble remains.
Once you report, trouble is known, can be fixed for everyone, all will benefit.

### Credits

* Thanks to ronaldo / Cheesetea (ronaldo@cheesetea.com) for testing on Cygwin, reporting issues and offering fixes.
* Thanks to artaburu, upstream author of cpcrslib, for cooperation making it easier to use.
