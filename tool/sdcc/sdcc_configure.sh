#!/bin/bash

# This script is intended to be called by make.
# Usage: scriptname buildtree installtree sourcetree

set -eu

set -xv

BUILDTREE="$1"
INSTALLTREE="$2"
SOURCETREE="$3"

shift
shift
shift

cd "$SOURCETREE"
SOURCETREE_ABS="$PWD"
cd -

mkdir -p "${INSTALLTREE}"
cd "${INSTALLTREE}"
INSTALLTREE_ABS="$PWD"
cd -

mkdir -p "${BUILDTREE}"
cd "${BUILDTREE}"
BUILDTREE_ABS="$PWD"

$SOURCETREE_ABS/configure --prefix="${INSTALLTREE_ABS}" \
        --disable-mcs51-port \
        --disable-z180-port \
        --disable-r2k-port \
        --disable-r3ka-port \
        --disable-gbz80-port \
        --disable-tlcs90-port \
        --disable-ds390-port \
        --disable-ds400-port \
        --disable-pic14-port \
        --disable-pic16-port \
        --disable-hc08-port \
        --disable-s08-port \
        --disable-stm8-port \
        --disable-ucsim \
        "$@"

# exit code will tell if we succeeded.
