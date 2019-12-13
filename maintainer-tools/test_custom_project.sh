#!/bin/bash

set -eu

set -xv

cd "$(dirname "$(readlink -f "$0")" )"

SCRIPTDIR="$PWD"

cd ..

export CDTC_ROOT="$PWD"

cd "$SCRIPTDIR"

PROJDIRNAME=my_cdct_project

if [[ -e "${PROJDIRNAME:?}" ]]
then
mv -v --backup=numbered "${PROJDIRNAME:?}" "${PROJDIRNAME:?}".bak
fi

mkdir "${PROJDIRNAME:?}"

cd "${PROJDIRNAME:?}"

"${CDTC_ROOT}"/cdtc-project-setup.sh .

{ echo "#include <stdio.h>" ; echo "int main() { printf(\"Hello World, hello ${USER:-you}.\\n\"); while (1) {} ; return 0; }" ; } >hello.c

make

#{ echo "#include <cpcrslib.h>" ; echo "int main() { cpc_PrintStr(\"Hello World, hello ${USER:-you}.\\n\"); while (1) {} ; return 0; }" ; } >hello.c

#make
