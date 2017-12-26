# Facilities for third-party projects that use cpc-dev-tool-chain

2 scenarios:

* Easy start for users of those project that to checkout and compile.
* Easy process for managers of those projects.

## Desiderata

### User: straight-no-surprise from checkout to a build

User should be able to something very simple, like this:

    # Not assuming any pre-existing copy of cpc-dev-tool-chain.
    git checkout http://github.com/author/third-party-project-using-cpc-dev-tool-chain
    ./configure
    make

This implies that the configure script should be able to fetch a copy
of cpc-dev-tool-chain if needed.

*Optionally*, the configure line may look like this:

    ./configure /path/to/cpc-dev-tool-chain

This option allows the user to specify an existing copy of
cpc-dev-tool-chain instead of letting the project fetch and compile
everything.

### User as developer comfort: out-of-source build

#### What

Just like CMake does.

* The configure step does not generate files in the source
  tree. **Benefit:** source tree remains clean (lightweight, fast text
  search without garbage matches).

* The build step does not even happen in the source tree and so cannot
  change it in any way (add/modify/delete any file/dir).  Everything
  happens in a build tree.

* The user may have as many build trees as they wish pointing to the
  same source tree and build from them without changing the source
  tree.

#### Howto

*Optionally*, the configure line may look like this:

    mkdir /path/to/custom-build-tree
    cd /path/to/custom-build-tree
    /path/to/third-party-project-using-cpc-dev-tool-chain/configure /path/to/cpc-dev-tool-chain
    make    # from build tree

This configures the build tree to refer to the source tree where
`configure` is located, without any copy.

For completeness: it is acceptable that the configure step changes or
adds a Makefile and perhaps a few number of small configuration files
in a way that is specific to cpc-dev-tool-chain, not specific to the
project.

### Multi-target projects: be one of the build option

* A project may consist of a number of source trees, probably under a
  common tree top.  For example one cross-architecture tree and
  several platform-specific trees (for example:
  https://github.com/cpcitor/color-flood-for-amstrad-cpc).

* Files related to cpc-dev-tool-chain do not sit at the root of the
  project tree.  At most, only a configure file specific to
  cpc-dev-tool-chain may sit there.

### Reuse for cpc-dev-tool-chain itself

* cpc-dev-tool-chain includes several test projects (sample programs
that double as unit test, cpcrslib samples, etc).
* Those should reuse the infrastructure simply.  Choose the solution
  that's simplest.

## Rationales

### Why separate configure and make?

* Configure ensures dependency is met (coarse-grain so far, only
  cpc-dev-tool-chain, perhaps later individual components)
* Configure generates empty build tree.
* Build does the build inside build tree.

Trying to do both at the same time complicates maintenance.  A
front-end make checks if cpc-dev-tool-chain is reachable, then passes
to a second one, etc.  That's complicated.
Separating the steps avoids complications.

So, in a sense, using out-of-source build simplifies maintenance of
cpc-dev-tool-chain.

### Why out-of-source build?

#### Formally

* Build trees are generated artifacts which obviously depend on source
  tree.
* By contrast, source don't even need be aware that build trees exist.
  Such awareness creates unnecessary coupling which hinders ability to .

#### Practically

I encountered several situations where out-of-source build is desirable:

* First, searching a string through source tree yields spurious
  results from generated files.
* I made at least one project where the same source was compiled for
  different targets (then, CPC and native Linux executable).  Letting
  everything in same tree won't scale.
* Several times I wished to study how a change in source file or
  compilation options affected generated code.  Having different build
  trees helped.
* Sometimes I had to track a regression and found helpful to keep
  series of compiled trees and compare them with `diff -ur` or emacs
  `ediff-directories`.

Half-baked solutions:

* `.gitignore` states which files to ignore.  A handful of programs
  read and obey `.gitignore`, but not the ones I use (e.g. grep).
* It happens that gcc produces `*.o` and sddd produces `*.asm`,
  `*.lst`, `*.rel`, `*.rst`, `*.sym` for each source file, and also
  `*.bin`, `*.ihx`, `*.lk`, `*.map`, `*.noi` for each executable.
  These don't conflict.  It's like hearing the bullet past your head,
  you're just close.
* The last two can be manually solved by manually copying the whole
  trees.

* Building the project should not add project-specific generated files
  to the source tree.
* Instead, a separate tree containing only generated files is created.

### Why multi-target projects?

#### Formally

* Project files refer to one or more source tree(s) and generate build
  trees.
* By contrast, source trees don't even need be aware that project
  files exist.  Such awareness creates unnecessary coupling.

#### Practical case

A project may consist of a number of source trees, for example one
cross-architecture tree and several platform-specific trees (for
example: https://github.com/cpcitor/color-flood-for-amstrad-cpc).

In that case, no specific project structure is the master of the
project root: each project and build management infrastructure is a
guest in the actual project hierarchy.

So, files related to cpc-dev-tool-chain should not be at the root of
the project tree.  At most, only a configure file or a readme file.
