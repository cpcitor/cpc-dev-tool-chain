[Back to main documentation](../README.md)

### Why would I want to use that?

* If you know to program in C and want to try that for the CPC without first learning all details of all tools.
* Or if you're an advanced hacker who like an open starting point to build upon (in that case you'll feel at home with the `Makefile`-based internals).

### Why would I *not* want to use that?

* If you don't want to know that virtually all Integrated Development
  Environments are indeed graphical front-ends for a toolchain.
* If anything looking like a command line or an open-source style
  environment makes you sick.  In that case, it might be interesting
  for you to read Wikipedia's article about
  [Hacker (programmer subculture)](http://en.wikipedia.org/wiki/Hacker_%28programmer_subculture%29)
  and ponder whether some kind of structure or common tools helps
  every member of a community.

### Why have you created this tool ?

#### Programming in C for the CPC is not very easy and documented

* There are **two** main **compilers** with different way of doing things.
* **Several tools** have to be installed (compiler, binary converter, disk/tape image manager and more converters).
* There are several **output formats** on CPC side (regular file on disk, cpm replacement boot sector, ROM, tape, cartridge for CPC+).
* Manual setups are possible following several tutorials
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

Suggested next step: [How to install](how_to_install.md)

[Back to main documentation](../README.md)
