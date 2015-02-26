#!/bin/bash

set -eu

usage() {
        echo "Take a directory full of source code and make a reference to cpc-dev-tool-chain so that it can build it."
        echo
        echo "Usage: $0 (directoryname)*"
        echo "Notice: directoryname can be just '.' to mean 'current directory'."
}

if [ "$#" == "0" ]
then
        usage
        exit 1
fi

cd "$(dirname "$(readlink -f "$0")" )"
SCRIPTDIR="$PWD"
cd - >/dev/null

CDTC_ROOT="${SCRIPTDIR}"

OVERWRITABLE_MARKER="THIS_FILE_WILL_BE_OVERWRITTEN_BY_CDTC"

proceed_with_one_item ()
{
        set -eu
        echo "Current dir is : $PWD"
        echo "Proceeding with : $1"
        cd "$1" || {
                echo "Not a directory I can go into : $1"
                echo "Trying to create a new directory."
                mkdir -p "$1" || { echo "Could not create directory. Aborting." ; return 1; }
                cd "$1" || { echo "Could not enter newly created directory. Aborting." ; return 1; }
        }

        if [[ -r Makefile ]]
        then
                echo "There's already a makefile in: $1"
                if grep -q "$OVERWRITABLE_MARKER" Makefile
                then
                        echo "Notice: will overwrite old generated Makefile in: $1"
                else
                        echo "There's already a makefile that I should probably not overwrite in: $1"
                        return 1
                fi
        fi

        {
                echo "# ${OVERWRITABLE_MARKER}"
                echo "-include cdtc_project.conf"
                echo "-include \$(CDTC_ROOT)/sdcc-project.Makefile"
                echo "failure:"
                echo -e "\011@echo 'Cannot locate cpc-dev-tool-chain main directory.'"
                echo -e "\011@echo 'Hint: edit cdtc_project.conf and adjust line CDTC_ROOT=/path/to/cpc-dev-tool-chain'"
                echo -e "\011@echo 'If you don't have a clue: this project appears to rely on cpc-dev-tool-chain for compilation.'"
                echo -e "\011@echo 'See https://github.com/cpcitor/cpc-dev-tool-chain for instructions.'"
                echo -e "\011@false"
        } >Makefile.tmp

        mv -f Makefile.tmp Makefile

        PROJNAME="$(basename "$PWD" | sed 's/[^a-zA-Z0-9]*//g' | tr 'A-Z' 'a-z' | sed -n 's/^\(........\).*$/\1/p' )"

        if ! [[ -e cdtc_project.conf ]]
        then
                echo "Generating new cdtc_project.conf"
                {
                        echo "PROJNAME=$PROJNAME"
                } >cdtc_project.conf.tmp
                echo "Auto-generated PROJNAME=$PROJNAME"
                mv -f cdtc_project.conf.tmp cdtc_project.conf
        fi

        echo "Setting in cdtc_project.conf CDTC_ROOT=${CDTC_ROOT}"
        {
                echo "CDTC_ROOT=${CDTC_ROOT}"
                [[ -e cdtc_project.conf ]] && grep -v "^CDTC_ROOT" cdtc_project.conf
        } >cdtc_project.conf.tmp
        mv -f cdtc_project.conf.tmp cdtc_project.conf

        return 0
}

while [ "$#" -gt 0 ]
do
        proceed_with_one_item "$1" || echo "Failed for $1"
        shift
done
